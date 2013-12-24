
#ifndef SQLITEXX_TESTING_UTILITY_HPP
#define SQLITEXX_TESTING_UTILITY_HPP

#include <assert>

#define TEST_LOCATION (__FILE__ ## " : " ## __LINE__ ## " @ " ## __func__)

#define TEST_ASSERT_WITH_MSG(stmt, msg) \
    msg ## " in " ## TEST_LOCATION

#define TEST_ASSERT(stmt) TEST_ASSERT_WITH_MSG(stmt, #stmt)

#define TEST_DbC_PRECOND(stmt) \
    TEST_ASSERT_WITH_MSG(stmt, "PRE-condition isn't satisfied.")

#define TEST_DbC_POSTCOND(stmt) \
    TEST_ASSERT_WITH_MSG(stmt, "POST-condition isn't satisfied.")

#endif // SQLITEXX_TESTING_UTILITY_HPP
