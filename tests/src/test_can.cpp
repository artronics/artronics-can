#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CanFixture.h"
#include "can.h"

using namespace testing;
using ::testing::AtLeast;

class CanTest : public ::CanFixture {
public:
    ~CanTest() override = default;

protected:
    static constexpr RingBufferHandler _canRxBufHandler = 12;
};


TEST_F(CanTest, Can_init__should_init_dependencies) {
  // Given
  HalCanInit actHalCanInit;
  EXPECT_CALL(*_halCanMock, HalCan_init(_))
          .WillOnce(Invoke(
                  [&actHalCanInit](auto *init) {
                      actHalCanInit = *init;
                      return 0;
                  }
          ));

  // When
  Can_init(1);

  // Then
  EXPECT_TRUE(actHalCanInit.receivedFrameCallback != nullptr);
}

TEST_F(CanTest, Can_transmit__should_send_tx_frame) {
  // Given
  const CanFrame *const frame = {};
  EXPECT_CALL(*_halCanMock, HalCan_transmit(frame));

  // When
  Can_transmit(frame);
}

TEST_F(CanTest, Can_receive_cb__should_put_received_frame_into_buffer) {
  // Given
  HalCanInit actHalCanInit;
  EXPECT_CALL(*_halCanMock, HalCan_init(_))
          .WillOnce(Invoke(
                  [&actHalCanInit](auto *init) {
                      actHalCanInit = *init;
                      return 0;
                  }
          ));

  Can_init(_canRxBufHandler);

  const CanFrame frame = {};
  EXPECT_CALL(*_ringBufferMock, RingBuffer_put(_canRxBufHandler, &frame));

  // When
  actHalCanInit.receivedFrameCallback(&frame);
}
