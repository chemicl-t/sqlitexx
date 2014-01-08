
#ifndef SQLITEXX_COMPILED_STATEMENT_HPP
#define SQLITEXX_COMPILED_STATEMENT_HPP

#include <tuple>
#include <functional>

#include <boost/optional.hpp>

#include "deftypes.hpp"
#include "string.hpp"
#include "exception.hpp"

#include "connection.hpp"
#include "result-set.hpp"

namespace caprice { namespace sqlitexx {

// foward declaration.
class statement;

class compiled_statement {
    friend class statement;

    compiled_statement(db_object_type db_, sql_statement_type stmt_) {
        db = db_;
        stmt = stmt_;
    }
    
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
    maybe_result_set<ColumnTypes...> execute(const boost::system::error_code& ec) noexcept;
    
    /** @breif execute SQL statement which is compiled at @link compile(const sql_string& src) @endlink.
     *  @tparam
     *  @retval true
     *          false
     */
    template <typename ...ColumnTypes>
    maybe_result_set<ColumnTypes...> execute();
    
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
 execute_compiled_statement(compiled_statement& stmt,
                            const sql_string& src,
                            boost::system::error_code& ec) noexcept {
    return stmt.template execute<ColumnTypes...>(src, ec);
}

/** @brief
 *  @tparam ColumnTypes
 *  @param stmt
 *  @param src
 */
template <typename ...ColumnTypes>
maybe_result_set<ColumnTypes...>
 execute_compiled_statement(compiled_statement& stmt, const sql_string& src) {
    return stmt.template execute<ColumnTypes...>(src);
}

} }

#include "detail/compiled-statement.hpp"

#endif // SQLITEXX_COMPILED_STATEMENT_HPP
