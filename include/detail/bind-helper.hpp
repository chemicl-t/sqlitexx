/// @internal

#ifndef CAPRICE_SQLITEXX_BIND_HELPER_HPP
#define CAPRICE_SQLITEXX_BIND_HELPER_HPP

#include <functional>

#include "deftypes.hpp"

namespace caprice {  namespace sqlitexx {

typedef std::function<int (const int)> bounded_function_type;

bounded_function_type
 do_bind(sql_statement_type stmt_, std::nullptr_t /* tag */) noexcept {
    return std::bind(::sqlite3_bind_null, stmt_.get(), std::placeholders::_1);
}

bounded_function_type
 do_bind(sql_statement_type stmt_, const double val) noexcept {
    return std::bind(::sqlite3_bind_double, stmt_.get(), std::placeholders::_1, val);
}

bounded_function_type
 do_bind(sql_statement_type stmt_, const int val) noexcept {
    return std::bind(::sqlite3_bind_int, stmt_.get(), std::placeholders::_1, val);
}

bounded_function_type
 do_bind(sql_statement_type stmt_, const std::int64_t val) noexcept {
    return std::bind(::sqlite3_bind_int64,
                     stmt_.get(),
                     std::placeholders::_1,
                     static_cast<::sqlite3_int64>(val));
}

bounded_function_type
 do_bind(sql_statement_type stmt_, blob_type& val) noexcept {
    return std::bind(::sqlite3_bind_blob,
                     stmt_.get(),
                     std::placeholders::_1,
                     val.value(),
                     val.size(),
                     SQLITE_TRANSIENT);
}

bounded_function_type
 do_bind(sql_statement_type stmt_, const sql_string& val) noexcept {
    return std::bind(::sqlite3_bind_text,
                     stmt_.get(),
                     std::placeholders::_1,
                     val.c_str(),
                     val.size() * sizeof(sql_string::value_type),
                     SQLITE_TRANSIENT);
}

// sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
// sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);

template <typename T>
bounded_function_type
 do_bind(sql_statement_type stmt_, T val) = delete;

} }

#endif // CAPRICE_SQLITEXX_BIND_HELPER_HPP

/// @endinternal
