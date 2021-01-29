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

    static const RingBufferHandler _txh = 12442;
};

CanFrame create_a_frame(MessageId messageId) {
  return {
          .id = messageId,
          .data = {0xAA, 0xBB},
          .is_remote = false,
          .is_extended = false,
  };
}

TEST_F(MessageProcessorTest, MessageProcessor_init__should_init_dependencies) {
  EXPECT_CALL(*_halModuleIdMock, HalModuleId_init());

  MessageProcessor_init(_txh);
}

TEST_F(MessageProcessorTest, MessageProcessor_process__should_process_ModuleId) {
  MessageProcessor_init(_txh);
  CanFrame moduleIdFrame = create_a_frame(ModuleId);

  EXPECT_CALL(*_halModuleIdMock, HalModuleId_get64BitId());

  MessageProcessor_process(&moduleIdFrame);
}

TEST_F(MessageProcessorTest, MessageProcessor_process__should_NOT_process_ModuleId) {
  MessageProcessor_init(_txh);
  CanFrame moduleIdFrame = create_a_frame(Unknown);

  EXPECT_CALL(*_halModuleIdMock, HalModuleId_get64BitId()).Times(0);

  MessageProcessor_process(&moduleIdFrame);
}
