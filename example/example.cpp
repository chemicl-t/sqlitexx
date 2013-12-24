#define LIBSQLITEXX_USE_UTF8_STRING

#include "sqlitexx.hpp"

namespace sqlite = caprice::sqlitexx;

int main() {
    sqlite::connection db;
    db.open(u8"test.db");
    
    sqlite::statement sql(db);
    sql.execute(u8"CREATE TABLE foo(name TEXT, id INTEGER);");
    
    // begin transaction
    db.begin_transaction();
    
    auto binder1 = sql.compile(u8"INSERT INTO foo VALUES(@name, @id);");
    
    std::vector<std:pair<sqlite::sql_string, int>> v
        = { {"Mrs. Roberts", 1337},
            {"Robert'); DROP TABLE students;--", 1984},
            {"Help! I'm trapped in a driver's license factory Elaine Roberts", 327} };
    
    for(auto i : v) {
        binder1["name", i.first]["id", i.second];
        sql.execute();
    }
    
    // end transaction
    db.end_transaction();
    
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
