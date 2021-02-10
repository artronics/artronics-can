#include "gtest/gtest.h"
#include "hal/hal_can.h"

using namespace testing;

class HalCanTest : public ::testing::Test {
public:
    ~HalCanTest() override = default;
};

TEST_F(HalCanTest, HalCan_init__should_init_CAN) {

}
