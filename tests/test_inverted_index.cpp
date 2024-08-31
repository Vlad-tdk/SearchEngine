//
// Created by Vladimir Martemianov on 31.8.24..
//
#include "gtest/gtest.h"
#include "InvertedIndex.h"

/**
 * Функция для тестирования функциональности InvertedIndex.
 * Сравнивает результаты с ожидаемыми значениями.
 *
 * @param docs Вектор строк, представляющий документы.
 * @param requests Вектор строк, представляющий запросы.
 * @param expected Ожидаемые результаты.
 */
void TestInvertedIndexFunctionality(
    const std::vector<std::string>& docs,
    const std::vector<std::string>& requests,
    const std::vector<std::vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;

    // Обновляем базу документов
    idx.UpdateDocumentBase(docs);

    // Выполняем запросы
    for (const auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }

    // Сравниваем результаты с ожидаемыми
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
        {{0, 1}}, // london встречается в первом документе один раз
        {{0, 1}, {1, 3}} // the встречается в первом документе один раз и во втором три раза
    };

    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
    const std::vector<std::vector<Entry>> expected = {
        {{0, 4}, {1, 1}, {2, 5}}, // milk
        {{0, 3}, {1, 2}, {2, 5}}, // water
        {{3, 1}} // cappuccino
    };

    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
        {}, // m не встречается ни в одном документе
        {{1, 1}} // statement встречается один раз во втором документе
    };

    TestInvertedIndexFunctionality(docs, requests, expected);
}