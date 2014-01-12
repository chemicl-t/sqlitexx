
#ifndef CAPRICE_SQLITEXX_DETAIL_STATEMNT_HPP
#define CAPRICE_SQLITEXX_DETAIL_STATEMNT_HPP

namespace caprice { namespace sqlitexx {

maybe_compiled_statement
 statement::compile(const sql_string& src, boost::system::error_code& ec) noexcept {
    // src is incomplete or empty, or db is unconstructed...
    if(!::sqlite3_complete(src.c_str()) || src.empty() || !db) {
        return boost::none;
    } else {
        auto stmt_given_param = stmt.get();
        ::sqlite3_prepare_v2(db.get(), src.c_str(), -1, &stmt_given_param, nullptr);
        return maybe_compiled_statement(compiled_statement(stmt));
    }
}


maybe_compiled_statement
 statement::compile(const sql_string& src) {
    boost::system::error_code ec;
    auto v = compile(src, ec);
    
    if (ec) { throw ec; }
    
    return v;
}

template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 statement::execute(
    const sql_string& src,
    boost::system::error_code& ec,
    disable_if_maybe_result_set_is_void<ColumnTypes...>*
 ) noexcept {
    auto v = compile(src, ec);
    
    if(ec) { return boost::none; }
    
    return compiled_statement(stmt).template execute<ColumnTypes...>(ec);
}
    
template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 statement::execute(
    const sql_string& src,
    disable_if_maybe_result_set_is_void<ColumnTypes...>*
 ) {
    boost::system::error_code ec;
    auto retval = execute<ColumnTypes...>(src, ec);
    
    if(ec) { throw ec; }
    
    return retval;
}

template <typename ...ColumnTypes>
void statement::execute(
    const sql_string& src,
    boost::system::error_code& ec,
    enable_if_maybe_result_set_is_void<ColumnTypes...>*
 ) noexcept {
    auto v = compile(src, ec);

    if(ec) { /* error handling... */ }

    compiled_statement(stmt).template execute<ColumnTypes...>(ec);
}

template <typename ...ColumnTypes>
void statement::execute(
    const sql_string& src,
    enable_if_maybe_result_set_is_void<ColumnTypes...>*
 ) {
    boost::system::error_code ec;
    execute<ColumnTypes...>(src, ec);
     
    if(ec) { throw ec; }
}

} }

#endif // CAPRICE_SQLITEXX_DETAIL_STATEMNT_HPP
