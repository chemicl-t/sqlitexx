
#ifndef CAPRICE_SQLITEXX_DETAIL_CONNECTION_HPP
#define CAPRICE_SQLITEXX_DETAIL_CONNECTION_HPP

namespace caprice { namespace sqlitexx {

bool connection::open(const sql_string& absolute_path, boost::error_code& ec) noexcept {
    if(!opened) {
        boost::algorithm::replace_all(absolute_path, _sqlSTR("\\"), _sqlSTR("/"));
        const sql_string file_uri = _sqlSTR("file://") + absolute_path;
        
        const int flag = SQLITE3_OPEN_READWRITE |
                         SQLITE3_OPEN_CREATE    |
                         SQLITE3_OPEN_URI       |
                         SQLITE3_OPEN_FULLMUTEX ;
        
        raw_db_object_type *raw_db;
        
        if(opened = check_result(::sqlite3_open_v2(file_uri, &raw_db, flag, nullptr))) {
            db = db_object_type(raw_db, &db_object_deleter);
        }
    }
    
    return opened;
}
    
} }

#endif // CAPRICE_SQLITEXX_DETAIL_CONNECTION_HPP
