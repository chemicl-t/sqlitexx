#ifndef CAPRICE_SQLITEXX_RESULT_SET_HPP
#define CAPRICE_SQLITEXX_RESULT_SET_HPP

#include <vector>
#include <tuple>
#include <stdexcept>
#include <type_traits>

#include <boost/fusion/container.hpp>
#include <boost/optional.hpp>

#include "deftypes.hpp"

namespace caprice { namespace sqlitexx {

/** @brief
 *  @tparam ColumnType1
 *  @tparam ColumnTypes
 *  @note result-set is immutable.
 */
template <typename ColumnType, typename ...ColumnTypes>
class result_set {
public:
    typedef boost::fusion::vector<ColumnType, ColumnTypes...> row_type;
    typedef std::vector<row_type> result_set_object_type;
    typedef std::size_t size_type;
    
    template <int ColumnIdx>
    using value_type
        = std::tuple_element<ColumnIdx, std::tuple<ColumnType, ColumnTypes...>>::type;
    
    template <int RowIdx, int ColumnIdx>
    using element_type = value_type<ColumnIdx>;
    
    template <int ColumnIdx>
    using column_type = std::vector<value_type<ColumnIdx>>;
    
public:
    #include "column-iterator.hpp"
    
private:
    explicit result_set(const result_set_object_type& val) {}

public:
    /// destructor.
    ~result_set() {}
    
    /** @brief
     *  @tparam RowIdx
     *  @tparam ColumnIdx
     */
    template <int RowIdx, int ColumnIdx>
    value_type<ColumnIdx> at() const {
        static_assert(ColumnIdx < column_size(), "`ColumnIdx' is out of range.");
        return boost::fusion::at_c<ColumnIdx>(container.at(RowIdx));
    }
    
    /** @brief
     *  @tparam RowIdx
     *  @tparam ColumnIdx
     */
    template <int RowIdx, int ColumnIdx>
    value_type<ColumnIdx> operator[]() const noexcept {
        static_assert(ColumnIdx < column_size(), "`ColumnIdx' is out of range.");
        return boost::fusion::at_c<ColumnIdx>(container[RowIdx]);
    }
    
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
    constexpr size_type column_size() const { return sizeof(...ColumnTypes) + 1; }
    
    /// @brief
    bool empty() const { return container.empty(); }
    
private:
    result_set_object_type container;
};
    
template <>
using result_set<> = void;

template <>
using result_set<void> = void;

template <typename ...T>
using boost::optional<result_set<T...>> maybe_result_set;
    
} }

#endif // CAPRICE_SQLITEXX_RESULT_SET_HPP
