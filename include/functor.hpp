#ifndef CAPRICE_SQLITEXX_FUNCTOR_HPP
#define CAPRICE_SQLITEXX_FUNCTOR_HPP

/// @note now under construction. I can't solve the type-associated problems...

/// @internal

#if 0

#include "deftypes.hpp"
#include "string.hpp"
#include "exception.hpp"

#include "connection.hpp"
#include "statement.hpp"
#include "binder.hpp"
#include "result-set.hpp"

namespace caprice { namespace sqlitexx {

template <typename R, typename ...Args>
class functor final {
public:
    functor() = delete;
    ~functor() = delete;
};

template <typename ...ValueType, typename ...Args>
class functor <result_set<ValueType...> (Args...)> final {
public:
    std::tuple<Args...> arg_type;
    typedef result_set<ValueType...> result_type;
    
    functor(connection& conn, const sql_string& sql) {
        static_assert(sizeof ...ValueType, "result-set must hold more than 1 template-paramater.");
        
        if(auto compiled_stmt = statement(conn).compile(sql)) { stmt(*compiled_stmt); }
        
        arg_binder(stmt);
    }
    
    functor(connection& conn,
            const sql_string& sql,
            boost::error_code& error) noexcept {
        static_assert(sizeof ...ValueType, "result-set must hold more than 1 template-paramater.");
        
        if(auto compiled_stmt = statement(conn).compile(sql, error)) { stmt(*compiled_stmt); }
        
        arg_binder(stmt);
    }
    
    ~functor() noexcept {}
    
    maybe_result_set<ValueType...> operator()(Args... value) {
        arg_binder
            .begin()
                (value...)
            .end();
        
        auto result = execute_statement<ValueType...>(stmt);
        arg_binder.reset();
        
        return result;
    }
    
    maybe_result_set<ValueType...>
    operator()(Args... value, boost::error_code& ec) noexcept {
        arg_binder
            .begin(ec)
                (value...)
            .end();
        
        auto result = execute_statement<ValueType...>(stmt);
        arg_binder.reset();
        
        return result;
    }
    
private:
    compiled_statement stmt;
    binder arg_binder;
};

} }

#endif

/// @endinternal

#endif // CAPRICE_SQLITEXX_FUNCTOR_HPP