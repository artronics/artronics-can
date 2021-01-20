#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace testing;
using ::testing::AtLeast;
using ::testing::_;

class CanService
{
public:
    virtual ~CanService() = default;
//    MOCK_METHOD(HAL_StatusTypeDef ,HAL_CAN_Init, (CAN_HandleTypeDef *));
};


TEST (CanService, firstTest) {
  CanService can;
//  CAN_HandleTypeDef hcan;
//  EXPECT_CALL(can, HAL_CAN_Init(&hcan)).Times(AtLeast(1));

//  CAN_Init();


}
