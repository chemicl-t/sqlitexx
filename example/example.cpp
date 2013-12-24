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
        binder1
            .begin()
                ["name", i.first]["id", i.second]
            .end();
        
        execute_compiled_statement();
    }
    
    auto binder2 = sql.compile(u8"INSERT INTO foo VALUES(?, ?);");
    
    try {
        binder1["name", "boooooo!"]["id", 0]; // invalid
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    binder2("booooo!", 0)("hmmm....", 1);
    sql.execute();
    
    sql.execute(u8"INSERT INTO foo VALUES(?, ?);", "sqlitexx", 2);
    sql.execute(u8"INSERT INTO foo VALEUS(@name, @id);", {"name", "caprice"}, {"id", 3});

    auto query_func = sql.compile(u8"INSERT INTO foo VALEUS(@name, @id);")
                         .do_functionize<void (const sql_string&, int)>();
    
    query_func("chemicl_t", 4);
    query_func("Handai Taro", 5);
    
    auto l = sql.compile(u8"SELECT name FROM foo;")
                .do_functionize<result_set<sql_string> ()>();
    
    db.close();
    
    return 0;
}
