#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_utils.h"
#include "message_processor.h"
#include "MessageProcessorFixture.h"
#include "message.h"

using namespace ::testing;
using ::testing::Return;
using namespace ::test_utils;

class MessageProcessorTest : public MessageProcessorFixture {
public:
    ~MessageProcessorTest() override = default;

    MessageProcessorTest() = default;
};

static CanFrame *frame = nullptr;


TEST_F(MessageProcessorTest, should_init_dependencies) {
  // Given
  EXPECT_CALL(*_halModuleIdMock, HalModuleId_init());

  // When
  MessageProcessor_init();
}
