#include <unity.h>
#include <EspNowRcLink/Protocol.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

using namespace EspNowRcLink;

void test_encode_aux()
{
    TEST_ASSERT_EQUAL_INT8(-124, MessageRc::encodeAux( 800));
    TEST_ASSERT_EQUAL_INT8(-124, MessageRc::encodeAux( 880));
    TEST_ASSERT_EQUAL_INT8(-100, MessageRc::encodeAux(1000));
    TEST_ASSERT_EQUAL_INT8( -80, MessageRc::encodeAux(1100));
    TEST_ASSERT_EQUAL_INT8( -50, MessageRc::encodeAux(1250));
    TEST_ASSERT_EQUAL_INT8(  -1, MessageRc::encodeAux(1497));
    TEST_ASSERT_EQUAL_INT8(   0, MessageRc::encodeAux(1498));
    TEST_ASSERT_EQUAL_INT8(   0, MessageRc::encodeAux(1499));
    TEST_ASSERT_EQUAL_INT8(   0, MessageRc::encodeAux(1500));
    TEST_ASSERT_EQUAL_INT8(   0, MessageRc::encodeAux(1501));
    TEST_ASSERT_EQUAL_INT8(   0, MessageRc::encodeAux(1502));
    TEST_ASSERT_EQUAL_INT8(   1, MessageRc::encodeAux(1503));
    TEST_ASSERT_EQUAL_INT8(  50, MessageRc::encodeAux(1750));
    TEST_ASSERT_EQUAL_INT8(  80, MessageRc::encodeAux(1900));
    TEST_ASSERT_EQUAL_INT8( 100, MessageRc::encodeAux(2000));
    TEST_ASSERT_EQUAL_INT8( 124, MessageRc::encodeAux(2120));
    TEST_ASSERT_EQUAL_INT8( 124, MessageRc::encodeAux(2200));
}

void test_decode_aux()
{
    TEST_ASSERT_EQUAL_UINT16( 880, MessageRc::decodeAux(-127));
    TEST_ASSERT_EQUAL_UINT16( 880, MessageRc::decodeAux(-124));
    TEST_ASSERT_EQUAL_UINT16(1000, MessageRc::decodeAux(-100));
    TEST_ASSERT_EQUAL_UINT16(1250, MessageRc::decodeAux( -50));
    TEST_ASSERT_EQUAL_UINT16(1495, MessageRc::decodeAux(  -1));
    TEST_ASSERT_EQUAL_UINT16(1500, MessageRc::decodeAux(   0));
    TEST_ASSERT_EQUAL_UINT16(1505, MessageRc::decodeAux(   1));
    TEST_ASSERT_EQUAL_UINT16(1750, MessageRc::decodeAux(  50));
    TEST_ASSERT_EQUAL_UINT16(1900, MessageRc::decodeAux(  80));
    TEST_ASSERT_EQUAL_UINT16(1950, MessageRc::decodeAux(  90));
    TEST_ASSERT_EQUAL_UINT16(2000, MessageRc::decodeAux( 100));
    TEST_ASSERT_EQUAL_UINT16(2100, MessageRc::decodeAux( 120));
    TEST_ASSERT_EQUAL_UINT16(2120, MessageRc::decodeAux( 124));
    TEST_ASSERT_EQUAL_UINT16(2120, MessageRc::decodeAux( 127));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_encode_aux);
    RUN_TEST(test_decode_aux);

    UNITY_END();

    return 0;
}
