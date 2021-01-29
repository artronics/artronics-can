#ifndef ARTRONICS_TEST_UTILS_H
#define ARTRONICS_TEST_UTILS_H

#include "gmock/gmock.h"


namespace test_utils {
    namespace {
        template<typename T>
        constexpr auto begin(const T &item) {
          return reinterpret_cast<const char *>(&item);
        }

        template<typename T>
        constexpr auto end(const T &item) {
          return reinterpret_cast<const char *>(&item) + sizeof(T);
        }

        template<typename T>
        auto bitwise_equal(const T &lhs, const T &rhs) {
          return std::equal(begin(lhs), end(lhs), // will become constexpr with C++20
                            begin(rhs), end(rhs));
        }
    }

    namespace matchers {
        MATCHER_P2(RingBufferInit_matcher, num_elem, size_elem, "Check element's size and number of elements") {
          return (arg->num_elem == num_elem) && (arg->size_elem == size_elem);
        }

        MATCHER_P(BitwiseStructEq, value, "Check equality of CanFrame") {
          return (bitwise_equal<decltype(value)>(value, *arg));
        }
    }
}
#endif //ARTRONICS_TEST_UTILS_H
