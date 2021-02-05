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

protected:
    constexpr static const RingBufferHandler _rbTxH = 1;
    constexpr static const RingBufferHandler _rbRxH = 2;
    constexpr static const CanFrame _canFrame0 = {
            .data = {0xAA, 0xBB, 0xFF, 0},
            .data_size = 3,
            .id = 1,
            .is_remote = false,
            .is_extended = false,
    };

    constexpr static const CanFrame _canFrame1 = {
            .data = {0xCC, 0xDD},
            .data_size = 2,
            .id = 2,
            .is_remote = false,
            .is_extended = false,
    };

    static void setupInit() {
      EXPECT_CALL(*_ringBufferMock, RingBuffer_init(_, _))
              .WillOnce(Invoke(
                      [=](auto *rbh, auto *rbInit) {
                          *rbh = _rbTxH;
                          return 0;
                      }))
              .WillOnce(Invoke(
                      [=](auto *rbh, auto *rbInit) {
                          *rbh = _rbRxH;
                          return 0;
                      }));

      State_init();
    }
};

TEST_F(StateTest, State_init__should_init_dependencies) {
  // Given
  RingBufferInit actRbRxInit;
  const RingBufferInit expRbRxInit = {
          .size_elem = sizeof(CanFrame),
          .num_elem = STATE_CAN_RX_BUF_SIZE,
  };
  RingBufferInit actRbTxInit;
  const RingBufferInit expRbTxInit = {
          .size_elem = sizeof(CanFrame),
          .num_elem = STATE_CAN_TX_BUF_SIZE,
  };

  EXPECT_CALL(*_ringBufferMock, RingBuffer_init(_, _))
          .WillOnce(Invoke(
                  [=, &actRbTxInit](auto *rbh, auto *rbInit) {
                      *rbh = _rbTxH;
                      actRbTxInit = *rbInit;
                      return 0;
                  }))
          .WillOnce(Invoke(
                  [=, &actRbRxInit](auto *rbh, auto *rbInit) {
                      *rbh = _rbRxH;
                      actRbRxInit = *rbInit;
                      return 0;
                  }));

  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_init(_rbTxH));
  EXPECT_CALL(*_canMock, Can_init(_rbRxH));

  // When
  State_init();

  // Then
  auto ringBufferInitEq = [](const RingBufferInit l, const RingBufferInit r) {
      // TODO: This doesn't check the buffer pointer. Add it to eq
      return (l.num_elem == r.num_elem) && (l.size_elem == r.size_elem);
  };
  EXPECT_TRUE(ringBufferInitEq(actRbRxInit, expRbRxInit));
  EXPECT_TRUE(ringBufferInitEq(actRbTxInit, expRbTxInit));
}

TEST_F(StateTest, State_Start__should_process_can_rx_frames_until_empty) {
  // Given
  setupInit();

  EXPECT_CALL(*_ringBufferMock, RingBuffer_get(_rbTxH, _))
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame0, RING_BUFFER_ERROR)));
  EXPECT_CALL(*_ringBufferMock, RingBuffer_get(_rbRxH, _))
          .Times(3)
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame0, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame1, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame1, RING_BUFFER_ERROR)));

  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_process(matchers::BitwiseStructEq(_canFrame0)));
  EXPECT_CALL(*_messageProcessorMock, MessageProcessor_process(matchers::BitwiseStructEq(_canFrame1)));

  // When
  State_start(false);
}

TEST_F(StateTest, State_Start__should_process_can_tx_frames_until_empty) {
  // Given
  setupInit();

  EXPECT_CALL(*_ringBufferMock, RingBuffer_get(_rbRxH, _))
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame0, RING_BUFFER_ERROR)));
  EXPECT_CALL(*_ringBufferMock, RingBuffer_get(_rbTxH, _))
          .Times(3)
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame0, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame1, RING_BUFFER_OK)))
          .WillOnce(Invoke(stub_RingBuffer_Get(_canFrame1, RING_BUFFER_ERROR)));

  EXPECT_CALL(*_canMock, Can_transmit(matchers::BitwiseStructEq(_canFrame0)));
  EXPECT_CALL(*_canMock, Can_transmit(matchers::BitwiseStructEq(_canFrame1)));

  // When
  State_start(false);
}
