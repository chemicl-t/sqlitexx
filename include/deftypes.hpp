
#ifndef CAPRICE_SQLITEXX_DEFTYPES_HPP
#define CAPRICE_SQLITEXX_DEFTYPES_HPP

#include <sqlite3.h>

#include <boost/optional.hpp>

#include "exception.hpp"

namespace boost {
    template <>
    using optional<void> = void;
    
    template <>
    using optional<> = void;
}

namespace caprice { namespace sqlitexx {

    typedef ::sqlite3 raw_db_object_type;
    typedef ::sqlite3_stmt raw_sql_statement_type;
    
    typedef std::shared_ptr<raw_db_object_type> db_object_type;
    typedef std::shared_ptr<raw_sql_statement_type> sql_statement_type;
    
    void db_object_deleter(raw_db_object_type *obj) {
        ::sqlite3_close_v2(obj);
    }
    
    void sql_statement_deleter(raw_sql_statement_type *obj) {
        ::sqlite3_finalize(obj);
    }
    
    typedef ::sqlite3_blob *db_blob_type;
    
    struct blob_type {
        typedef db_blob_type value_type;
        
        blob_type(value_type v, std::size_t sz) {
            std::copy()
        }
        
        ~blob_type() {
            
        }
        
        value_type value() { return obj; }
        std::size_t size() { return length; }
    
    private:
        value_type obj;
        std::size_t length;
    };

    typedef ::sqlite3_context *app_defined_func_context_type;
    typedef int db_table_column_index_type;
    
    // use at `detail/binder_impl.hpp'
    class noexcept_tag {};
    class except_tag {};
} }

#endif // CAPRICE_SQLITEXX_DEFTYPES_HPP
