
#ifndef CAPRICE_SQLITEXX_CONNECTION_HPP
#define CAPRICE_SQLITEXX_CONNECTION_HPP

#include <boost/algorithm/string/replace.hpp>

#include "deftypes.hpp"
#include "string.hpp"

namespace caprice { namespace sqlitexx {

class connection final {
public:
    /** @brief constructor. the ctor call @link open() @endlink automatically.
     *         if the file specified by `absolute_path' is not found, the file
     *         is newly created at this ctor.
     *  @param absolute_path[in] absolute path to a database file.
     *  @param ec[out]
     */
    connection(const sql_string& absolute_path,
               boost::error_code& ec) noexcept {
        open(absolute_path, ec);
    }

    /** @brief constructor. the ctor call @link open() @endlink automatically.
     *         if the file specified by `absolute_path' is not found, the file
     *         is newly created at this ctor.
     *  @param absolute_path[in] absolute path to a database file.
     *  @throw .
     */
    explicit connection(const sql_string& absolute_path) {
        boost::error_code ec;
        throw_error_code(open(absolute_path, ec), ec);
    }

    /** @brief default constructor. after, you need to call
     *         @link open(const sql_string&, const boost::error_code&) @endlink
     *         explicitly.
     */
    connection() : opened(false) noexcept {}

    /// @brief destructor. the dtor call @link close() @endlink automatically.
    ~connection() { close(); }

    /** @brief open the database file specified by `absolute_path'. if the file 
     *         is not found, this function create the file newly.
     *  @param absolute_path[in]
     *  @param ec[out]
     *  @retval true success to open the file.
     *          false fail to open the file.
     */
    bool open(const sql_string& absolute_path, boost::error_code& ec) noexcept;
    
    /** @brief this function is nearly same as 
     *         @link open(const sql_string&, const boost::error_code&) @endlink.
     *         the difference is designated `noexcept' or not. if you want more 
     *         information, see @link open(const sql_string&, boost::error_code&) @endlink.
     *  @param absolute_path[in]
     *  @param ec[out]
     *  @retval true success to open the file.
     *          false fail to open the file. this function throw more detailed information about the failure.
     */
    bool open(const sql_string& absolute_path) {
        boost::error_code ec;
        return opened = throw_error_code(open(absolute_path, ec), ec);
    }

    /** @brief close the database file. if the file has already closed,
     *         this function is the harmless no-operation.
     *  @retval true success to close the file.
     *          false fail to close the file.
     */
    bool close() noexcept {
        if(opened) { opened = check_result(::sqlite3_close_v2(db)); }
        return opened;
    }

    /** @brief checking the database file is opened or not.
     *  @retval true the file is opened.
     *          false the file is not opened. 
     */
    bool is_opened() noexcept const { return opened; }

    /// @brief return the text that discribes error.
    const sql_char *get_error_message() noexcept const {
        return ::sqlite3_errmsg(db);
    }

    /// @brief return database object.
    db_object_type get_db_object() noexcept { return db; }

private:
    db_object_type db;
    bool opened;
};

} }

#include "detail/connection.hpp"

#endif // CAPRICE_SQLITEXX_CONNECTION_HPP
