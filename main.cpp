//
// Created by Vladimir on 31.8.24..
//
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>
#include <vector>
#include <filesystem>

int main() {
    const std::string CURRENT_VERSION = "0.1";

    try {
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

        ConverterJSON converter;

        auto configData = converter.readJSON("config.json");

        if (configData["config"].contains("name")) {
            std::string programName = configData["config"]["name"];
            std::cout << "Starting " << programName << std::endl;
        } else {
            throw std::runtime_error("Error: Program name is missing in config.json.");
        }

        if (configData["config"].contains("version")) {
            std::string configVersion = configData["config"]["version"];
            if (configVersion != CURRENT_VERSION) {
                throw std::runtime_error("Error: config.json has incorrect file version.");
            } else {
                std::cout << "Version: " << configVersion << std::endl;
            }
        } else {
            throw std::runtime_error("Error: Program version is missing in config.json.");
        }

        std::vector<std::string> documents = converter.GetTextDocuments();

        std::vector<std::string> requests = converter.GetRequests();

        InvertedIndex index;
        index.UpdateDocumentBase(documents);

        SearchServer search_server(index);

        std::vector<std::vector<RelativeIndex>> search_results = search_server.search(requests);

        std::vector<std::vector<std::pair<int, float>>> answers;
        for (const auto& result : search_results) {
            std::vector<std::pair<int, float>> doc_ranks;
            doc_ranks.reserve(result.size());

            for (const auto& entry : result) {
                doc_ranks.emplace_back(entry.doc_id, entry.rank);
            }
            answers.push_back(doc_ranks);
        }

        converter.putAnswers(answers);

        std::cout << "Поиск завершен. Результаты сохранены в answers.json." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}