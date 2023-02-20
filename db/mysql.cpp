#include "mysql.hpp"

#include <cppconn/driver.h>
#include <optional>

#include "../util/toml.hpp"

namespace mysql
{
std::unique_ptr<environment> env = nullptr;

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

void server::get_values(std::map<std::string, std::string> &field_values) const
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

// TABLE
// ----------------------------------------------------------------------------------------

// public
table::table(const std::string name, const std::string field1, const std::string field2)
    : tablename(std::move(name)), key_field1(std::move(field1)), key_field2(std::move(field2))
{
    PLOG_DEBUG << to_string();
}

table::table(const table &other)
    : tablename(other.tablename), key_field1(other.key_field1), key_field2(other.key_field2),
      fields_values(other.fields_values)
{
    PLOG_DEBUG << to_string();
}

table::table(table &&other)
    : tablename(std::move(other.tablename)), key_field1(std::move(other.key_field1)),
      key_field2(std::move(other.key_field2)), fields_values(std::move(other.fields_values))
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
    populate();

    if (insert())
    {
        if (reload())
        {
            reset();
            PLOG_DEBUG << to_string();
            return save_related();
        }
    }

    return false;
}

auto table::remove() -> bool
{
    std::string stmt = fmt::format("DELETE FROM {} WHERE {};", tablename, key_and_values());
    PLOG_DEBUG << stmt;

    srv.connect();
    srv.prepare(stmt);
    srv.execute();
    ++_sql_count;

    fields_values.clear();
    reset();

    return true;
}

auto table::sql_count() const -> int
{
    return _sql_count;
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

auto table::next() const -> bool
{
    return srv.next();
}

auto table::get_value(const std::string &field) const -> std::string
{
    return srv.get_value(field);
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

void table::set(const std::string &field_name, const std::string &value)
{
    if (!value.empty())
    {
        fields_values[field_name] = value;
    }
}

// private
auto table::select(const std::string &stmt) -> bool
{
    PLOG_DEBUG << stmt;

    srv.connect();
    srv.prepare(stmt);
    srv.execute();
    ++_sql_count;

    if (srv.has_rows())
    {
        srv.get_values(fields_values);
        return true;
    }

    return false;
}

auto table::select_list(const std::string &stmt) -> bool
{
    PLOG_DEBUG << stmt;

    srv.connect();
    srv.prepare(stmt);
    srv.execute();
    ++_sql_count;

    return srv.has_rows();
}

auto table::insert() -> bool
{
    std::string stmt = fmt::format("REPLACE INTO {} ({}) VALUES ({});", tablename, fields(), values());
    PLOG_DEBUG << stmt;

    srv.connect();
    srv.prepare(stmt);
    srv.execute();
    ++_sql_count;

    return true;
}

auto table::reload() -> bool
{
    std::string stmt = fmt::format("SELECT * FROM {} WHERE {} LIMIT 1;", tablename, key_and_values());
    ;
    return select(stmt);
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
        result += ("'" + value + "'");
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
