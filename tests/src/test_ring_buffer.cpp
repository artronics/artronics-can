#include "gtest/gtest.h"
#include "ring_buffer.h"

#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))
#define BUFFER_SIZE 8

class RingBufferTest : public ::testing::Test {
protected:
    void SetUp() override {

    }
    char _buf[BUFFER_SIZE] = {0};
    rb_attr_t _attr = {
            .size_elem=sizeof(_buf[0]),
            .num_elem=BUFFER_SIZE,
            .buffer=_buf,
    };
};

TEST_F(RingBufferTest, RingBufer_Put_and_Get_ring_behaviour) {
  // Given
  rbd_t rbd;
  RingBuffer_Init(&rbd, &_attr);

  // When; put until full
  char data = 'a';
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Put(rbd, &data);
    data++;
  }

  // Then; consume until empty
  char act_data;

  char exp_data = 'a';
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Get(rbd, &act_data);
    EXPECT_EQ(exp_data, act_data);
    exp_data++;
  }

  // When; put until full
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Put(rbd, &data);
    data++;
  }

  // Then; consume until empty
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    RingBuffer_Get(rbd, &act_data);
    EXPECT_EQ(exp_data, act_data);
    exp_data++;
  }
}

TEST_F(RingBufferTest, RingBufer_Init__buffer_size_should_be_power_of_two) {
  // Given
  rbd_t rbd;
  char buf[10] = {0};
  rb_attr_t attr = {
          .size_elem=sizeof(buf[0]),
          .num_elem=ARRAY_SIZE(buf),
          .buffer=buf,
  };

  // When
  int err = RingBuffer_Init(&rbd, &attr);

  // Then
  EXPECT_EQ(-1, err);
}

TEST_F(RingBufferTest, RingBufer_Put_fill_buffer) {
  // Given
  rbd_t rbd;
  RingBuffer_Init(&rbd, &_attr);

  // When
  char data = 'a';
  int no_err;
  int err;
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    no_err = RingBuffer_Put(rbd, &data);
  }
  err = RingBuffer_Put(rbd, &data);

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
  rbd_t rbd;
  test_data_t buf[2] = {0};
  rb_attr_t attr = {
          .size_elem=sizeof(buf[0]),
          .num_elem=ARRAY_SIZE(buf),
          .buffer=buf,
  };

  int err = RingBuffer_Init(&rbd, &attr);
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
  ASSERT_EQ(0, err);

  EXPECT_EQ(42, actData0.foo);
  EXPECT_EQ(42, actData0.bar);
  EXPECT_EQ(nullptr, actData0.baz);

  EXPECT_EQ(43, actData1.foo);
  EXPECT_EQ(43, actData1.bar);
  EXPECT_EQ(nullptr, actData1.baz);
}
