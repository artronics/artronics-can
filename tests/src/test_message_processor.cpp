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
    constexpr static const uint8_t _module64BitId[8] = {0x1A, 0x43, 0x12, 0};
};

CanFrame createRxFrame(MessageId messageId, const uint8_t * const data, const size_t data_size = CAN_FRAME_MAX_DATA_LEN) {
  CanFrame f = {
          .data = {0},
          .data_size = CAN_FRAME_MAX_DATA_LEN,
          .id = messageId,
          .is_remote = false,
          .is_extended = false,
  };
  for(auto i = 0; i < data_size; i++) {
    f.data[i] = data[i];
  }

  return f;
}

TEST_F(MessageProcessorTest, MessageProcessor_init__should_init_dependencies) {
  EXPECT_CALL(*_halModuleIdMock, HalModuleId_init());

  // When
  MessageProcessor_init(_txh);
}

TEST_F(MessageProcessorTest, MessageProcessor_process__should_process_ModuleId) {
  // Given
  MessageProcessor_init(_txh);
  const CanFrame expTxFrame = createRxFrame(ModuleId, _module64BitId);

  EXPECT_CALL(*_halModuleIdMock, HalModuleId_get64BitId())
          .Times(1)
          .WillOnce(Return(_module64BitId));
  EXPECT_CALL(*_ringBufferMock, RingBuffer_put(_txh, matchers::BitwiseVoidStructEq(expTxFrame)));

  // When
  MessageProcessor_process(&expTxFrame);
}

TEST_F(MessageProcessorTest, MessageProcessor_process__should_NOT_process_ModuleId) {
  // Given
  MessageProcessor_init(_txh);
  CanFrame moduleIdFrame = createRxFrame(Unknown, _module64BitId);

  EXPECT_CALL(*_halModuleIdMock, HalModuleId_get64BitId()).Times(0);

  // When
  MessageProcessor_process(&moduleIdFrame);
}
