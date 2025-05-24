#ifndef TEST_HPP
#define TEST_HPP

#include "../ArraySequence.hpp"
#include "../ListSequence.hpp"
#include "unity.h"
using namespace std;


void setUp(void) {}

void tearDown(void) {}

// ArraySequence (по умолчанию изменяемая)
// Создание объекта
inline void test_array_constructor_1(void) {
    ArraySequence<int> arrSeq;
    TEST_ASSERT_EQUAL(0, arrSeq.GetLength());
}

inline void test_array_constructor_2(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    TEST_ASSERT_EQUAL(5, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(1, arrSeq.GetFirst());
    TEST_ASSERT_EQUAL(5, arrSeq.GetLast());
    TEST_ASSERT_EQUAL(2, arrSeq.Get(1));
    TEST_ASSERT_EQUAL(4, arrSeq.Get(3));
}

inline void test_array_constructor_3(void) {
    int items[] = {1, 2, 3, 4, 5};
    DynamicArray<int> arr(items, 5);
    ArraySequence<int> arrSeq(arr);
    TEST_ASSERT_EQUAL(5, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(1, arrSeq.GetFirst());
    TEST_ASSERT_EQUAL(5, arrSeq.GetLast());
    TEST_ASSERT_EQUAL(2, arrSeq.Get(1));
    TEST_ASSERT_EQUAL(4, arrSeq.Get(3));
}

// Декомпозиция
inline void test_array_decomposition(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    auto arrSeqSub = arrSeq.GetSubsequence(1, 3);
    TEST_ASSERT_EQUAL(3, arrSeqSub->GetLength());
    TEST_ASSERT_EQUAL(2, arrSeqSub->GetFirst());
    TEST_ASSERT_EQUAL(4, arrSeqSub->GetLast());
    TEST_ASSERT_EQUAL(3, arrSeqSub->Get(1));
    TEST_ASSERT_EQUAL(3, (*arrSeqSub)[1]);
    TEST_ASSERT_EQUAL(2, arrSeq[1]);
}

// Операции
inline void test_array_append(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    arrSeq.Append(6);
    TEST_ASSERT_EQUAL(6, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(6, arrSeq.GetLast());
}

inline void test_array_prepend(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    arrSeq.Prepend(0);
    TEST_ASSERT_EQUAL(6, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(0, arrSeq.GetFirst());
}

inline void test_array_remove(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    arrSeq.Remove(0);
    arrSeq.Remove(1);
    arrSeq.Remove(2);
    TEST_ASSERT_EQUAL(2, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(2, arrSeq.GetFirst());
    TEST_ASSERT_EQUAL(4, arrSeq.GetLast());
}

inline void test_array_insertat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    arrSeq.InsertAt(10, 2);
    TEST_ASSERT_EQUAL(10, arrSeq.Get(2));
}

inline void test_array_putat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ArraySequence<int> arrSeq(items, 5);
    arrSeq.PutAt(10, 2);
    TEST_ASSERT_EQUAL(6, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(2, arrSeq.Get(1));
    TEST_ASSERT_EQUAL(10, arrSeq.Get(2));
    TEST_ASSERT_EQUAL(3, arrSeq.Get(3));
}

inline void test_array_concat(void) {
    int items1[] = {1, 2, 3, 4, 5};
    int items2[] = {6, 7, 8, 9, 10};
    ArraySequence<int> arrSeq1(items1, 5);
    ArraySequence<int> arrSeq2(items2, 5);
    arrSeq1.Concat(&arrSeq2);
    TEST_ASSERT_EQUAL(10, arrSeq1.GetLength());
    TEST_ASSERT_EQUAL(1, arrSeq1.GetFirst());
    TEST_ASSERT_EQUAL(10, arrSeq1.GetLast());
}

// Операции (неизменяемая)
inline void test_immutable_array_append(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> arrSeq(items, 5);
    auto result = arrSeq.Append(6);
    TEST_ASSERT_EQUAL(6, result->GetLength());
    TEST_ASSERT_EQUAL(6, result->GetLast());
    TEST_ASSERT_EQUAL(5, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(5, arrSeq.GetLast());
}

inline void test_immutable_array_prepend(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> arrSeq(items, 5);
    auto result = arrSeq.Prepend(0);
    TEST_ASSERT_EQUAL(6, result->GetLength());
    TEST_ASSERT_EQUAL(0, result->GetFirst());
    TEST_ASSERT_EQUAL(5, arrSeq.GetLength());
    TEST_ASSERT_EQUAL(1, arrSeq.GetFirst());
}

inline void test_immutable_array_remove(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> arrSeq(items, 5);
    auto result = arrSeq.Remove(0);
    result = result->Remove(1);
    result = result->Remove(2);
    TEST_ASSERT_EQUAL(2, result->GetLength());
    TEST_ASSERT_EQUAL(2, result->GetFirst());
    TEST_ASSERT_EQUAL(4, result->GetLast());
    TEST_ASSERT_EQUAL(5, arrSeq.GetLength());
}

inline void test_immutable_array_insertat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> arrSeq(items, 5);
    auto result = arrSeq.InsertAt(10, 2);
    TEST_ASSERT_EQUAL(10, result->Get(2));
    TEST_ASSERT_EQUAL(3, arrSeq.Get(2));
}

inline void test_immutable_array_putat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> arrSeq(items, 5);
    auto result = arrSeq.PutAt(10, 2);
    TEST_ASSERT_EQUAL(6, result->GetLength());
    TEST_ASSERT_EQUAL(2, result->Get(1));
    TEST_ASSERT_EQUAL(10, result->Get(2));
    TEST_ASSERT_EQUAL(3, result->Get(3));
    TEST_ASSERT_EQUAL(5, arrSeq.GetLength());
}

inline void test_immutable_array_concat(void) {
    int items1[] = {1, 2, 3, 4, 5};
    int items2[] = {6, 7, 8, 9, 10};
    ImmutableArraySequence<int> arrSeq1(items1, 5);
    ArraySequence<int> arrSeq2(items2, 5);
    auto result = arrSeq1.Concat(&arrSeq2);
    TEST_ASSERT_EQUAL(10, result->GetLength());
    TEST_ASSERT_EQUAL(1, result->GetFirst());
    TEST_ASSERT_EQUAL(10, result->GetLast());
    TEST_ASSERT_EQUAL(5, arrSeq1.GetLength());
    TEST_ASSERT_EQUAL(1, arrSeq1.GetFirst());
    TEST_ASSERT_EQUAL(5, arrSeq1.GetLast());
}

// ListSequence (по умолчанию изменяемая)
// Создание объекта
inline void test_list_constructor_1(void) {
    ListSequence<int> listSeq;
    TEST_ASSERT_EQUAL(0, listSeq.GetLength());
}

inline void test_list_constructor_2(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    TEST_ASSERT_EQUAL(5, listSeq.GetLength());
    TEST_ASSERT_EQUAL(1, listSeq.GetFirst());
    TEST_ASSERT_EQUAL(5, listSeq.GetLast());
    TEST_ASSERT_EQUAL(2, listSeq.Get(1));
    TEST_ASSERT_EQUAL(4, listSeq.Get(3));
}

inline void test_list_constructor_3(void) {
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);
    ListSequence<int> listSeq(list);
    TEST_ASSERT_EQUAL(5, listSeq.GetLength());
    TEST_ASSERT_EQUAL(1, listSeq.GetFirst());
    TEST_ASSERT_EQUAL(5, listSeq.GetLast());
    TEST_ASSERT_EQUAL(2, listSeq.Get(1));
    TEST_ASSERT_EQUAL(4, listSeq.Get(3));
}

// Декомпозиция
inline void test_list_decomposition(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    auto listSeqSub = listSeq.GetSubsequence(1, 3);
    TEST_ASSERT_EQUAL(3, listSeqSub->GetLength());
    TEST_ASSERT_EQUAL(2, listSeqSub->GetFirst());
    TEST_ASSERT_EQUAL(4, listSeqSub->GetLast());
    TEST_ASSERT_EQUAL(3, listSeqSub->Get(1));
    TEST_ASSERT_EQUAL(3, (*listSeqSub)[1]);
    TEST_ASSERT_EQUAL(2, listSeq[1]);
}

// Операции
inline void test_list_append(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    listSeq.Append(6);
    TEST_ASSERT_EQUAL(6, listSeq.GetLength());
    TEST_ASSERT_EQUAL(6, listSeq.GetLast());
}

inline void test_list_prepend(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    listSeq.Prepend(0);
    TEST_ASSERT_EQUAL(6, listSeq.GetLength());
    TEST_ASSERT_EQUAL(0, listSeq.GetFirst());
}

inline void test_list_remove(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    listSeq.Remove(0);
    listSeq.Remove(1);
    listSeq.Remove(2);
    TEST_ASSERT_EQUAL(2, listSeq.GetLength());
    TEST_ASSERT_EQUAL(2, listSeq.GetFirst());
    TEST_ASSERT_EQUAL(4, listSeq.GetLast());
}

inline void test_list_insertat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    listSeq.InsertAt(10, 2);
    TEST_ASSERT_EQUAL(10, listSeq.Get(2));
}

inline void test_list_putat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ListSequence<int> listSeq(items, 5);
    listSeq.PutAt(10, 2);
    TEST_ASSERT_EQUAL(6, listSeq.GetLength());
    TEST_ASSERT_EQUAL(2, listSeq.Get(1));
    TEST_ASSERT_EQUAL(10, listSeq.Get(2));
    TEST_ASSERT_EQUAL(3, listSeq.Get(3));
}

inline void test_list_concat(void) {
    int items1[] = {1, 2, 3, 4, 5};
    int items2[] = {6, 7, 8, 9, 10};
    ListSequence<int> listSeq1(items1, 5);
    ListSequence<int> listSeq2(items2, 5);
    listSeq1.Concat(&listSeq2);
    TEST_ASSERT_EQUAL(10, listSeq1.GetLength());
    TEST_ASSERT_EQUAL(1, listSeq1.GetFirst());
    TEST_ASSERT_EQUAL(10, listSeq1.GetLast());
}

// Операции (неизменяемая)
inline void test_immutable_list_append(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> listSeq(items, 5);
    auto result = listSeq.Append(6);
    TEST_ASSERT_EQUAL(6, result->GetLength());
    TEST_ASSERT_EQUAL(6, result->GetLast());
    TEST_ASSERT_EQUAL(5, listSeq.GetLength());
    TEST_ASSERT_EQUAL(5, listSeq.GetLast());
}

inline void test_immutable_list_prepend(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> listSeq(items, 5);
    auto result = listSeq.Prepend(0);
    TEST_ASSERT_EQUAL(6, result->GetLength());
    TEST_ASSERT_EQUAL(0, result->GetFirst());
    TEST_ASSERT_EQUAL(5, listSeq.GetLength());
    TEST_ASSERT_EQUAL(1, listSeq.GetFirst());
}

inline void test_immutable_list_remove(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableListSequence<int> listSeq(items, 5);
    auto result = listSeq.Remove(0);
    result = result->Remove(1);
    result = result->Remove(2);
    TEST_ASSERT_EQUAL(2, result->GetLength());
    TEST_ASSERT_EQUAL(2, result->GetFirst());
    TEST_ASSERT_EQUAL(4, result->GetLast());
    TEST_ASSERT_EQUAL(5, listSeq.GetLength());
}

inline void test_immutable_list_insertat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> listSeq(items, 5);
    auto result = listSeq.InsertAt(10, 2);
    TEST_ASSERT_EQUAL(10, result->Get(2));
    TEST_ASSERT_EQUAL(3, listSeq.Get(2));
}

inline void test_immutable_list_putat(void) {
    int items[] = {1, 2, 3, 4, 5};
    ImmutableListSequence<int> listSeq(items, 5);
    auto result = listSeq.PutAt(10, 2);
    TEST_ASSERT_EQUAL(6, result->GetLength());
    TEST_ASSERT_EQUAL(2, result->Get(1));
    TEST_ASSERT_EQUAL(10, result->Get(2));
    TEST_ASSERT_EQUAL(3, result->Get(3));
    TEST_ASSERT_EQUAL(5, listSeq.GetLength());
}

inline void test_immutable_list_concat(void) {
    int items1[] = {1, 2, 3, 4, 5};
    int items2[] = {6, 7, 8, 9, 10};
    ImmutableArraySequence<int> listSeq1(items1, 5);
    ListSequence<int> listSeq2(items2, 5);
    auto result = listSeq1.Concat(&listSeq2);
    TEST_ASSERT_EQUAL(10, result->GetLength());
    TEST_ASSERT_EQUAL(1, result->GetFirst());
    TEST_ASSERT_EQUAL(10, result->GetLast());
    TEST_ASSERT_EQUAL(5, listSeq1.GetLength());
    TEST_ASSERT_EQUAL(1, listSeq1.GetFirst());
    TEST_ASSERT_EQUAL(5, listSeq1.GetLast());
}

inline int run_tests(void) {
    UNITY_BEGIN();

    // ArraySequence (по умолчанию изменяемая)
    // Создание объекта
    RUN_TEST(test_array_constructor_1);
    RUN_TEST(test_array_constructor_2);
    RUN_TEST(test_array_constructor_3);

    // Декомпозиция
    RUN_TEST(test_array_decomposition);

    // Операции
    RUN_TEST(test_array_append);
    RUN_TEST(test_array_prepend);
    RUN_TEST(test_array_remove);
    RUN_TEST(test_array_insertat);
    RUN_TEST(test_array_putat);
    RUN_TEST(test_array_concat);

    // Операции (неизменяемая)
    RUN_TEST(test_immutable_array_append);
    RUN_TEST(test_immutable_array_prepend);
    RUN_TEST(test_immutable_array_remove);
    RUN_TEST(test_immutable_array_insertat);
    RUN_TEST(test_immutable_array_putat);
    RUN_TEST(test_immutable_array_concat);

    // ListSequence (по умолчанию изменяемая)
    // Создание объекта
    RUN_TEST(test_list_constructor_1);
    RUN_TEST(test_list_constructor_2);
    RUN_TEST(test_list_constructor_3);

    // Декомпозиция
    RUN_TEST(test_list_decomposition);

    // Операции
    RUN_TEST(test_list_append);
    RUN_TEST(test_list_prepend);
    RUN_TEST(test_list_remove);
    RUN_TEST(test_list_insertat);
    RUN_TEST(test_list_putat);
    RUN_TEST(test_list_concat);

    // Операции (неизменяемая)
    RUN_TEST(test_immutable_list_append);
    RUN_TEST(test_immutable_list_prepend);
    RUN_TEST(test_immutable_list_remove);
    RUN_TEST(test_immutable_list_insertat);
    RUN_TEST(test_immutable_list_putat);
    RUN_TEST(test_immutable_list_concat);
    
    return UNITY_END();
}

#endif // TEST_HPP
