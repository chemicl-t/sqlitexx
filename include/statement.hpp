
#ifndef CAPRICE_SQLITEXX_STATEMENT_HPP
#define CAPRICE_SQLITEXX_STATEMENT_HPP

#include "compiled-statement.hpp"

namespace caprice { namespace sqlitexx {

class statement {
public:
    /** @brief constructor.
     *  @param con[in] @link connction @endlink instance.
     */
    explicit statement(connection& con) noexcept {
        db = con.get_db_object();
    }

    /// @brief destructor.
    ~statement() noexcept {}

    /** @brief compile SQL statement.
     *  @warning when you pass multi SQL statements at once, the result is undefined.
     *  @param src[in] SQL statement.
     *  @param ec[out]
     */
    maybe_compiled_statement compile(const sql_string& src, boost::system::error_code& ec) noexcept;
    
    /** @brief compile SQL statement.
     *  @warning when you pass multi SQL statements at once, the result is undefined.
     *  @param src[in] SQL statement.
     */
    maybe_compiled_statement compile(const sql_string& src);
    
    /** @brief execute SQL statement.
     *  @warning when you pass multi SQL statements at once, the result is undefined.
     *  @param src[in] SQL statement.
     *  @param ec[out] .
     *  @retval 
     */
    template <typename ...ColumnTypes>
    maybe_result_set<ColumnTypes...> execute(const sql_string& src, boost::system::error_code& ec) noexcept;
    
    /** @brief execute SQL statement.
     *  @warning when you pass multi SQL statements at once, the result is undefined.
     *  @param src[in] SQL statement.
     *  @retval 
     */
    template <typename ...ColumnTypes>
    maybe_result_set<ColumnTypes...> execute(const sql_string& src);
    
    /// @brief return database object.
    db_object_type get_db_object() { return db; }

private:
    db_object_type db;
    sql_statement_type stmt;
    bool param_binding_is_finished;
};

typedef maybe<statement> maybe_statement;

/** @brief
 *  @param stmt
 *  @param src
 *  @param ec
 */
maybe_compiled_statement
 compile_statement(statement& stmt,
                  const sql_string& src,
                   boost::system::error_code& ec) noexcept {
    return stmt.compile(src, ec);
}

/** @brief
 *  @param stmt
 *  @param src
 */
maybe_compiled_statement
 compile_statement(statement& stmt, const sql_string& src) {
    return stmt.compile(src);
}

/** @brief
 *  @tparam ColumnTypes
 *  @param stmt
 *  @param src
 *  @param ec
 */
template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 execute_statement(statement& stmt, const sql_string& src, boost::system::error_code& ec) noexcept {
    return stmt.template execute<ColumnTypes...>(src, ec);
}

/** @brief
 *  @tparam ColumnTypes
 *  @param stmt
 *  @param src
 *  @param ec
 */
template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 execute_statement(statement& stmt, const sql_string& src) {
    return stmt.template execute<ColumnTypes...>(src);
}

} }

#include "detail/statement.hpp"

#endif // CAPRICE_SQLITEXX_STATEMENT_HPP
