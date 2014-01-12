
#ifndef CAPRICE_SQLITEXX_DETAIL_CONNECTION_HPP
#define CAPRICE_SQLITEXX_DETAIL_CONNECTION_HPP

namespace caprice { namespace sqlitexx {

bool connection::open(
    const sql_string& absolute_path_to_db_file,
    boost::system::error_code& ec
 ) noexcept {
    if(!opened) {
        sql_string absolute_path(absolute_path_to_db_file);
        boost::algorithm::replace_all(absolute_path, _sqlSTR("\\"), _sqlSTR("/"));
        
        sql_string file_uri(_sqlSTR("file://"));
        file_uri += absolute_path;
        
        const int flag = SQLITE_OPEN_READWRITE |
                         SQLITE_OPEN_CREATE    |
                         SQLITE_OPEN_URI       |
                         SQLITE_OPEN_FULLMUTEX ;
        
        raw_db_object_type *raw_db;
        
        if((opened = check_result(::sqlite3_open_v2(file_uri.c_str(), &raw_db, flag, nullptr)))) {
            db = db_object_type(raw_db, &db_object_deleter);
        }
    }
    
    return opened;
}
    
} }

#endif // CAPRICE_SQLITEXX_DETAIL_CONNECTION_HPP
