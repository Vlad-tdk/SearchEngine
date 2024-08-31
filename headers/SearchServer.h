//
// Created by Vladimir on 31.8.24..
//

#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "InvertedIndex.h"
#include "RelativeIndex.h"
#include <vector>
#include <string>
#include <algorithm>

class SearchServer {
public:
    /**
    * @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов, встречаемых в запросе
    */
    SearchServer(InvertedIndex& idx) : _index(idx) { }

    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла requests.json
    * @return возвращает отсортированный список релевантных ответов для заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index; // Ссылка на инвертированный индекс
};

#endif // SEARCH_SERVER_H
