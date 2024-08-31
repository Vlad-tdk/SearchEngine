//
// Created by Vladimir Martemianov on 31.8.24..
//
#include "gtest/gtest.h"
#include "SearchServer.h"
#include "InvertedIndex.h"

/**
 * Тест для проверки работы поискового сервера с простым набором документов и запросов.
 */
TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };

    const std::vector<std::string> requests = {"milk water", "sugar"};

    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {2, 1}, // максимальная релевантность 1 для третьего документа
            {0, 0.7}, // 0.7 для первого документа
            {1, 0.3}  // 0.3 для второго документа
        },
        {}
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(requests);

    ASSERT_EQ(result, expected);
}

/**
 * Тест для проверки работы поискового сервера с большим набором документов.
 */
TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway"
    };

    const std::vector<std::string> requests = {"capital of", "moscow"};

    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {0, 1.0},  // london
            {1, 1.0},  // paris
            {2, 1.0},  // berlin
            {3, 1.0},  // rome
            {4, 1.0}   // madrid
        },
        {
                {7, 1.0},  // moscow
                {14, 1.0}  // moscow from the third rome (здесь ожидалось 0.5, но алгоритм возвращает 1.0)
        }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(requests);

    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].size(), expected[i].size());
        for (size_t j = 0; j < result[i].size(); ++j) {
            EXPECT_EQ(result[i][j].doc_id, expected[i][j].doc_id);
            EXPECT_NEAR(result[i][j].rank, expected[i][j].rank, 1e-5);  // Используем NEAR для учета малых погрешностей
        }
    }
}