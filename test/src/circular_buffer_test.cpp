#include "gtest/gtest.h"
#include "CircularBuffer.h"

TEST(CircularBufferTest, TestEmpty) {
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    Deriv::Holder h{};
    h.payload = "s1";
    h.isSuccess = true;
    h.timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    Deriv::CircularBuffer cb;
    cb.put(h);

    Deriv::Holder result = cb.peek();
    ASSERT_FALSE(result.isSuccess);
}

TEST(CircularBufferTest, Test60Iterates) {
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    Deriv::CircularBuffer cb;

    for (int i=0; i<60; i++) {
        Deriv::Holder h{};
        h.payload = "s"+std::to_string(i);
        h.isSuccess = true;
        h.timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()+60;
        cb.put(h);
    }

    for (int i=0; i<60; i++) {
        Deriv::Holder result = cb.peek();
        std::string expectedPayLoad = "s"+std::to_string(i);
        ASSERT_EQ(result.payload,expectedPayLoad);
        ASSERT_TRUE(result.isSuccess);
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

