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
    sql.execute(u8"CREATE TABLE foo(name TEXT, id INTEGER);");
    
    if(!(auto compiled = sql.compile(u8"INSERT INTO foo VALUES(@name, @id);", ec))) {
        std::cerr << "error! " << ec.cause() << std::endl;
    }
    
    std::vector<std::pair<sqlite::sql_string, int>> table
        = { {u8"Mrs. Roberts", 1337},
            {u8"Robert'); DROP TABLE students;--", 1984},
            {u8"Help! I'm trapped in a driver's license factory Elaine Roberts", 327} };
    
    for(auto i : v) {
        binder(compiled)
            .begin(ec)
                [u8"name", i.first][u8"id", i.second]
            .end();
        
        execute_compiled_statement<>(compiled);
    }
    
    try {
        // cf: line 29.
        binder(compiled)
            .begin()
                [u8"name", u8"boooooo!"][u8"id", 0];
            .end();
        
        execute_compiled_statement<>(compiled);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    auto inserter = sql.compile(u8"INSERT INTO foo VALUES(?, ?);");
    
    binder(inserter)
        .begin(ec)
            /* (name, id) */
            ("booooo!", 0)
            ("hmmm....", 1)
        .end();
    
    execute_compiled_statement<>(inserter);
    /*
    auto query_func
        = functor<void (const sql_string&, int)>(db, u8"INSERT INTO foo VALEUS(@name, @id);")
    
    query_func("chemicl_t", 4, ec);
    
    try {
        query_func("Handai Taro", 5);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
        
    auto selector
        = functor<result_set<sql_string> (const sql_string&)>(db, u8"SELECT name FROM foo;");
    
    selector(u8"Handai Taro");
    */
    db.close();
    
    return 0;
}
