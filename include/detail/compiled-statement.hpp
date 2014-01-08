
#ifndef CAPRICE_SQLITEXX_COMPILED_STATEMENT_HPP
#define CAPRICE_SQLITEXX_COMPILED_STATEMENT_HPP

namespace caprice { namespace sqlitexx {

template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 compiled_statement::execute(const boost::system::error_code& ec) noexcept {
    if(::sqlite3_bind_parameter_count(stmt.get())) {
        // error...
    }
    
    const bool result = check_result(::sqlite3_step(stmt.get()));
    
    if(!result) {
        // ec...
        return boost::none;
    }
    
    result_set<ColumnTypes...>::result_set_object_type v;
    
    ::sqlite3_reset(stmt.get());
    
    return result_set<ColumnTypes...>(v);
}

template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 compiled_statement::execute() {
    boost::error_code ec;
    return throw_error_code(execute(src), ec);
}

} }
    
#endif // CAPRICE_SQLITEXX_COMPILED_STATEMENT_HPP
