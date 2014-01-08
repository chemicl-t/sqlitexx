
#ifndef CAPRICE_SQLITEXX_STRING_HPP
#define CAPRICE_SQLITEXX_STRING_HPP

#include <sqlite3.h>

#ifndef LIBSQLITEXX_USED_BY_GIKOMONA

#include <string>
#include <boost/utility/string_ref.hpp>

#ifdef LIBSQLITEXX_USE_UTF8_STRING

namespace caprice { namespace sqlitexx {

    typedef char u8_char;
    typedef u8_char sql_char;

    #define _sqlSTR(str) (u8##str)

} }


#elif LIBSQLITEXX_USE_UTF16_STRING

namespace caprice { namespace sqlitexx {
    
    typedef char16_t u16_char;
    typedef u16_char sql_char;

    #define _sqlSTR(str) (u16##str)

} }

// so dirty!!!!!!!!!!!!!!!!!!!!!!
// #define <utf-8 supported> <utf-16 supported>
#define sqlite3_bind_text            sqlite3_bind_text16
#define sqlite3_collation_needed     sqlite3_collation_needed16
#define sqlite3_column_bytes         sqlite3_column_bytes16
#define sqlite3_column_database_name sqlite3_column_database_name16
#define sqlite3_column_decltype      sqlite3_column_decltype16
#define sqlite3_column_name          sqlite3_column_name16
#define sqlite3_column_origin_name   sqlite3_column_origin_name16
#define sqlite3_column_table_name    sqlite3_column_table_name16
#define sqlite3_column_text          sqlite3_column_text16
#define sqlite3_complete             sqlite3_complete16
#define sqlite3_create_function      sqlite3_create_function16
#define sqlite3_create_collation     sqlite3_create_collation16
#define sqlite3_errmsg               sqlite3_errmsg16
#define sqlite3_open                 sqlite3_open16
#define sqlite3_prepare              sqlite3_prepare16
#define sqlite3_prepare_v2           sqlite3_prepare16_v2
#define sqlite3_result_error         sqlite3_result_error16
#define sqlite3_result_text          sqlite3_result_text16
#define sqlite3_value_text           sqlite3_value_text16

#endif // LIBSQLITEXX_USE_ *

namespace caprice { namespace sqlitexx {
    
    typedef std::basic_string<sql_char, std::char_traits<sql_char>> sql_string;
    typedef boost::basic_string_ref<sql_char, std::char_traits<sql_char>> sql_string_ref;

} }

#else // defined LIBSQLITEXX_PRAGMA_USED_BY_GIKOMONA

namespace caprice { namespace sqlitexx {

    typedef monazilla::GikoMona::u8_char sql_char;
    typedef monazilla::GikoMona::u8_string sql_string;
    typedef monazilla::GikoMona::u8_string_ref sql_string_ref;
    #define _sqlSTR(str) (u8 str)
    
} }

#endif // LIBSQLITEXX_PRAGMA_USED_BY_GIKOMONA

#endif // CAPRICE_SQLITEXX_STRING_HPP
