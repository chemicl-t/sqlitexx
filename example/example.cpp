#define LIBSQLITEXX_USE_UTF8_STRING

#include <vector>
#include <utility>

#include "sqlitexx.hpp"

namespace sqlite = caprice::sqlitexx;

int main() {
    sqlite::connection db;
    boost::system::error_code ec;
    
    db.open(u8"test.db", ec);
    
    sqlite::statement sql(db);
    
    // it's so dirty!
    sql.template execute<>(u8"CREATE TABLE foo(name TEXT, id INTEGER);");
    
    // cf: line 19.
    // good !
    sqlite::execute_statement<>(sql, u8"CREATE TABLE bar(name TEXT, id INTEGER);", ec);
    sqlite::execute_statement<>(sql, u8"DROP TABLE IF EXIST bar;", ec);
    
    
    auto compiled = sql.compile(u8"INSERT INTO foo VALUES(@name, @id);", ec);
    
    if(!ec) { std::cerr << "error! " << ec.message() << std::endl; }
    
    std::vector<std::pair<sqlite::sql_string, int>> table
        = { {u8"Mrs. Roberts", 1337},
            {u8"Robert'); DROP TABLE students;--", 1984},
            {u8"Help! I'm trapped in a driver's license factory Elaine Roberts", 327} };
    
    for(auto i : table) {
        sqlite::binder(*compiled)
            .begin(ec)
                (u8"name", i.first)(u8"id", i.second)
            .end();
        
        sqlite::execute_compiled_statement<>(*compiled);
    }
    
    try {
        // cf: line 29.
        sqlite::binder(*compiled)
            .begin()
                (u8"name", u8"boooooo!")(u8"id", 0)
            .end();
        
        sqlite::execute_compiled_statement<>(*compiled);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    db.close();
    
    return 0;
}
