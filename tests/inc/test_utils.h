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
          return std::equal(begin(lhs), end(lhs),
                            begin(rhs), end(rhs));
        }

        // convert lambda with capture to function pointers
        // source: https://stackoverflow.com/a/48368508/3943054
        template <class F>
        struct lambda_traits : lambda_traits<decltype(&F::operator())>
        { };

        template <typename F, typename R, typename... Args>
        struct lambda_traits<R(F::*)(Args...)> : lambda_traits<R(F::*)(Args...) const>
        { };

        template <class F, class R, class... Args>
        struct lambda_traits<R(F::*)(Args...) const> {
            using pointer = typename std::add_pointer<R(Args...)>::type;

            static pointer cify(F&& f) {
              static F fn = std::forward<F>(f);
              return [](Args... args) {
                  return fn(std::forward<Args>(args)...);
              };
            }
        };
    }

    namespace stub {
        template <class F>
        inline typename lambda_traits<F>::pointer lambdaToFunPointer(F&& f) {
          return lambda_traits<F>::cify(std::forward<F>(f));
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
