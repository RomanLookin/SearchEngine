#include <vector>
#include <string>
#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H


class ConverterJSON
{
public:
    ConverterJSON();
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    std::vector<std::string> GetTextDocuments(std::string path_file, bool & update_needed);
    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * @return
    */
    //int GetResponsesLimit(std::string path_file);
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    //std::vector<std::string> GetRequests(std::string path_file);
    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
    bool find_doc (const std::vector<std::string>& data, std::string value);
    int max_responses = 5;
    int max_request = 10;
private:
    std::string name;

    std::string version;
    std::vector<std::string> path_files_resourse;//список адресов файлов документов
    std::string path_files_requests;//адрес файла с запросами
};

#endif // CONVERTERJSON_H
