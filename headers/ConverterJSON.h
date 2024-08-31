//
// Created by Vladimir on 31.8.24..
//

#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class ConverterJSON {
public:
    ConverterJSON() = default;

    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов, перечисленных в config.json
    */
    std::vector<std::string> GetTextDocuments();

    /**
    * Метод считывает поле max_responses для определения предельного количества ответов на один запрос
    * @return возвращает максимальное количество ответов
    */
    int GetResponsesLimit();

    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();

    /**
    * Положить в файл answers.json результаты поисковых запросов
    * @param answers - вектор, содержащий результаты поисковых запросов
    */
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);

    /**
    * Вспомогательный метод для чтения данных из JSON-файла
    * @param filename - имя JSON-файла
    * @return объект nlohmann::json, содержащий данные из файла
    */
    nlohmann::json readJSON(const std::string& filename);  // Переместили в public

private:
    // Здесь могут быть другие приватные методы или переменные
};

#endif // CONVERTER_JSON_H
