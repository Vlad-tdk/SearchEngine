//
// Created by Vladimir on 31.8.24..
//
#include "SearchServer.h"
#include <sstream>
#include <map>
#include <algorithm>
#include <iostream>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
        std::istringstream query_stream(query);
        std::vector<std::string> words;
        std::string word;

        while (query_stream >> word) {
            words.push_back(word);
        }

        std::map<size_t, size_t> doc_relevance;

        for (const auto& w : words) {
            auto entries = _index.GetWordCount(w);

            for (const auto& entry : entries) {
                doc_relevance[entry.doc_id] += entry.count;
            }
        }

        if (doc_relevance.empty()) {
            result.push_back({});
            continue;
        }

        size_t max_relevance = std::max_element(
            doc_relevance.begin(), doc_relevance.end(),
            [](const auto& lhs, const auto& rhs) {
                return lhs.second < rhs.second;
            })->second;

        std::cout << "Max relevance for query \"" << query << "\": " << max_relevance << std::endl;

        std::vector<RelativeIndex> relative_indices;

        for (const auto& [doc_id, relevance] : doc_relevance) {
            float rank = static_cast<float>(relevance) / max_relevance;

            std::cout << "Document ID: " << doc_id << ", Relevance: " << relevance << ", Rank: " << rank << std::endl;

            relative_indices.push_back({doc_id, rank});
        }

        std::sort(relative_indices.begin(), relative_indices.end(),
                  [](const RelativeIndex& lhs, const RelativeIndex& rhs) {
                      if (lhs.rank != rhs.rank) {
                          return lhs.rank > rhs.rank;
                      }
                      return lhs.doc_id < rhs.doc_id;
                  });

        const size_t max_responses = 5;
        if (relative_indices.size() > max_responses) {
            relative_indices.resize(max_responses);
        }

        result.push_back(relative_indices);
    }

    return result;
}
