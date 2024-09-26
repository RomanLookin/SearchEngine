#include "searchserver.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

SearchServer::SearchServer()
{

}
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input,
                                                             int req_max, int max_response)
{
    //поиск по первым req_max из queries_input  запросам. если req_max=0, то поиск по всем запросам
    int k=0;
    std::vector<std::vector<RelativeIndex>> out_vector;
    for(auto& req : queries_input) {
        //cout << req << " " << endl;
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
        //std::map<size_t, float> otn_relev;
        size_t max_abs_relev = 0;
        std::vector<RelativeIndex> otn_relat;

        std::vector<std::string> request;
        for(auto kla : word_req){

            vec_count_word = _index.GetWordCount(kla);//если в словаре нет нужного слова
            //cout << kla << ":";
            int count_tot =0;
            for(size_t k=0; k < vec_count_word.size();k++){
                //cout << "{" << vec_count_word[k].doc_id << " " << vec_count_word[k].count << "}";
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
            //cout << " sum=" << count_tot << endl;
        }

        // это вектор для сортировки
        std::vector<std::pair<size_t, float>> fr;
        for(const auto& el : abs_relev) {
            if(max_abs_relev != 0)fr.push_back({el.first, (float)el.second/max_abs_relev});
        }
        std::sort(fr.begin(), fr.end(), [](const auto& a, const auto& b)
        { return a.second != b.second?  a.second > b.second : a.first < b.first;});


        for(auto& it : fr) {
            RelativeIndex RE;
            RE.doc_id = it.first;
            RE.rank = it.second;
            otn_relat.push_back(RE);
        }

        if(otn_relat.size() > max_response){
            otn_relat.erase(otn_relat.begin()+max_response, otn_relat.end());
        }

        out_vector.push_back(otn_relat);

        k++;
        if((k >= req_max) && (req_max != 0))break;
    }

    return out_vector;


}
