
#ifndef CAPRICE_SQLITEXX_DETAIL_BINDER_IMPL_HPP
#define CAPRICE_SQLITEXX_DETAIL_BINDER_IMPL_HPP

#include "deftypes.hpp"
#include "bind-helper.hpp"

namespace caprice { namespace sqlitexx {

/// @internal

/// @todo broken English...

/** @brief you can't instantinate this class. Because the template-paramater `T' is
 *         so called `tag'. In other word, `T' is passed only two tags
 *         (noexcept_tag or except_tag).
 */
template <typename T>
class binder_impl {
    binder_impl() = delete;
    ~binder_impl() = delete;
};
/// @endinternal

template <>
class binder_impl<noexcept_tag> final {
public:
    typedef binder_impl<noexcept_tag> self_type;
    
    /** @brief
     *  @param parent
     *  @param stmt
     *  @param ec[out]
     */
    binder_impl(sql_statement_type stmt, boost::system::error_code& ec) noexcept
        : stmt_(stmt), cause(ec)  {}
    
    /// @brief destructor.
    ~binder_impl() {}
    
    /** @brief bind a value with a named-parameter.
     *  @param parameter_name parameter name.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    self_type& operator()(const u8_string& parameter_name, T val) noexcept {
        auto param_idx = ::sqlite3_bind_parameter_index(stmt_.get(), parameter_name.c_str());
        bind(param_idx, val);
        return (*this);
    }
    
    /** @brief bind a value with a named-parameter.
     *  @param parameter_name parameter name.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    self_type& operator()(const int parameter_id, T val) noexcept {
        return (*this)(std::to_string(parameter_id), val);
    }
    
    /// @brief
    void end() noexcept {}

private:
    template <typename T>
    void bind(const int parameter_idx, T val) noexcept {
        auto bound_func = do_bind(stmt_, val);
        if(!check_result(bound_func(parameter_idx))) {
            // ec...
        }
    }

    boost::system::error_code& cause;
    sql_statement_type stmt_;
};

template <>
class binder_impl<except_tag> final {
public:
    typedef binder_impl<except_tag> self_type;
    
    /// @brief 
    binder_impl(sql_statement_type stmt) : stmt_(stmt) {}
    /// @brief
    ~binder_impl() {}
    
    /** @brief bind a value with a named-parameter.
     *  @tparam T
     *  @param parameter_name parameter name.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    self_type& operator()(const u8_string& parameter_name, T val) {
        auto param_idx = ::sqlite3_bind_parameter_index(stmt_.get(), parameter_name.c_str());
        bind(param_idx, val);
        return (*this);
    }
    
    /** @brief bind a value with a named-parameter.
     *  @tparam T
     *  @param parameter_id parameter id, it's expressed by integer-type.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    self_type& operator()(const int parameter_id, T val) {
        return (*this)(std::to_string(parameter_id), val);
    }
    
    /// @brief
    void end() noexcept {}
    
private:
    template <typename T>
    void bind(const int parameter_idx, T val) {
        auto bound_func = do_bind(stmt_, val);
        if(!check_result(bound_func(parameter_idx))) {
            // ec...
        }
    }
    
    void bind(const int parameter_idx, const sql_char *val) {
        bind(parameter_idx, sql_string(val));
    }
    
    sql_statement_type stmt_;
};


typedef binder_impl<noexcept_tag> noexcept_binder;
typedef binder_impl<except_tag> except_binder;

} }

#endif // CAPRICE_SQLITEXX_DETAIL_BINDER_IMPL_HPP


