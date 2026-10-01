#include <unity.h>
#include <EspNowRcLink/Receiver.h>
#include <EspNowRcLink/Transmitter.h>

using namespace EspNowRcLink;

void setUp(void) {}
void tearDown(void) {}

void test_checksum()
{
    const uint8_t data[] = {0x01, 0x02, 0x04};
    TEST_ASSERT_EQUAL_HEX8(0x55 ^ 0x01 ^ 0x02 ^ 0x04, checksum(data, sizeof(data)));
    TEST_ASSERT_EQUAL_HEX8(0x55, checksum(data, 0));
}

void test_receiver_stub_begin_fails()
{
    Receiver rx;
    TEST_ASSERT_EQUAL_INT(0, rx.begin());
    TEST_ASSERT_EQUAL_INT(0, rx.begin(true));
    rx.end();
}

void test_receiver_defaults()
{
    Receiver rx;
    TEST_ASSERT_EQUAL_INT(0, rx.available());
    TEST_ASSERT_EQUAL_INT16(1500, rx.getChannel(0));
    TEST_ASSERT_EQUAL_INT16(1000, rx.getChannel(2));
    TEST_ASSERT_EQUAL_INT16(1500, rx.getChannel(4));
    TEST_ASSERT_EQUAL_INT16(0, rx.getChannel(8));
}

void test_transmitter_stub_begin_fails()
{
    Transmitter tx;
    TEST_ASSERT_EQUAL_INT(0, tx.begin());
    TEST_ASSERT_EQUAL_INT(0, tx.begin(true));
    tx.end();
}

void test_transmitter_state()
{
    Transmitter tx;
    TEST_ASSERT_EQUAL(Transmitter::DISCOVERING, tx.getState());
    tx.setChannel(0, 1200);
    tx.commit();
    tx.update();
    TEST_ASSERT_EQUAL(Transmitter::DISCOVERING, tx.getState());
    TEST_ASSERT_EQUAL_INT(0, tx.getSensor(0));
    TEST_ASSERT_EQUAL_INT(-1, tx.getSensor(4));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_checksum);
    RUN_TEST(test_receiver_stub_begin_fails);
    RUN_TEST(test_receiver_defaults);
    RUN_TEST(test_transmitter_stub_begin_fails);
    RUN_TEST(test_transmitter_state);

    return UNITY_END();
}
