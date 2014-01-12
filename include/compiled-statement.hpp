
#ifndef SQLITEXX_COMPILED_STATEMENT_HPP
#define SQLITEXX_COMPILED_STATEMENT_HPP

#include <functional>

#include "deftypes.hpp"
#include "string.hpp"
#include "exception.hpp"

#include "connection.hpp"
#include "result-set.hpp"

namespace caprice { namespace sqlitexx {

// foward declaration.
class statement;
class binder;

class compiled_statement {
    friend class statement;
    friend class binder;

    compiled_statement(sql_statement_type stmt_) { stmt = stmt_; }

public:
    compiled_statement() = delete;
    ~compiled_statement() {}
    
    /** @breif execute SQL statement which is compiled at @link compile(const sql_string& src, boost::error_code& ec) @endlink.
     *  @tparam ColumnTypes
     *  @param ec
     *  @retval true
     *          false
     */
    template <typename ...ColumnTypes>
    maybe_result_set<ColumnTypes...>
    execute(const boost::system::error_code& ec,
            disable_if_maybe_result_set_is_void<ColumnTypes...>* = 0) noexcept;
    
    template <typename ...ColumnTypes>
    void execute(const boost::system::error_code& ec,
                 enable_if_maybe_result_set_is_void<ColumnTypes...>* = 0) noexcept;
    
    /** @breif execute SQL statement which is compiled at @link compile(const sql_string& src) @endlink.
     *  @tparam
     *  @retval true
     *          false
     */
    template <typename ...ColumnTypes>
    maybe_result_set<ColumnTypes...>
    execute(disable_if_maybe_result_set_is_void<ColumnTypes...>* = 0);
    
    template <typename ...ColumnTypes>
    void execute(enable_if_maybe_result_set_is_void<ColumnTypes...>* = 0);

private:
    sql_statement_type stmt;
};

typedef maybe<compiled_statement> maybe_compiled_statement;
    
/** @brief
 *  @tparam ColumnTypes
 *  @param stmt
 *  @param src
 *  @param ec
 */
template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 execute_compiled_statement(
    compiled_statement& stmt,
    boost::system::error_code& ec,
    disable_if_maybe_result_set_is_void<ColumnTypes...>* = 0
 ) noexcept {
    return stmt.template execute<ColumnTypes...>(ec);
}
    
template <typename ...ColumnTypes>
void execute_compiled_statement(
    compiled_statement& stmt,
    boost::system::error_code& ec,
    enable_if_maybe_result_set_is_void<ColumnTypes...>* = 0
 ) noexcept {
    stmt.template execute<ColumnTypes...>(ec);
}

/** @brief
 *  @tparam ColumnTypes
 *  @param stmt
 *  @param src
 */
template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 execute_compiled_statement(
    compiled_statement& stmt,
    disable_if_maybe_result_set_is_void<ColumnTypes...>* = 0
 ) {
    boost::system::error_code ec;
    auto result = stmt.template execute<ColumnTypes...>(ec);
    
    if (ec) { /* error handling... */ }
     
    return result;
}

template <typename ...ColumnTypes>
void execute_compiled_statement(
    compiled_statement& stmt,
    enable_if_maybe_result_set_is_void<ColumnTypes...>* = 0
 ) {
    boost::system::error_code ec;
    stmt.template execute<ColumnTypes...>(ec);
    
    if (ec) { /* error handling... */ }
}

} }

#include "detail/compiled-statement.hpp"

#endif // SQLITEXX_COMPILED_STATEMENT_HPP
