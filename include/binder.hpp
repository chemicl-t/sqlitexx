
#ifndef CAPRICE_SQLITEXX_BINDER_HPP
#define CAPRICE_SQLITEXX_BINDER_HPP

#include <cstdint>

#include "deftypes.hpp"
#include "string.hpp"

#include "compiled-statement.hpp"

#include "detail/bind-helper.hpp"
#include "detail/binder-impl.hpp"

namespace caprice { namespace sqlitexx {

class binder final {
public:
    explicit binder(compiled_statement& sql_stmt) : stmt(sql_stmt) {}
    
    ~binder() { reset(); }
    
    noexcept_binder begin(boost::system::error_code& ec) noexcept {
        return noexcept_binder(stmt.stmt, ec);
    }
    
    except_binder begin() {
        return except_binder(stmt.stmt);
    }
    
    void reset() noexcept { ::sqlite3_clear_bindings(stmt.stmt.get()); }

private:
    compiled_statement stmt;
};

} }
    
#endif // CAPRICE_SQLITEXX_BINDER_HPP
