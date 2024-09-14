#include "converterjson.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdio.h>

using json = nlohmann::json;

ConverterJSON::ConverterJSON()
{

}
bool ConverterJSON::find_doc (const std::vector<std::string>& data, std::string value)
{
    auto result{ std::find(begin(data), end(data), value) };
    if (result == end(data))
        return false; //std::cout << "Value not found" << std::endl;
    else
        return true; //std::cout << "Value found at position " << (result - begin(data)) << std::endl;
 }
std::vector<std::string> ConverterJSON::GetTextDocuments(std::string path_file, bool& update_needed)
{
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    std::ifstream in;       // поток для чтения
    std::vector<std::string> msv_out; //массив текстов из файлов
    //upd = false;
    in.open(path_file);  //"C:\\Roman\\search_engine\\config.json");
    //
    if(in.is_open()){
        nlohmann::json dict;
        json data = json::parse(in);
        bool config_empty = true;
        for(json::iterator it = data.begin(); it != data.end(); ++it){
            //std::cout << "Find: " << it.key() << std::endl;

            if(it.key()=="files"){
                for(const auto& path_file : data["files"]) {
                    //std::cout << ele << std::endl;
                    //проверяем имеющиеся и получаемые пути к докам .
                    if(!find_doc(this->path_files_resourse, path_file)){
                        this->path_files_resourse.push_back(path_file);//добавляем новый путь файла с текстами
                        update_needed = true;//если нашли новый файл, то надо обновить invertedindex
                        std::ifstream in_files;       // поток для чтения
                        //in.setCodec("UTF-8");
                        in_files.open(path_file);  //открываем файлы с текстами
                        //std::cout << "Read " << l+1 << " file." << std::endl;
                        if (in_files.is_open())
                        {
                            int k=0;
                            std::string line;
                            while (std::getline(in_files, line)) //читаем файл по строкам. каждая из этих строк будет документом
                            {
                                //std::cout << k << ":" << line << std::endl;
                                msv_out.push_back(line); //записываем доки в выходной вектор
                                k++;
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
                for(const auto& ele : data["requests"]){
                    //std::cout << "Requests file: " << ele << std::endl;
                    this->path_files_requests = ele;
                    config_empty = false;

                    std::ifstream in_files;       // поток для чтения
                    in_files.open(ele);  //открываем файлы с текстами
                    //std::cout << "Read " << l+1 << " file." << std::endl;
                    if (in_files.is_open())
                    {
                        int k=0;
                        std::string line;
                        while (std::getline(in_files, line)) //читаем файл по строкам. каждая из этих строк будет документом
                        {
                            //std::cout << k << ":" << line << std::endl;
                            msv_out.push_back(line); //записываем запросы в выходной вектор
                            k++;
                            if(k == this->max_request)break;
                        }
                        in_files.close();
                    }
                    else{
                        std::cout << "Cannot open file: " << ele << std::endl;
                    }
                }


            }
            else if(it.key() == "config"){
                config_empty = false;
                json dataact =it.value();
                for(json::iterator ita = dataact.begin(); ita != dataact.end(); ++ita){
                   //std::cout << "--Find: " << ita.key() << "  " << ita.value() << std::endl;
                    if(ita.key() == "name"){
                        std::cout << "Name: " << ita.value() << std::endl;
                        this->name = ita.value();
                    }
                    else if(ita.key() == "version"){
                        std::cout << "Version: " << ita.value() << std::endl;
                        this->version = ita.value();
                        //if(ita.value() != version_prog)
                        //    throw std::runtime_error("Config.json has incorrect file version.");
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
        //return this->path_files_resourse;
    }
    else{
        throw std::runtime_error("Config file is missing.");
        //std::cout << "ok4";
    }
}
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{




    FILE * f = fopen("answers.json", "w");
    fprintf(f, "%s", "{\"answers\":{");
    int num_req = 0;


    for(auto& answer : answers){
        fprintf(f, "\"request%03d\":{", num_req);
        if(answer.size()==0){
            fprintf(f, "\"result\": \"false\"\n");
        }
        else if(answer.size()==1){
            fprintf(f, "\"result\": \"true\",\n");
            std::pair<int, float> Pair=answer[0];
            fprintf(f, "[\"docid\": %d, \"rank\":%.3f,]\n", Pair.first, (double)Pair.second);
        }
        else if(answer.size()>1){
            fprintf(f, "\"result\": \"true\",\n");
            fprintf(f, "%s", "\"relevance\":{");
            for(auto& answ_pair : answer){

                //std::pair Pair=answ_pair;
                //std::cout << "{" << answ_pair.first << ":" << answ_pair.second << std::endl;
                fprintf(f, "[\"docid\": %d, \"rank\":%.3f,]\n", answ_pair.first, answ_pair.second);
            }
            fprintf(f, "%s\n", "}");
        }
        fprintf(f, "%s\n", "},");
        num_req++;
    }

    //fprintf(f, "[\"docid\": %d, \"rank\":%.3f,]\n", 0, 0.997);
    //fprintf(f, "[\"docid\": %d, \"rank\":%.3f,]", 1, 0.897);



    fprintf(f, "%s", "}\n}");
    fclose(f);
}
/*std::vector<std::string> ConverterJSON::GetRequests(std::string path_file)
{
    std::ifstream in;       // поток для чтения
    in.open(path_file);  //
    if (in.is_open())
    {
        //std::vector<std::string> out;
        nlohmann::json dict;
        json data = json::parse(in);
        std::vector<std::string> msv_out;
        //bool config_empty = true;
        //std::vector<std::string> path_files_resourse;

        for(json::iterator it = data.begin(); it != data.end(); ++it){
            //std::cout << "*Find: " << it.key() << std::endl;

            if(it.key()=="requests"){
                //for(const auto& ele : data["requests"]) {
                    //std::cout << ele << std::endl;
                    //this->path_files_requests.push_back(data["requests"]);//ele);
                //}
            }
        }
        in.close();             // закрываем файл
        return msv_out;
    }
    else{
        throw std::runtime_error("Requests file is missing.");
    }
}*/
/*int ConverterJSON::GetResponsesLimit(std::string path_file){
    std::ifstream in;       // поток для чтения
    in.open(path_file);  //"C:\\Roman\\search_engine\\config.json");
    if (in.is_open())
    {
        //std::vector<std::string> out;
        nlohmann::json dict;
        json data = json::parse(in);
        bool config_empty = true;
        //std::vector<std::string> path_files_resourse;

        for(json::iterator it = data.begin(); it != data.end(); ++it){
            //std::cout << "Find: " << it.key() << std::endl;

            if(it.key()=="files"){
                for(const auto& ele : data["files"]) {
                    std::cout << ele << std::endl;
                    this->path_files_resourse.push_back(ele);
                }
            }
            else if(it.key() == "config"){
                config_empty = false;
                json dataact =it.value();
                for(json::iterator ita = dataact.begin(); ita != dataact.end(); ++ita){
                   //std::cout << "--Find: " << ita.key() << "  " << ita.value() << std::endl;
                    if(ita.key() == "name"){
                        std::cout << "Name: " << ita.value() << std::endl;
                        this->name = ita.value();
                    }
                    else if(ita.key() == "version"){
                        std::cout << "Version: " << ita.value() << std::endl;
                        this->version = ita.value();
                        //if(ita.value() != version_prog)
                        //    throw std::runtime_error("Config.json has incorrect file version.");
                    }
                    //else if(ita.key() == "max_responses")std::cout << "Max responses: " << ita.value() << std::endl;
                }
            }
        }
        if(config_empty)
            throw std::runtime_error("Config file is empty.");

        in.close();             // закрываем файл
        return this->path_files_resourse;
    }
    else{
        throw std::runtime_error("Config file is missing.");
    }


}*/
