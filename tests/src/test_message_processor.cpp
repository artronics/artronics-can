#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_utils.h"
#include "message_processor.h"
#include "message.h"

using namespace ::testing;
using ::testing::Return;
using namespace ::test_utils;

class MessageProcessorTest : public ::testing::Test {
public:
    ~MessageProcessorTest() override = default;

    MessageProcessorTest() = default;
};

static CanFrame *frame = nullptr;

void update_frame(MessageId messageId) {
  *frame = {
          .id = messageId,
          .data = {0xAA, 0xBB},
          .is_remote = false,
          .is_extended = false,
  };
}

TEST_F(MessageProcessorTest, First) {
  // Given
  update_frame(ModuleId);

  // When
  MessageProcessor_Process(frame);

  // Then

}
