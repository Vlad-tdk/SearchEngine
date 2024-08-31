//
// Created by Vladimir on 31.8.24..
//

#ifndef RELATIVE_INDEX_H
#define RELATIVE_INDEX_H

#include <cstddef>

struct RelativeIndex {
    size_t doc_id; // Идентификатор документа
    float rank;    // Релевантность документа

    // Оператор сравнения для использования в тестах
    bool operator==(const RelativeIndex& other) const {
        return doc_id == other.doc_id && rank == other.rank;
    }
};

#endif // RELATIVE_INDEX_H
