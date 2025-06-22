#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include "../SegmentFunction.hpp"
#include "unity.h"


void setUp(void) {}

void tearDown(void) {}

// Основные функции
void test1(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.5, [](double x) {return x;});
    segFunc.Define(1.5, 2.75, [](double x) {return x*x-3*x+3.75;});
    segFunc.Define(2.75, 3.5, [](double x) {return sqrt(x+6.63);});

    TEST_ASSERT_EQUAL_DOUBLE(1.0, segFunc(1.0));
    TEST_ASSERT_EQUAL_DOUBLE(1.75, segFunc(2.0));
    TEST_ASSERT_EQUAL_DOUBLE(3.1, segFunc(3.0));

    TEST_ASSERT_FALSE(segFunc.IsContinuous());
    TEST_ASSERT_TRUE(segFunc.IsMonotonic());

    try {cout << "Ошибки нет! f(x) = " << segFunc(1.5) << endl;}
    catch (const std::exception &e) {std::cout << e.what() << std::endl;}
    try {segFunc(2.75);}
    catch (const std::exception &e) {std::cout << e.what() << std::endl;}
    try {segFunc(-0.5);}
    catch (const std::exception &e) {std::cout << e.what() << std::endl;}
}

void test2(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return -x+2;});
    segFunc.Define(2.0, 3.0, [](double x) {return x-2;});

    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc(0.5));
    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc(1.5));
    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc(2.5));

    TEST_ASSERT_TRUE(segFunc.IsContinuous());
    TEST_ASSERT_FALSE(segFunc.IsMonotonic());
}

void test3(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return x*x;});
    segFunc.Define(2.0, 3.0, [](double x) {return 4;});

    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFunc(0.5));
    TEST_ASSERT_EQUAL_DOUBLE(2.25, segFunc(1.5));
    TEST_ASSERT_EQUAL_DOUBLE(4.0, segFunc(2.5));

    TEST_ASSERT_TRUE(segFunc.IsContinuous());
    TEST_ASSERT_TRUE(segFunc.IsMonotonic());
}

void test4(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return x*x;});
    segFunc.Define(2.0, 3.0, [](double x) {return 4;});
    segFunc.Define(0.5, 2.5, [](double x) {return 3*x*x;});

    TEST_ASSERT_EQUAL_DOUBLE(0.49, segFunc(0.49));
    TEST_ASSERT_EQUAL_DOUBLE(0.78, segFunc(0.51));
    TEST_ASSERT_EQUAL_DOUBLE(18.6, segFunc(2.49));
    TEST_ASSERT_EQUAL_DOUBLE(4, segFunc(2.51));

    TEST_ASSERT_FALSE(segFunc.IsContinuous());
    TEST_ASSERT_FALSE(segFunc.IsMonotonic());
}

void test5(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return 1;});
    segFunc.Define(1.0, 2.0, [](double x) {return x;});
    TEST_ASSERT_TRUE(segFunc.IsContinuous());
    TEST_ASSERT_TRUE(segFunc.IsMonotonic());
}

void test6(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(3.0, 4.0, [](double x) {return 1/(x-3)+1;});
    segFunc.Define(2.0, 2.9, [](double x) {return x;});
    TEST_ASSERT_EQUAL(2, segFunc.GetSize());
    TEST_ASSERT_EQUAL_DOUBLE(11, segFunc(3.1));

    try {std::cout << "Ошибки нет! f(x) = " << segFunc(3.0) << std::endl;}
    catch (const std::exception &e) {cout << e.what() << std::endl;}
}

// Дополнительные функции
void map_where_reduce(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(2.0, 3.0, [](double x) {return 4;});
    segFunc.Define(0.75, 2.25, [](double x) {return 3*x*x;});

    auto map = segFunc.Map<double>([](Segment<double> seg) {return Segment<double>(seg.start-1.0, seg.end-0.5, seg.func);});
    TEST_ASSERT_EQUAL_DOUBLE(-0.5, map(-0.5));
    TEST_ASSERT_EQUAL_DOUBLE(4, map(2.0));
    TEST_ASSERT_EQUAL_DOUBLE(3, map(1.0));

    auto where = segFunc.Where([](Segment<double> seg) {return seg.end-seg.start > 1;});
    TEST_ASSERT_EQUAL(1, where.GetSize());
    TEST_ASSERT_EQUAL_DOUBLE(12, where(2.0));

    auto reduce = segFunc.Reduce([](double x, Segment<double> seg) {return x+seg.func((seg.end-seg.start)/2);}, 0.0);
    TEST_ASSERT_EQUAL_DOUBLE(6.06, reduce);
}

void zip_unzip(void) {
    SegmentFunction<double> segFunc1;
    SegmentFunction<double> segFunc2;
    segFunc1.Define(0.0, 1.0, [](double x) {return 4;});
    segFunc1.Define(1.0, 2.0, [](double x) {return x*x;});
    segFunc2.Define(0.5, 1.5, [](double x) {return x;});
    segFunc2.Define(1.5, 2.5, [](double x) {return 3*x*x;});

    auto zip = segFunc1.Zip(segFunc2);
    TEST_ASSERT_EQUAL(3, zip.GetSize());
    TEST_ASSERT_EQUAL_DOUBLE(4, zip(0.75).first);
    TEST_ASSERT_EQUAL_DOUBLE(0.75, zip(0.75).second);
    TEST_ASSERT_EQUAL_DOUBLE(1.56, zip(1.25).first);
    TEST_ASSERT_EQUAL_DOUBLE(1.25, zip(1.25).second);
    TEST_ASSERT_EQUAL_DOUBLE(3.06, zip(1.75).first);
    TEST_ASSERT_EQUAL_DOUBLE(9.19, zip(1.75).second);

    auto [first,second] = SegmentFunction<double>::Unzip(zip);
    TEST_ASSERT_EQUAL(3, first.GetSize());
    TEST_ASSERT_EQUAL(3, second.GetSize());
    TEST_ASSERT_EQUAL_DOUBLE(4, first(0.75));
    TEST_ASSERT_EQUAL_DOUBLE(0.75, second(0.75));
    TEST_ASSERT_EQUAL_DOUBLE(1.56, first(1.25));
    TEST_ASSERT_EQUAL_DOUBLE(1.25, second(1.25));
    TEST_ASSERT_EQUAL_DOUBLE(3.06, first(1.75));
    TEST_ASSERT_EQUAL_DOUBLE(9.19, second(1.75));
}

void mutable_immutable(void) {
    SegmentFunction<double> segFunc;
    segFunc.Define(0.0, 1.0, [](double x) {return x;});
    segFunc.Define(1.0, 2.0, [](double x) {return x*x;});
    segFunc.Define(2.0, 3.0, [](double x) {return 4;});
    segFunc.Define(3.0, 4.0, [](double x) {return 3*x*x;});

    ImmutableSegmentFunction<double> segFuncIm(segFunc);
    TEST_ASSERT_EQUAL(4, segFuncIm.GetSize());
    TEST_ASSERT_EQUAL_DOUBLE(0.5, segFuncIm(0.5));
    TEST_ASSERT_EQUAL_DOUBLE(2.25, segFuncIm(1.5));
    TEST_ASSERT_EQUAL_DOUBLE(4, segFuncIm(2.5));
    TEST_ASSERT_EQUAL_DOUBLE(36.75, segFuncIm(3.5));
}

int run_tests(void) {
    UNITY_BEGIN();

    // Основные функции
    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);

    // Дополнительные функции
    RUN_TEST(map_where_reduce);
    RUN_TEST(zip_unzip);
    RUN_TEST(mutable_immutable);
    
    return UNITY_END();
}

#endif // TEST_HPP
