
#ifndef CAPRICE_SQLITEXX_COMPILED_STATEMENT_HPP
#define CAPRICE_SQLITEXX_COMPILED_STATEMENT_HPP

namespace caprice { namespace sqlitexx {

template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 compiled_statement::execute(
    const boost::system::error_code& ec,
    disable_if_maybe_result_set_is_void<ColumnTypes...>*
 ) noexcept {
    if(::sqlite3_bind_parameter_count(stmt.get())) {
        // error...
    }
    
    const bool result = check_result(::sqlite3_step(stmt.get()));
    
    if(!result) {
        // ec...
        return boost::none;
    }
    
    typename result_set<ColumnTypes...>::result_set_object_type v;
    
    ::sqlite3_reset(stmt.get());
    
    return (result_set<ColumnTypes...>)(v);
}

template <typename ...ColumnTypes>
void compiled_statement::execute(
    const boost::system::error_code& ec,
    enable_if_maybe_result_set_is_void<ColumnTypes...>*
 ) noexcept {
    if(::sqlite3_bind_parameter_count(stmt.get())) {
        // error...
    }
    
    const bool result = check_result(::sqlite3_step(stmt.get()));
    
    if(!result) {
        // ec...
    }
    
    ::sqlite3_reset(stmt.get());
}

template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 compiled_statement::execute(disable_if_maybe_result_set_is_void<ColumnTypes...>*) {
    boost::system::error_code ec;
    return throw_error_code(execute<ColumnTypes...>(ec), ec);
}

template <typename ...ColumnTypes>
void compiled_statement::execute(enable_if_maybe_result_set_is_void<ColumnTypes...>*) {
    boost::system::error_code ec;
    execute<ColumnTypes...>(ec);
    
    if(ec) { throw ec; }
}
} }
    
#endif // CAPRICE_SQLITEXX_COMPILED_STATEMENT_HPP
