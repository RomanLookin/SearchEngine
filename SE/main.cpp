#include <iostream>
#include "converterjson.h"
#include "invertedindex.h"
#include "searchserver.h"
#include <vector>
#include <string>
#include <map>
//#include <Windows.h>
#include <thread>
#include <mutex>
//#include <algorithm>
//#include <utility>
using namespace std;

/*std::mutex mtx;

void thread_proc(InvertedIndex* indx, int tnum, string doc) {
    //for(;;) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << tnum << endl;
            indx->UpdateDocumentBaseOne(doc);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //}
}*/


int main()
{
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);
    system("chcp 65001");
    try
    {
        size_t doc_max =0;//количество загружамых документов за 1 раз
        int req_max =20;//количество загружаемых запросов
        ConverterJSON file_json;
        InvertedIndex *inv_index = new InvertedIndex;//InvertedIndex inv_index;
        std::vector<std::string> docs;
        std::vector<std::string> requests;
        bool load_docs = false, load_req = false;
        std::string path_file_conf = "config1.json";
        std::string path_file_req = "requests.json";
        std::string path_file_answ = "answers.json";
        while(true){
            if(!load_req){
                cout << "Enter: 1(load docs), 2(show docs), 3(load request), 4(exit): " << std::endl;
            }
            else {
                cout << "Enter: 1(load docs), 2(show docs), 3(show request), 4(search), 5(exit): " << std::endl;
            }
            string user_choice;
            cin >> user_choice;
            if(user_choice == "1"){//каждый раз загружается doc_max документов
                //если doc_max=0,то загружаются все документы
                bool update_needed = false;
                docs = file_json.GetTextDocuments(path_file_conf, update_needed);
                if(update_needed){
                    cout << "Load docs..." << std::endl;
                    load_docs = true;

                    //for(size_t k = 0; k < docs.size(); k++) {
                    //    std::cout << k << ":\t" << docs.at(k) << std::endl;
                    //inv_index->add_doc(docs, doc_max);

                    inv_index->UpdateDocumentBase(docs, doc_max);
                    docs.clear();
                    //если doc_max=0,то обрабатываются все документы
                    //иначе только первые doc_max
                    //inv_index->show_freq_dict();
                }
                else cout << "No update docs." << std::endl;
            }
            else if(user_choice == "2"){
                //int k = 0;
                inv_index->show_docs();
            }
            else if(user_choice == "3"){
                if(!load_req){
                    bool get_upd = false;
                    if(req_max != file_json.max_request)file_json.max_request = req_max;
                    cout << "Load requests..." << std::endl;
                    requests = file_json.GetTextDocuments(path_file_req, get_upd);
                    cout << "Load " << requests.size() << " requests." << endl;
                    load_req = true;
                }
                else{
                    size_t k = 0;
                    for(auto& req : requests) {
                        std::cout << k << ":\t" << req << std::endl;
                        //inv_index.add_docs(docs);
                        k++;
                        //if(k>=req_max)break;
                    }
                }
            }
            else if(!load_req && user_choice == "4"){
                std::cout << "Bay...";
                break;
            }
            else if(load_docs && load_req && user_choice == "4"){
                std::vector<std::vector<RelativeIndex>> out_vector;
                SearchServer SS(*inv_index);


                out_vector=SS.search(requests,req_max,file_json.max_responses);


                std::vector<std::vector<std::pair<int, float>>> answers;
                //int numb_req=0;
                for(auto& it1 : out_vector){
                    std::vector<std::pair<int, float>> answer_2;
                    //cout << numb_req;
                    for(auto& it : it1){
                        std::pair<int, float> Pair_answ;
                        Pair_answ.first = it.doc_id;
                        Pair_answ.second = it.rank;
                        answer_2.push_back(Pair_answ);
                        //cout << "\t{" << it.doc_id << ':' << it.rank << '}' << endl;
                    }
                    //numb_req++;
                    answers.push_back(answer_2);
                }



                file_json.putAnswers(answers);



            }
            else if(load_req && user_choice == "5"){
                std::cout << "Bay...";
                break;

            }

        }
    }
    catch (const std::runtime_error& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    catch (const std::exception&)
    {
        std::cout << "Something wrong"<< std::endl;
    }
    return 0;
}

