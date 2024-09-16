#include "converterjson.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdio.h>
#include <vector>
#include <string>
//#include <iomanip>

std::string count_words(const std::string& str)
{
    std::string word, out_str;

    std::stringstream words(str);
    size_t count_word = 0;
    while (words >> word)
    {
        if(word.size() > 100)word.erase(100);
        if(count_word == 0)out_str=word;
        else out_str+=" "+word;
        count_word++;
        if(count_word == 999)break;
    }

    return out_str;
}

using json = nlohmann::json;

ConverterJSON::ConverterJSON()
{

}
bool ConverterJSON::findDoc (const std::vector<std::string>& data, std::string value)
{
    auto result{ std::find(begin(data), end(data), value) };
    if (result == end(data))
        return false;
    else
        return true;
 }
std::vector<std::string> ConverterJSON::getTextDocuments(std::string path_dir, std::string fname, bool& update_needed)
{
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    std::ifstream in;       // поток для чтения
    std::vector<std::string> msv_out; //массив текстов из файлов

    in.open(path_dir+fname);

    if(in.is_open()){
        nlohmann::json dict;
        json data = json::parse(in);
        bool config_empty = true;
        for(json::iterator it = data.begin(); it != data.end(); ++it){


            if(it.key()=="files"){
                for(auto& path_file : data["files"]) {
                    std::string path_file_res = path_dir+(std::string)path_file;
                    //проверяем имеющиеся и получаемые пути к докам .
                    if(!findDoc(this->path_files_resource, path_file_res)){
                        this->path_files_resource.push_back(path_file_res);//добавляем новый путь файла с текстами
                        update_needed = true;//если нашли новый файл, то надо обновить invertedindex
                        std::ifstream in_files;       // поток для чтения

                        in_files.open(path_file_res);  //открываем файлы с текстами

                        if(in_files.is_open())
                        {

                            std::string line;
                            while (std::getline(in_files, line)) //читаем файл по строкам. каждая из этих строк будет документом
                            {

                                line = count_words(line);
                                msv_out.push_back(line); //записываем доки в выходной вектор

                            }
                            in_files.close();
                        }
                        else{
                            std::cout << "Cannot open file: " << path_file << std::endl;
                        }
                    }
                }
            }
            else if(it.key()=="requests"){
                for(const auto& file_req : data["requests"]){
                    std::string path_file_req = path_dir+(std::string)file_req;

                    this->path_files_requests = path_file_req;
                    config_empty = false;

                    std::ifstream in_files;       // поток для чтения
                    in_files.open(path_file_req);  //открываем файлы с текстами
                    if (in_files.is_open())
                    {
                        int count_request=0;
                        std::string line;
                        while (std::getline(in_files, line)) //читаем файл по строкам. каждая из этих строк будет документом
                        {
                            msv_out.push_back(line); //записываем запросы в выходной вектор
                            count_request++;
                            if(count_request == this->max_request)break;
                        }
                        in_files.close();
                    }
                    else{
                        std::cout << "Cannot open file: " << path_file_req << std::endl;
                    }
                }


            }
            else if(it.key() == "config"){
                config_empty = false;
                json dataact =it.value();
                for(json::iterator ita = dataact.begin(); ita != dataact.end(); ++ita){
                    if(ita.key() == "name"){
                        std::cout << "Name: " << ita.value() << std::endl;
                        this->name = ita.value();
                    }
                    else if(ita.key() == "version"){
                        std::cout << "Version: " << ita.value() << std::endl;
                        //this->version_prog = ita.value();
                        if(ita.value() != this->version_prog)
                            throw std::runtime_error("Config.json has incorrect file version.");
                    }
                    else if(ita.key() == "max_responses"){
                        std::cout << "Max responses: " << ita.value() << std::endl;
                        this->max_responses = ita.value();

                    }
                }
            }
        }
        if(config_empty)
            throw std::runtime_error("Config file is empty.");

        in.close();             // закрываем файл
        return msv_out;

    }
    else{
        throw std::runtime_error("Config file is missing.");

    }
}
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers,
                               std::string path_file_answers)
{
    std::ofstream out;       // поток для записи
    out.open(path_file_answers, std::ios::trunc);

    if(out.is_open()){
        int num_req = 0;
        json data,data1;

        for(auto& answer : answers){
            json data2;
            if(answer.size()==0){
                data2["result"]="false";
            }
            else if(answer.size()==1){
                data2["result"]="true";
                std::pair<int, float> Pair=answer[0];
                data2["docid"]= Pair.first;
                data2["rank"]=(double)Pair.second;

            }
            else if(answer.size()>1){
                data2["result"]="true";
                json arr;
                for(auto& answ_pair : answer){
                    int qqq=(double)answ_pair.second * 1000.0;
                    json Doc={{"docid", answ_pair.first}, {"rank", (double)qqq/1000.0}};
                    arr.push_back(Doc);
                }
                data2["relevance"]=arr;
            }
            std::string str_data1 = "request", probel;
            if(num_req < 10)probel= "00";
            else if(num_req < 100 && num_req > 9)probel= "0";
            str_data1 += probel + std::to_string(num_req);
            data1[str_data1]=data2;

            num_req++;
        }
        data["answers"] = data1;
        out << data;
        out.close();
    }
    else
        throw std::runtime_error("Answers file is missing.");
}
