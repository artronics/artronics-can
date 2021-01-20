#include "gtest/gtest.h"
#include "ring_buffer.h"

#define BUFFER_SIZE 8

class RingBufferTest : public ::testing::Test {
protected:
    void SetUp() override {
      // FIXME: the default value for RING_BUFFER_MAX must be big enough to contain all the buffers we're creating during tests
      int err = RingBuffer_Init(&_rbd, &_attr);
      ASSERT_EQ(0, err);
    }

    RingBufferHandler _rbd;
    char _buf[BUFFER_SIZE] = {0};
    RingBufferInit _attr = {
            .size_elem=sizeof(_buf[0]),
            .num_elem=BUFFER_SIZE,
            .buffer=_buf,
    };
};

TEST_F(RingBufferTest, RingBufer_Put_and_Get_ring_behaviour) {
  // Given
  char data = 'a';

  // When; put until full
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Put(_rbd, &data);
    data++;
  }

  // Then; consume until empty
  char act_data;

  char exp_data = 'a';
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Get(_rbd, &act_data);
    EXPECT_EQ(exp_data, act_data);
    exp_data++;
  }

  // When; put until full
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Put(_rbd, &data);
    data++;
  }

  // Then; consume until empty
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Get(_rbd, &act_data);
    EXPECT_EQ(exp_data, act_data);
    exp_data++;
  }
}

TEST_F(RingBufferTest, RingBufer_Init__buffer_size_should_be_power_of_two) {
  // Given
  RingBufferHandler rbd;
  char buf[10] = {0};
  RingBufferInit attr = {
          .size_elem=sizeof(buf[0]),
          .num_elem=sizeof(buf)/sizeof(buf[0]),
          .buffer=buf,
  };

  // When
  int err = RingBuffer_Init(&rbd, &attr);

  // Then
  EXPECT_EQ(-1, err);
}

TEST_F(RingBufferTest, RingBufer_Put_fill_buffer) {
  // Given
  char data = 'a';

  // fill buffer
  int no_err;
  int err;
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    no_err = RingBuffer_Put(_rbd, &data);
  }

  // When
  err = RingBuffer_Put(_rbd, &data);

  // Then
  ASSERT_EQ(0, no_err);
  ASSERT_EQ(-1, err);
}

typedef struct test_data {
    int32_t foo;
    int32_t bar;
    int32_t *baz;
} test_data_t;

TEST_F(RingBufferTest, RingBufer_Put__struct) {
  // Given
  RingBufferHandler rbd;
  test_data_t buf[2] = {0};
  RingBufferInit attr = {
          .size_elem=sizeof(buf[0]),
          .num_elem=sizeof(buf)/sizeof(buf[0]),
          .buffer=buf,
  };

  int err = RingBuffer_Init(&rbd, &attr);
  ASSERT_EQ(0, err);

  test_data_t data0 = {.foo = 42, .bar=42, .baz = nullptr};
  test_data_t data1 = {.foo = 43, .bar=43, .baz = nullptr};

  // When
  RingBuffer_Put(rbd, &data0);
  RingBuffer_Put(rbd, &data1);

  test_data_t actData0;
  RingBuffer_Get(rbd, &actData0);
  test_data_t actData1;
  RingBuffer_Get(rbd, &actData1);

  // Then

  EXPECT_EQ(42, actData0.foo);
  EXPECT_EQ(42, actData0.bar);
  EXPECT_EQ(nullptr, actData0.baz);

  EXPECT_EQ(43, actData1.foo);
  EXPECT_EQ(43, actData1.bar);
  EXPECT_EQ(nullptr, actData1.baz);
}
