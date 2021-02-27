#include "gtest/gtest.h"
#include "test_utils.h"
#include "hal/hal_module_id.h"
#include "stm32f0xx_hal.h"

#include "fff.h"

DEFINE_FFF_GLOBALS

FAKE_VALUE_FUNC(uint32_t, HAL_GetUIDw0);
FAKE_VALUE_FUNC(uint32_t, HAL_GetUIDw1);

using namespace testing;
using namespace test_utils::stub;

class ModuleIdTest : public ::testing::Test {
protected:
    void SetUp() override {
      RESET_FAKE(HAL_GetUIDw0);
      RESET_FAKE(HAL_GetUIDw1);
      FFF_RESET_HISTORY();
    }
};

TEST_F(ModuleIdTest, HalModuleId_init__should_get_UID_after_init) {
  // When
  auto status = HalModuleId_init();

  // Then
  ASSERT_EQ(status, 0);
  ASSERT_EQ(HAL_GetUIDw0_fake.call_count, 1);
  ASSERT_EQ(HAL_GetUIDw1_fake.call_count, 1);
}

TEST_F(ModuleIdTest, HalModuleId_get64BitId__should_get_moduleId_64bit) {
  // Give
  HAL_GetUIDw0_fake.return_val = 0x11223344;
  HAL_GetUIDw1_fake.return_val = 0x55667788;

  HalModuleId_init();

  // When
  auto *id = HalModuleId_get64BitId();

  // Then
  ASSERT_EQ(id[0], 0x44);
  ASSERT_EQ(id[1], 0x33);
  ASSERT_EQ(id[2], 0x22);
  ASSERT_EQ(id[3], 0x11);

  ASSERT_EQ(id[4], 0x88);
  ASSERT_EQ(id[5], 0x77);
  ASSERT_EQ(id[6], 0x66);
  ASSERT_EQ(id[7], 0x55);
}
