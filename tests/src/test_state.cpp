#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "state.h"
#include "StateFixture.h"
#include "mocks/RingBufferMock.h"
#include "test_utils.h"

using namespace ::testing;
using ::testing::Return;
using namespace ::test_utils;

class StateTest : public StateFixture {
public:
    ~StateTest() override = default;

    StateTest() = default;
};

CanFrame rxFrame0 = {
        .id = 1,
        .data = {0xAA, 0xBB},
        .is_remote = false,
        .is_extended = false,
};

CanFrame rxFrame1 = {
        .id = 2,
        .data = {0xCC, 0xDD},
        .is_remote = false,
        .is_extended = false,
};

TEST_F(StateTest, State_Init__calls_RingBuffer_Init) {
  // Given
  RingBufferHandler *rbh = State_GetCanRxBufHandler();
  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_init());
  EXPECT_CALL(*_ringBufferMock,
              RingBuffer_init(rbh, matchers::RingBufferInit_matcher(STATE_CAN_RX_BUF_SIZE, sizeof(CanFrame))));

  // When
  State_init();
}

TEST_F(StateTest, State_Start__should_process_can_rx_frames_until_empty) {
  // Given
  RingBufferHandler *rbh = State_GetCanRxBufHandler();
  EXPECT_CALL(*_ringBufferMock, RingBuffer_get(*rbh, _))
          .Times(3)
          .WillOnce(Invoke(stub_RingBuffer_Get(rxFrame0, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(rxFrame1, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(rxFrame1, RING_BUFFER_ERROR)));

  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_process(StructEq(rxFrame0)));
  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_process(StructEq(rxFrame1)));

  State_init();

  // When
  State_start(false);
}
