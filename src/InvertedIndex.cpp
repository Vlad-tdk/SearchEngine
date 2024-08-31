//
// Created by Vladimir on 31.8.2024
//
#include "InvertedIndex.h"
#include <sstream>
#include <stdexcept>
#include <thread>
#include <algorithm>
#include <ranges>  // Для использования std::ranges

/**
 * Метод для обновления базы документов.
 * Перестраивает частотный словарь для новых документов.
 *
 * @param input_docs Вектор строк, каждая строка представляет содержимое одного документа.
 */
void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    try {
        docs = input_docs;
        freq_dictionary.clear();

        std::vector<std::thread> threads;

        for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
            threads.emplace_back([this, doc_id]() {
                std::istringstream stream(docs[doc_id]);
                std::string word;

                std::map<std::string, std::vector<Entry>> local_freq_dictionary;

                while (stream >> word) {
                    auto& entries = local_freq_dictionary[word];

                    auto it = std::ranges::find_if(entries, [doc_id](const Entry& entry) {
                        return entry.doc_id == doc_id;
                    });

                    if (it != entries.end()) {
                        it->count++;
                    } else {
                        entries.push_back(Entry{doc_id, 1});
                    }
                }

                std::lock_guard<std::mutex> lock(mtx);
                for (auto& [word, entries] : local_freq_dictionary) {
                    freq_dictionary[word].insert(
                        freq_dictionary[word].end(), entries.begin(), entries.end());
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Error in UpdateDocumentBase: " + std::string(e.what()));
    }
}

/**
 * Метод для получения частоты вхождений слова в базе документов.
 *
 * @param word Слово, частоту вхождений которого необходимо определить.
 * @return Вектор Entry с частотами для каждого документа, содержащего слово.
 */
[[nodiscard]] std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    try {
        std::lock_guard<std::mutex> lock(mtx); // Защита от одновременного чтения и записи
        if (freq_dictionary.contains(word)) {
            return freq_dictionary.at(word);
        } else {
            return {};
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in GetWordCount: " + std::string(e.what()));
    }
}
