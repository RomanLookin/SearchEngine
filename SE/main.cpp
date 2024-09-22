#include <iostream>
#include "converterjson.h"
#include "invertedindex.h"
#include "searchserver.h"
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <mutex>

using namespace std;


int main(int argc, char *argv[])
{
    system("chcp 65001");

    string path_exe = argv[0];
    string path_dir = path_exe.substr(0, path_exe.find_last_of( "\\/" ))+"\\";

    try
    {
        size_t max_doc =20;//количество загружамых документов за 1 раз
        int max_request =20;//количество загружаемых запросов
        ConverterJSON file_json;
        InvertedIndex *inv_index = new InvertedIndex;
        std::vector<std::string> docs;
        std::vector<std::string> requests;
        bool load_docs = false, load_req = false;
        std::string file_conf = "config1.json";
        std::string file_req = "requests.json";
        std::string path_file_answ = path_dir+"answers.json";
        while(true){
            if(!load_req){
                cout << "Enter: 1(load docs), 2(show docs), 3(load request), 4(exit): " << std::endl;
            }
            else {
                cout << "Enter: 1(load docs), 2(show docs), 3(show request), 4(search), 5(exit): " << std::endl;
            }
            string user_choice;
            cin >> user_choice;
            if(user_choice == "1"){//каждый раз загружаются только первые doc_max документов
                //если doc_max=0,то загружаются все документы
                bool update_needed = false;
                docs = file_json.getTextDocuments(path_dir, file_conf, update_needed);
                if(update_needed){
                    cout << "Load docs..." << std::endl;
                    load_docs = true;

                    inv_index->updateDocumentBase(docs, max_doc);
                    docs.clear();
                }
                else cout << "No update docs." << std::endl;
            }
            else if(user_choice == "2"){
                inv_index->showDocs();
            }
            else if(user_choice == "3"){
                if(!load_req){
                    bool get_upd = false;
                    if(max_request != file_json.max_request)file_json.max_request = max_request;
                    cout << "Load requests..." << std::endl;
                    requests = file_json.getTextDocuments(path_dir, file_req,  get_upd);
                    cout << "Load " << requests.size() << " requests." << endl;
                    load_req = true;
                }
                else{
                    for(auto& req : requests) {
                        std::cout << req << std::endl;
                    }
                }
            }
            else if((!load_req && user_choice == "4") or(load_req && user_choice == "5")){
                std::cout << "Bay...";
                break;
            }
            else if(load_docs && load_req && user_choice == "4"){
                std::vector<std::vector<RelativeIndex>> out_vector;
                SearchServer SS(*inv_index);

                out_vector=SS.search(requests,max_request,file_json.max_responses);

                std::vector<std::vector<std::pair<int, float>>> answers;
                for(auto& out_vector_interior : out_vector){
                    std::vector<std::pair<int, float>> answer_2;
                    for(auto& pair_unit : out_vector_interior){
                        std::pair<int, float> Pair_answ;
                        Pair_answ.first = pair_unit.doc_id;
                        Pair_answ.second = pair_unit.rank;
                        answer_2.push_back(Pair_answ);
                    }
                    answers.push_back(answer_2);
                }
                file_json.putAnswers(answers, path_file_answ);
            }
        }
    }
    catch (const std::runtime_error& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    catch (const std::exception& err)
    {
        std::cout << "Something wrong"<< err.what() << std::endl;
    }
    return 0;
}

