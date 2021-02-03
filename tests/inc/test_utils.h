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

        MATCHER_P2(EqIf, predicate, value, "Check equality with a predicate.") {
          return predicate(value, *arg);
        }

        MATCHER_P(BitwiseStructEq, value, "Check bitwise equality of a struct") {
          return (bitwise_equal<decltype(value)>(value, *arg));
        }

        MATCHER_P(BitwiseVoidStructEq, value, "Cast void * to type of arg and check bitwise equality of the value") {
          auto *tmp = static_cast<decltype(value) *>(arg); // arg is void *. convert it first to type of value
          return (bitwise_equal<decltype(value)>(value, *tmp));
        }
    }
}
#endif //ARTRONICS_TEST_UTILS_H
