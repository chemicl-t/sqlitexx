
#ifndef CAPRICE_SQLITEXX_DETAIL_STATEMNT_HPP
#define CAPRICE_SQLITEXX_DETAIL_STATEMNT_HPP

namespace caprice { namespace sqlitexx {

statement::maybe_compiled_statement
 statement::compile(const sql_string& src, boost::system::error_code& ec) noexcept {
    // src is incomplete or empty, or db is unconstructed...
    if(!::sqlite3_complete(src.c_str()) || src.empty() || !db) {
        return boost::none;
    } else {
        ::sqlite3_prepare_v2(db.get(), src, -1, stmt.get(), nullptr);
        return maybe_compiled_statement(compiled_statement(db, stmt));
    }
}
    
statement::maybe_compiled_statement
 statement::compile(const sql_string& src) {
    boost::error_code ec;
    auto v = compile(src, ec);
    
    if (ec) { throw ec; }
    
    return v;
}
    
template <typename ...ColumnTypes>
statement::maybe_result_set<ColumnTypes...>
 statement::execute(const sql_string& src, boost::system::error_code& ec) noexcept {
    auto v = compile(src, ec);
    
    if(ec) { return boost::none; }
    
    return compiled_statement(db, stmt).template execute<ColumnTypes...>(ec);
}
    
template <typename ...ColumnTypes>
statement::maybe_result_set<ColumnTypes...>
 statement::execute(const sql_string& src) {
    boost::error_code ec;
    auto retval = execute(src, ec);
    
    if(ec) { throw ec; }
    
    return retval;
}

} }

#endif // CAPRICE_SQLITEXX_DETAIL_STATEMNT_HPP
