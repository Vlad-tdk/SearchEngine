#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <string>
#include <map>
#include <mutex> // Для работы с мьютексами

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator==(const Entry& other) const {
        return doc_id == other.doc_id && count == other.count;
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    [[nodiscard]] std::vector<Entry> GetWordCount(const std::string& word) const;

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    mutable std::mutex mtx; // Мьютекс для защиты freq_dictionary
};

#endif // INVERTED_INDEX_H
