#ifndef ARTRONICS_TEST_UTILS_H
#define ARTRONICS_TEST_UTILS_H

#include "gmock/gmock.h"

MATCHER_P(StructEq, value, "Check deref struct equality (must override ==)") {return (*arg == value);}

namespace test_utils {
    namespace matchers {
        MATCHER_P2(RingBufferInit_matcher, num_elem, size_elem, "kir") {return (arg->num_elem == num_elem) && (arg->size_elem == size_elem);}
    }
}
#endif //ARTRONICS_TEST_UTILS_H
