// this file is included by "result-set.hpp". you mustn't do it.

#ifndef CAPRICE_SQLITEXX_COLUMN_ITERATOR_HPP
#define CAPRICE_SQLITEXX_COLUMN_ITERATOR_HPP

/** @brief column_iterator is imutable. 'operator*()' return value, not reference.
 *  @tparam ColumnIdx
 */
template <int ColumnIdx>
class column_iterator {
    /** @brief
     *  @note
     */
    explicit
    column_iterator(typename result_set_object_type::iterator& set_itr)
        : itr(set_itr) {}
    
public:
    /// @brief define `value_type' which indicates result_set
    typedef result_set_impl::value_type<ColumnIdx> value_type;
    /// @brief define `self_type'.
    typedef column_iterator<ColumnIdx> self_type;
    /// @brief default constructor is deleted.
    column_iterator() = delete;
    /// @brief copy constructor.
    column_iterator(const self_type& other) { itr = other.itr; }
    /// @brief destructor.
    ~column_iterator() {}
    
    /// @brief
    self_type& operator++() {
        ++itr;
        return *this;
    }
    
    /// @brief
    self_type operator++(int) {
        self_type tmp = *this;
        ++itr;
        return tmp;
    }
    
    //// @brief
    self_type& operator--() {
        --itr;
        return *this;
    }
    
    /// @brief
    self_type operator--(int) {
        self_type tmp = *this;
        --itr;
        return tmp;
    }
    
    /// @brief
    value_type operator*() const { return boost::fusion::at_c<ColumnIdx>(*itr); }
    
    /// @brief
    bool operator>(const self_type& other) const { return itr > other.itr; }
    
    /// @brief
    bool operator<(const self_type& other) const { return itr < other.itr; }
    
    /// @brief
    bool operator==(const self_type& other) const { return itr == other.itr; }
    
    /// @brief
    bool operator!=(const self_type& other) const { return itr != other.itr; }
    
private:
    typename result_set_object_type::iterator itr;
};

#endif // CAPRICE_SQLITEXX_COLUMN_ITERATOR_HPP
