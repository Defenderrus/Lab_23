#ifndef TEST_HPP
#define TEST_HPP

#include "../SegmentFunction.hpp"
#include "unity.h"
using namespace std;


void setUp(void) {}

void tearDown(void) {}

inline void test1(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.5, [](double x) {return x;});
    segFunc.Define(1.5, 2.75, [](double x) {return x*x-3*x+3.75;});
    segFunc.Define(2.75, 3.5, [](double x) {return sqrt(x+6.63);});

    TEST_ASSERT_EQUAL_DOUBLE(1.0, segFunc.CalculateAt(1.0));
    TEST_ASSERT_EQUAL_DOUBLE(1.75, segFunc.CalculateAt(2.0));
    TEST_ASSERT_EQUAL_DOUBLE(3.1, segFunc.CalculateAt(3.0));

    TEST_ASSERT_FALSE(segFunc.IsContinuous());
    TEST_ASSERT_TRUE(segFunc.IsMonotonic());
}

inline void test2(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return -x+2;});
    segFunc.Define(2.0, 3.0, [](double x) {return x-2;});

    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc.CalculateAt(0.5));
    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc.CalculateAt(1.5));
    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc.CalculateAt(2.5));

    TEST_ASSERT_TRUE(segFunc.IsContinuous());
    TEST_ASSERT_FALSE(segFunc.IsMonotonic());
}

inline void test3(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return x*x;});
    segFunc.Define(2.0, 3.0, [](double x) {return 4;});

    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc.CalculateAt(0.5));
    TEST_ASSERT_EQUAL_DOUBLE(2.25, segFunc.CalculateAt(1.5));
    TEST_ASSERT_EQUAL_DOUBLE(4.0, segFunc.CalculateAt(2.5));

    TEST_ASSERT_TRUE(segFunc.IsContinuous());
    TEST_ASSERT_TRUE(segFunc.IsMonotonic());
}

inline void test4(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return x*x;});
    segFunc.Define(2.0, 3.0, [](double x) {return 4;});
    segFunc.Define(0.5, 2.5, [](double x) {return 3*x*x;});

    TEST_ASSERT_EQUAL_DOUBLE(0.49, segFunc.CalculateAt(0.49));
    TEST_ASSERT_EQUAL_DOUBLE(0.78, segFunc.CalculateAt(0.51));
    TEST_ASSERT_EQUAL_DOUBLE(18.6, segFunc.CalculateAt(2.49));
    TEST_ASSERT_EQUAL_DOUBLE(4, segFunc.CalculateAt(2.51));

    TEST_ASSERT_FALSE(segFunc.IsContinuous());
    TEST_ASSERT_FALSE(segFunc.IsMonotonic());
}

inline void test5(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return 1;});
    segFunc.Define(1.0, 2.0, [](double x) {return x;});
    TEST_ASSERT_TRUE(segFunc.IsContinuous());
    TEST_ASSERT_TRUE(segFunc.IsMonotonic());
}

inline void test6(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(3.0, 4.0, [](double x) {return 1/(x-3)+1;});
    segFunc.Define(2.0, 3.0, [](double x) {return x;});
    TEST_ASSERT_EQUAL(2, segFunc.GetSize());
}

inline int run_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);
    return UNITY_END();
}

#endif // TEST_HPP
