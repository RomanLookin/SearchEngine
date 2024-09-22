#include "searchserver.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

SearchServer::SearchServer()
{

}
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input,
                                                             int max_req, int max_response)
{
    //поиск по первым req_max из queries_input  запросам. если req_max=0, то поиск по всем запросам
    int count_queries_input=0;
    std::vector<std::vector<RelativeIndex>> out_vector;
    for(auto& req : queries_input) {

        std::vector<std::string> word_req;
        std::stringstream ss(req);

        std::string word;
        while (ss >> word) {
            word_req.push_back(word);
        }
        sort(word_req.begin(),word_req.end());
        auto end_unique = unique(word_req.begin(), word_req.end());
        word_req.erase(end_unique, word_req.end());

        std::vector<Entry> vec_count_word;

        std::map<size_t, size_t> abs_relev;

        size_t max_abs_relev = 0;
        std::vector<RelativeIndex> otn_relat;

        std::vector<std::string> request;
        for(auto elem_word_req : word_req){

            vec_count_word = _index.getWordCount(elem_word_req);

            int count_tot =0;
            for(size_t k=0; k < vec_count_word.size();k++){

                count_tot +=vec_count_word[k].count;
                if(abs_relev.empty()){
                    abs_relev[vec_count_word[k].doc_id] = vec_count_word[k].count;
                }
                else{
                    if(abs_relev.count(vec_count_word[k].doc_id) == 0){
                        abs_relev[vec_count_word[k].doc_id] = vec_count_word[k].count;
                    }
                    else{
                        abs_relev[vec_count_word[k].doc_id] += vec_count_word[k].count;

                    }
                }
                if(abs_relev[vec_count_word[k].doc_id] > max_abs_relev)
                    max_abs_relev = abs_relev[vec_count_word[k].doc_id];
            }

        }

        // это вектор для сортировки
        std::vector<std::pair<size_t, float>> vect_sort;
        for(const auto& elem_abs_relev : abs_relev) {
            if(max_abs_relev != 0)vect_sort.push_back({elem_abs_relev.first,
                                                (float)elem_abs_relev.second/max_abs_relev});
            else
                throw std::runtime_error("Division by zero.");
        }
        std::sort(vect_sort.begin(), vect_sort.end(), [](const auto& a, const auto& b)
        {
            return a.second != b.second?  a.second > b.second : a.first < b.first;
        });


        for(auto& elem_vect_sort : vect_sort) {
            RelativeIndex RE;
            RE.doc_id = elem_vect_sort.first;
            RE.rank = elem_vect_sort.second;
            otn_relat.push_back(RE);
        }

        if(otn_relat.size() > max_response){
            otn_relat.erase(otn_relat.begin()+max_response, otn_relat.end());
        }

        out_vector.push_back(otn_relat);

        count_queries_input++;
        if((count_queries_input >= max_req) && (max_req != 0))break;
    }

    return out_vector;


}
