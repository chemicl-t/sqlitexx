#ifndef CAPRICE_SQLITEXX_RESULT_SET_HPP
#define CAPRICE_SQLITEXX_RESULT_SET_HPP

#include <vector>
#include <stdexcept>

#include <boost/fusion/container.hpp>

#include "deftypes.hpp"

namespace caprice { namespace sqlitexx {

/** @brief
 *  @tparam ColumnTypes
 *  @note result-set is immutable.
 */
template <typename ...ColumnTypes>
class result_set_impl {
public:
    typedef result_set_impl<ColumnTypes...> self_type;
    
    typedef boost::fusion::vector<ColumnTypes...> row_type;
    
    typedef std::vector<row_type> result_set_object_type;
    typedef std::size_t size_type;
    
    template <int ColumnIdx>
    using value_type
        = typename std::tuple_element<ColumnIdx, std::tuple<ColumnTypes...>>::type;
    
    template <int RowIdx, int ColumnIdx>
    using element_type = value_type<ColumnIdx>;
    
    template <int ColumnIdx>
    using column_type = std::vector<value_type<ColumnIdx>>;
    
public:
    #include "detail/column-iterator.hpp"
    
    /// @brief constructor.
    explicit result_set_impl(const result_set_object_type& val) : container(val) {}

    /// @brief destructor.
    ~result_set_impl() {}
    
    /** @brief
     *  @param row_idx
     *  @tparam ColumnIdx
     */
    template <int ColumnIdx>
    value_type<ColumnIdx> at(int row_idx) const {
        static_assert(ColumnIdx < column_size(), "`ColumnIdx' is out of range.");
        return boost::fusion::at_c<ColumnIdx>(container.at(row_idx));
    }
    
    /** @brief
     *  @param row_idx
     *  @tparam ColumnIdx
     */
    template <int ColumnIdx>
    value_type<ColumnIdx> operator[](int row_idx) const noexcept {
        static_assert(ColumnIdx < column_size(), "`ColumnIdx' is out of range.");
        return boost::fusion::at_c<ColumnIdx>(container[row_idx]);
    }
    
    /** @brief
     *  @tparam RowIdx
     */
    template <int RowIdx>
    row_type row() const {
        row_type ret = container[RowIdx];
        return ret;
    }
    
    /** @brief
     *  @tparam ColumnIdx
     */
    template <int ColumnIdx>
    column_type<ColumnIdx> column() const {
        return column_type<ColumnIdx>(column_begin(), column_end());
    }
    
    /** @brief
     *  @tparam ColumnIdx
     */
    template <int ColumnIdx>
    column_iterator<ColumnIdx> column_begin() const {
        return column_iterator<ColumnIdx>(container.begin());
    }
    
    /** @brief
     *  @tparam ColumnIdx
     */
    template <int ColumnIdx>
    column_iterator<ColumnIdx> column_end() const {
        return column_iterator<ColumnIdx>(container.end());
    }
    
    /// @brief
    size_type row_size() const { return container.size(); }
    
    /// @brief
    constexpr size_type column_size() const { return sizeof...(ColumnTypes) + 1; }
    
    /// @brief
    bool empty() const { return container.empty(); }
    
private:
    result_set_object_type container;
};

template <typename ...T>
class result_set_impl<void, T...> {
public:
    typedef void self_type;
};

template <>
class result_set_impl<> {
public:
    typedef void self_type;
};

template <typename ...T>
using result_set = typename result_set_impl<T...>::self_type;

template <typename ...T>
using maybe_result_set = maybe<result_set<T...>> ;
    
template <typename T>
constexpr bool is_void() { return std::is_same<void, T>::value; }
    
template <typename ...T>
using enable_if_maybe_result_set_is_void
    = typename std::enable_if<is_void<maybe_result_set<T...>>()>::type;

template <typename ...T>
using disable_if_maybe_result_set_is_void
    = typename std::enable_if<!is_void<maybe_result_set<T...>>()>::type;

} }

#endif // CAPRICE_SQLITEXX_RESULT_SET_HPP
