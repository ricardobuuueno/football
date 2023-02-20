#pragma once

#include <fmt/core.h>
#include <map>

#include "../plog/Log.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

namespace mysql
{

struct environment;
class server;

extern std::unique_ptr<environment> env;
extern std::unique_ptr<server> srv;

auto init(const std::string &config_filename) -> void;

struct environment final
{
    std::string server;
    std::string port;
    std::string username;
    std::string password;
    std::string database;
};

class server final
{
  public:
    void connect();
    void prepare(const std::string &stmt);
    void execute();

    auto start_transaction() -> void;
    auto commit() -> void;
    auto rollback() -> void;

    [[nodiscard]] auto next() const -> bool;
    [[nodiscard]] auto get_value(const std::string &field) const -> std::string;
    [[nodiscard]] auto has_rows() const -> bool;
    auto get_values(std::map<std::string, std::string> &field_values) const -> void;

  private:
    sql::Driver *driver{nullptr};
    sql::Connection *connection{nullptr};
    sql::Statement *stmt{nullptr};
    sql::PreparedStatement *pstmt{nullptr};
    sql::ResultSet *result{nullptr};
    sql::Savepoint *savepoint{nullptr};

    static std::string configuration_filename;
};

class table
{
  public:
    table(const std::string name, const std::string field1, const std::string field2 = "");
    table(const table &other);
    table(table &&other);

    virtual ~table();

    auto operator<<(std::ostream &o) -> std::ostream &;

    [[nodiscard]] auto save() -> bool;
    [[nodiscard]] auto remove() -> bool;

    [[nodiscard]] auto sql_count() const -> int;

    virtual auto empty() -> bool = 0;
    virtual auto to_string() -> std::string;

    [[nodiscard]] auto next() const -> bool;
    [[nodiscard]] auto get_value(const std::string &field) const -> std::string;

  protected:
    auto start(const std::string &field1, const std::string &value1, const std::string &field2 = "",
               const std::string &value2 = "") -> bool;

    auto start_list(const std::string &field1, const std::string &value1) -> bool;

    [[nodiscard]] auto get(const std::string &field_name) const -> std::string;

    void set(const std::string &field_name, const std::string &value);

    virtual void populate() = 0;
    virtual void reset() = 0;

    virtual auto start_related() -> void
    {
    }

    virtual auto save_related() -> bool
    {
        return true;
    }

  private:
    const std::string tablename;
    const std::string key_field1;
    const std::string key_field2;
    std::map<std::string, std::string> fields_values{};

    int _sql_count{};

    [[nodiscard]] auto fields() const -> std::string;
    [[nodiscard]] auto values() const -> std::string;
    auto key_and_values() -> std::string;

    auto select(const std::string &stmt) -> bool;
    auto select_list(const std::string &stmt) -> bool;
    auto insert() -> bool;
    auto reload() -> bool;
};

} // namespace mysql