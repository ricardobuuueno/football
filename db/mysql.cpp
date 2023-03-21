#include "mysql.hpp"

#include <cppconn/driver.h>
#include <optional>

#include "../util/json.hpp"
#include "../util/toml.hpp"

namespace mysql
{
std::unique_ptr<environment> env = nullptr;
std::unique_ptr<server> srv = nullptr;

auto init(const std::string &config_filename) -> void
{
    env = std::make_unique<environment>();

    toml::table config = toml::parse_file(config_filename);
    auto my_config = config["mysql"];

    env->server = my_config["server"].value<std::string>().value();
    env->port = my_config["port"].value<std::string>().value();
    env->username = my_config["username"].value<std::string>().value();
    env->password = my_config["password"].value<std::string>().value();
    env->database = my_config["database"].value<std::string>().value();

    srv = std::make_unique<server>();
}

// SERVER
// ----------------------------------------------------------------------------------------

void server::connect()
{
    if (!driver)
    {
        driver = get_driver_instance();
    }

    if (!connection)
    {

        connection = driver->connect(env->server, env->username, env->password);

        connection->setSchema(env->database);
    }
    else if (connection->isClosed())
    {
        connection->reconnect();
    }
}

void server::prepare(const std::string &stmt)
{
    pstmt = connection->prepareStatement(stmt);
}

void server::execute()
{
    result = pstmt->executeQuery();
}

std::string server::prepare_execute(const std::string &stmt)
{
    std::scoped_lock(server_mutex);
    prepare(stmt);
    execute();

    std::vector<std::map<std::string, std::string>> tbl;
    tbl.reserve(result->rowsCount());

    auto meta = result->getMetaData();
    while (result->next())
    {
        std::map<std::string, std::string> row{};
        for (int i{1}; i <= meta->getColumnCount(); ++i)
        {
            auto field = meta->getColumnName(i);
            auto value = result->getString(i);
            row[field] = value;
        }
        tbl.emplace_back(std::move(row));
    }

    nlohmann::json j(tbl);

    return j.dump();
}

auto server::start_transaction() -> void
{
    if (connection)
    {
        savepoint = connection->setSavepoint();
    }
}

auto server::commit() -> void
{
    if (connection)
    {
        connection->commit();
    }
}

auto server::rollback() -> void
{
    if (connection)
    {
        connection->rollback(savepoint);
    }
}

auto server::next() const -> bool
{
    return (result != nullptr && result->next());
}

auto server::get_value(const std::string &field) const -> std::string
{
    auto meta = result->getMetaData();
    for (int i{1}; i <= meta->getColumnCount(); ++i)
    {
        if (auto f = meta->getColumnName(i); f == field)
        {
            return result->getString(i);
        }
    }
    return {};
}

auto server::has_rows() const -> bool
{
    PLOG_DEBUG << result->rowsCount();
    return (result != nullptr && result->rowsCount() > 0);
}

auto server::get_values(std::map<std::string, std::string> &field_values) const -> void
{
    auto meta = result->getMetaData();
    PLOG_DEBUG << "Column count: " << meta->getColumnCount();

    while (result->next())
    {
        for (int i{1}; i <= meta->getColumnCount(); ++i)
        {
            auto field = meta->getColumnName(i);
            auto value = result->getString(i);
            field_values[field] = value;
        }
        break;
    }
}

auto server::last_insert_id(const std::string &tablename) -> uint64_t
{
    std::string stmt = fmt::format("SELECT LAST_INSERT_ID() FROM {} LIMIT 1;", tablename);

    prepare(stmt);
    execute();

    uint64_t value{};
    while (result->next())
    {
        value = result->getUInt64(1);
    }
    PLOG_DEBUG << stmt << " (result: " << value << ")";

    return value;
}

// TABLE
// ----------------------------------------------------------------------------------------

// public
table::table(const std::string name, const std::string field1, const std::string field2)
    : tablename(std::move(name)), key_field1(std::move(field1)), key_field2(std::move(field2)), _found(false)
{
    PLOG_DEBUG << to_string();
}

table::table(const table &other)
    : tablename(other.tablename), key_field1(other.key_field1), key_field2(other.key_field2),
      fields_values(other.fields_values), _found(other._found)
{
    PLOG_DEBUG << to_string();
}

table::table(table &&other)
    : tablename(std::move(other.tablename)), key_field1(std::move(other.key_field1)),
      key_field2(std::move(other.key_field2)), fields_values(std::move(other.fields_values)), _found(other._found)
{
    PLOG_DEBUG << to_string();
}

table::~table()
{
    PLOG_DEBUG << to_string();
}

auto table::operator<<(std::ostream &o) -> std::ostream &
{
    return o << to_string();
}

auto table::save() -> bool
{
    std::scoped_lock(table_mutex);

    try
    {
        populate();
    }
    catch (std::exception &e)
    {
        std::string message = fmt::format("[EXCEPTION] {}", e.what());
        PLOG_DEBUG << message;
        return false;
    }

    if (auto [inserted, error_message] = insert(); inserted)
    {
        if (reload())
        {
            reset();
            PLOG_DEBUG << to_string();
            return save_related();
        }
    }
    else
    {
        PLOG_DEBUG << error_message;
    }

    return false;
}

auto table::remove() -> bool
{
    std::string stmt = fmt::format("DELETE FROM {} WHERE {};", tablename, key_and_values());
    PLOG_DEBUG << stmt;

    srv->connect();
    srv->prepare(stmt);
    srv->execute();
    ++_sql_count;

    fields_values.clear();
    reset();

    return true;
}

auto table::sql_count() const -> int
{
    return _sql_count;
}

auto table::found() const -> bool
{
    return _found;
}

auto table::to_string() -> std::string
{
    std::string str{};
    for (auto const &[field, value] : fields_values)
    {
        str += (field + ": " + value + " ");
    }
    return fmt::format("[tablename: {}, key1: {}, key 2: {} fields: ({})]", tablename, key_field1, key_field2, str);
}

auto table::id_int() const -> uint64_t
{
    auto idd = id();
    return (idd.empty() ? 0 : std::stoi(idd));
}

auto table::next() const -> bool
{
    return srv->next();
}

auto table::get_value(const std::string &field) const -> std::string
{
    return srv->get_value(field);
}

// protected
auto table::start(const std::string &field1, const std::string &value1, const std::string &field2,
                  const std::string &value2) -> bool
{
    if (value1.empty())
    {
        return false;
    }

    std::string second_condition{};
    if (!value2.empty())
    {
        second_condition = fmt::format("AND {} = '{}'", field2, value2);
    }

    std::string stmt = fmt::format("SELECT {} FROM {} WHERE {} = '{}' {} LIMIT 1;", fields(), tablename, field1, value1,
                                   second_condition);

    if (select(stmt))
    {
        return true;
    }

    set(field1, value1);

    if (!value2.empty())
    {
        set(field2, value2);
    }

    return false;
}

auto table::start_list(const std::string &field1, const std::string &value1) -> bool
{
    if (value1.empty())
    {
        return false;
    }

    std::string stmt = fmt::format("SELECT {} FROM {} WHERE {} = '{}';", fields(), tablename, field1, value1);

    if (select_list(stmt))
    {
        set(field1, value1);
        return true;
    }

    return false;
}

auto table::get(const std::string &field_name) const -> std::string
{
    if (auto search = fields_values.find(field_name); search != fields_values.end())
    {
        return search->second;
    }
    return {};
}

auto table::get_int(const std::string &field_name) const -> uint64_t
{
    if (auto search = fields_values.find(field_name); search != fields_values.end())
    {
        return std::stoi(search->second);
    }
    return 0;
}

auto table::set(const std::string &field_name, const std::string &value) -> void
{
    if (!value.empty())
    {
        fields_values[field_name] = value;
    }
}

auto table::set(const std::string &field_name, uint64_t value) -> void
{
    fields_values[field_name] = std::to_string(value);
}

// private
auto table::select(const std::string &stmt) -> bool
{
    PLOG_DEBUG << stmt;

    srv->connect();
    srv->prepare(stmt);
    srv->execute();
    ++_sql_count;

    _found = srv->has_rows();
    if (_found)
    {
        srv->get_values(fields_values);
        return true;
    }

    return false;
}

auto table::select_list(const std::string &stmt) -> bool
{
    PLOG_DEBUG << stmt;

    srv->connect();
    srv->prepare(stmt);
    srv->execute();
    ++_sql_count;

    _found = srv->has_rows();

    return _found;
}

auto table::insert() -> std::pair<bool, std::string>
{
    std::string message{};
    bool result{false};

    std::string stmt = fmt::format("REPLACE INTO {} ({}) VALUES ({});", tablename, fields(), values());
    PLOG_DEBUG << stmt;

    try
    {
        srv->connect();
        srv->prepare(stmt);
        srv->execute();
        ++_sql_count;

        result = true;
    }
    catch (sql::SQLException &e)
    {
        message = fmt::format("[EXCEPTION] error_code: {}, message: {}", e.getErrorCode(), e.what());
    }

    return {result, message};
}

auto table::reload() -> bool
{
    if (must_get_id())
    {
        auto last_id = srv->last_insert_id(tablename);
        fields_values[key_field1] = std::to_string(last_id);
    }

    std::string stmt = fmt::format("SELECT * FROM {} WHERE {} LIMIT 1;", tablename, key_and_values());

    return select(stmt);
}

auto table::must_get_id() -> bool
{
    if (key_field1 == "id")
    {
        auto value = fields_values[key_field1];
        return (value.empty() || value == "NULL");
    }
    return false;
}

auto table::fields() const -> std::string
{
    if (fields_values.empty())
    {
        return {"*"};
    }

    std::string result{};

    size_t count{0};
    for (const auto &[field, value] : fields_values)
    {
        result += field;
        if (++count < fields_values.size())
        {
            result += ", ";
        }
    }

    return result;
}

auto table::values() const -> std::string
{
    std::string result{};

    size_t count{0};
    for (const auto &[field, value] : fields_values)
    {
        result += ((value == "NULL") ? value : ("'" + value + "'"));
        if (++count < fields_values.size())
        {
            result += ", ";
        }
    }

    return result;
}

auto table::key_and_values() -> std::string
{
    std::string stmt = fmt::format("{} = '{}'", key_field1, fields_values[key_field1]);
    if (!key_field2.empty())
    {
        stmt += fmt::format(" AND {} = '{}'", key_field2, fields_values[key_field2]);
    }
    return stmt;
}

} // namespace mysql
