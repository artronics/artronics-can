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

    constexpr static const CanFrame _rxFrame0 = {
            .data = {0xAA, 0xBB, 0xFF, 0},
            .data_size = 3,
            .id = 1,
            .is_remote = false,
            .is_extended = false,
    };

    constexpr static const CanFrame _rxFrame1 = {
            .data = {0xCC, 0xDD},
            .data_size = 2,
            .id = 2,
            .is_remote = false,
            .is_extended = false,
    };
};

// TODO: This doesn't check the buffer pointer. Add it to eq
const static auto ringBufferInitEq = [] (const RingBufferInit l, const RingBufferInit r) {
    return (l.num_elem == r.num_elem) && (l.size_elem == r.size_elem);
};


TEST_F(StateTest, State_init__should_init_dependencies) {
  // Given
  RingBufferHandler *rxh = State_GetCanRxBufHandler();
  RingBufferHandler *txh = State_GetCanTxBufHandler();

  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_init(*txh));

  const RingBufferInit expRBRxInit = {
          .size_elem = sizeof(CanFrame),
          .num_elem = STATE_CAN_RX_BUF_SIZE,
  };
  const RingBufferInit expRBTxInit = {
          .size_elem = sizeof(CanFrame),
          .num_elem = STATE_CAN_TX_BUF_SIZE,
  };
  EXPECT_CALL(*_ringBufferMock,
              RingBuffer_init(rxh, matchers::EqIf(ringBufferInitEq, expRBRxInit)));
  EXPECT_CALL(*_ringBufferMock,
              RingBuffer_init(txh, matchers::EqIf(ringBufferInitEq, expRBTxInit)));

  // When
  State_init();
}

TEST_F(StateTest, State_Start__should_process_can_rx_frames_until_empty) {
  // Given
  RingBufferHandler *rbh = State_GetCanRxBufHandler();
  EXPECT_CALL(*_ringBufferMock, RingBuffer_get(*rbh, _))
          .Times(3)
          .WillOnce(Invoke(stub_RingBuffer_Get(_rxFrame0, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(_rxFrame1, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(_rxFrame1, RING_BUFFER_ERROR)));

  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_process(matchers::BitwiseStructEq(_rxFrame0)));
  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_process(matchers::BitwiseStructEq(_rxFrame1)));

  State_init();

  // When
  State_start(false);
}
