
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
    /** @brief 
     *  @param parent
     *  @param stmt
     *  @param ec[out]
     */
    binder_impl(binder& parent, sql_statement_type stmt, boost::error_code& ec)
        : parent_(parent), stmt_(stmt), cause(ec) noexcept {}
    
    /// @brief destructor.
    ~binder_impl() {}
    
    /** @brief bind a value with a parameter.
     *  @param value[in] value of the parameter.
     */
    template <typename Arg, typename ...Args>
    void operator()(const Arg value,
                    const Args... v) noexcept {
        (*this)[value];
        return (*this)(v...);
    }
    
    /** @brief bind a value with a parameter.
     *  @param value[in] value of the parameter
     */
    template <typename Arg>
    void operator()(const Arg value) noexcept {
        (*this)[value];
    }
    
    /** @brief bind a value with a named-parameter.
     *  @param parameter_name parameter name.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    binder& operator[](const u8_string& parameter_name, T val) noexcept {
        param_idx = ::sqlite3_bind_parameter_index(stmt, parameter_name.c_str());
        return (*this)[val];
    }
    
    /** @brief bind a value with a named-parameter.
     *  @param parameter_name parameter name.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    binder& operator[](const int parameter_id, T val) noexcept {
        return (*this)[std::to_string(parameter_id), val];
    }
    
    /** @brief
     *  @param val[int]
     */
    template <typename T>
    binder& operator[](T val) noexcept {
        bind(param_idx, val);
        ++param_idx;
        return (*this);
    }
    
    /// @brief
    void end() noexcept {}

private:
    template <typename T>
    void bind(const int parameter_idx, T val, boost::error_code& ec) noexcept {
        if(!check_result(do_bind(parameter_idx))) {
            // ec...
        }
    }

    boost::error_code& cause;
    binder& parent_;
    sql_statement_type stmt_;
    int param_idx;
};

template <>
class binder_impl<except_tag> final {
public:
    /// @brief 
    binder_impl(binder& parent, sql_statement_type stmt)
        : parent_(parent), stmt_(stmt) {}
    /// @brief
    ~binder_impl() {}
    
    /** @brief bind a value with a parameter.
     *  @param value[in] value of the parameter.
     */
    template <typename Arg, typename ...Args>
    void operator()(const Arg value, const Args... v) {
        (*this)[value];
        return (*this)(v...);
    }
    
    /** @brief bind a value with a parameter.
     *  @param value[in] value of the parameter.
     */
    template <typename Arg>
    void operator()(const Arg value) {
        (*this)[value];
    }
    
    /** @brief bind a value with a named-parameter.
     *  @tparam T
     *  @param parameter_name parameter name.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    binder& operator[](const u8_string& parameter_name, T val) {
        param_idx = ::sqlite3_bind_parameter_index(stmt, parameter_name.c_str());
        return (*this)[val];
    }
    
    /** @brief bind a value with a named-parameter.
     *  @tparam T
     *  @param parameter_id parameter id, it's expressed by integer-type.
     *  @param val[in] value of the named-parameter.
     */
    template <typename T>
    binder& operator[](const int parameter_id, T val) {
        auto result = (*this)[std::to_string(parameter_id), val];
        return result;
    }
    
    /** @brief
     *  @tparam T
     *  @param val
     */
    template <typename T>
    binder& operator[](T val) {
        bind(param_idx, val);
        ++param_idx;
        return (*this);
    }
    
    /// @brief
    void end() noexcept {}
    
private:
    template <typename T>
    void bind(const int parameter_idx, T val) {
        if(!check_result(do_bind(parameter_idx))) {
            // ec...
        }
    }
    
    binder& parent_;
    sql_statement_type stmt_;
    int param_idx;
};


typedef binder_impl<noexcept_tag> noexcept_binder;
typedef binder_impl<except_tag> except_binder;

} }

#endif // CAPRICE_SQLITEXX_DETAIL_BINDER_IMPL_HPP


