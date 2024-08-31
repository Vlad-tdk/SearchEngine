//
// Created by Vladimir on 31.8.24..
//
#include "ConverterJSON.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

nlohmann::json ConverterJSON::readJSON(const std::string& filename) {
    std::ifstream file;

    try {
        std::cout << "Attempting to open file: " << filename << std::endl;
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Error: " + filename + " file is missing or cannot be opened.");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to open " + filename + ": " + std::string(e.what()));
    }

    nlohmann::json jsonData;

    try {
        file >> jsonData;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Error: " + filename + " contains invalid JSON: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to read " + filename + ": " + std::string(e.what()));
    }

    if (jsonData.empty()) {
        throw std::runtime_error("Error: " + filename + " is empty.");
    }

    return jsonData;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> files;

    try {
        auto configData = readJSON("config.json");

        if (!configData.contains("config")) {
            throw std::runtime_error("Error: 'config' section is missing in config.json.");
        }

        for (const auto& file : configData["files"]) {
            files.push_back(file.get<std::string>());
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to retrieve text documents: " + std::string(e.what()));
    }
    return files;
}

int ConverterJSON::GetResponsesLimit() {
    int maxResponses = 5;

    try {
        auto configData = readJSON("config.json");

        if (configData["config"].contains("max_responses")) {
            maxResponses = configData["config"]["max_responses"].get<int>();
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to retrieve 'max_responses' from config.json: " + std::string(e.what()));
    }

    return maxResponses;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;

    try {
        auto requestsData = readJSON("requests.json");
        for (const auto& request : requestsData["requests"]) {
            requests.push_back(request.get<std::string>());
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to retrieve requests from requests.json: " + std::string(e.what()));
    }

    return requests;
}
void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    nlohmann::json answersData;

    try {
        for (size_t i = 0; i < answers.size(); ++i) {
            nlohmann::json result;
            if (answers[i].empty()) {
                result["result"] = "false";
            } else {
                result["result"] = "true";
                nlohmann::json relevance;
                for (const auto& [docid, rank] : answers[i]) {
                    relevance.push_back({ {"docid", docid}, {"rank", rank} });
                }
                result["relevance"] = relevance;
            }
            answersData["answers"]["request" + std::to_string(i + 1)] = result; // Формируем окончательный ответ
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to construct answers JSON: " + std::string(e.what()));
    }

    std::ofstream file;

    try {
        file.open("answers.json");
        if (!file.is_open()) {
            throw std::runtime_error("Error: Unable to open or create answers.json for writing.");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to open answers.json: " + std::string(e.what()));
    }

    try {
        file << answersData.dump(4);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: Failed to write to answers.json: " + std::string(e.what()));
    }
}