
#ifndef CAPRICE_SQLITEXX_EXCEPTION_HPP
#define CAPRICE_SQLITEXX_EXCEPTION_HPP

#include <system_error>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "deftypes.hpp"

namespace caprice { namespace sqlitexx {

class sqlite3_error_category : public std::error_category {
    const char* name() const noexcept override { return "sqlite3"; }

    std::string message(int ev) const override { return ::sqlite3_errmsg(ev); }
    
    std::error_condition default_error_condition(int ev) const noexcept override {
        switch (ev) {
            case SQLITE_OK:
                return std::error_condition();
            case SQLITE_ABORT:
                return std::error_condition(std::errc::operation_canceled);
            case SQLITE_BUSY:
                return std::error_condition(std::errc::device_or_resource_busy);
            case SQLITE_NOMEM:
                return std::error_condition(std::errc::not_enough_memory);
            case SQLITE_INTERRUPT:
                return std::error_condition(std::errc::operation_canceled);
            case SQLITE_IOERR:
                return std::error_condition(std::errc::io_error);
            case SQLITE_FULL:
                return std::error_condition(std::errc::no_space_on_device);
            
            default:
                return std::error_condition(ev, sqlite3_error_category());
        }
    }
};

const std::error_category& sqlite3_error_category() {
    static thread_local const sqlite3_error_category category;
    return category;
}

bool check_result(const int result) noexcept {
    return (result == SQLITE_OK);
}
    
bool throw_error_code(const bool result, const boost::system::error_code& ec) {
    if (!result) { throw ec; }
    return result;
}
    
} }

#endif // CAPRICE_SQLITEXX_EXCEPTION_HPP
