#include "invertedindex.h"
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <mutex>
#include <vector>
std::mutex mtx;

void thread_proc(InvertedIndex* indx, int tnum, std::string doc) {
        {
            indx->calcFreqDict(doc, tnum);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));

}

InvertedIndex::InvertedIndex()
{

}
void InvertedIndex::calcFreqDict(std::string input_doc, int numb_doc)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << numb_doc << std::endl;
    this->docs.push_back(input_doc);

    this->numb_doc.push_back(numb_doc);
    //разбираем строку по словам.
    //если слово есть в словаре, то добавляем 1 к счетчику слов
    //если нет, то добавляем новую пару

    std::stringstream ss(input_doc);
    std::string word;

    while(ss >> word){ // Extract word from the stream.
        if(this->freq_dictionary.count(word) == 0){//если в словаре нет нужного слова
            Entry new_rec;
            new_rec.doc_id = numb_doc;
            new_rec.count = 1;
            std::vector<Entry> rec_entry;
            rec_entry.push_back(new_rec);
            this->freq_dictionary[word] = rec_entry;
        }
        else{//если в словаре есть нужное слово
            std::vector<Entry> rec_entry = this->freq_dictionary[word];
            bool get_word = false;
            for(size_t elem_req_entry = 0; elem_req_entry< rec_entry.size(); elem_req_entry++){
                if(rec_entry[elem_req_entry].doc_id == numb_doc){
                    rec_entry[elem_req_entry].count += 1;
                    get_word = true;
                }
            }
            if(!get_word){
                Entry new_rec;
                new_rec.doc_id = numb_doc;
                new_rec.count = 1;
                rec_entry.push_back(new_rec);
            }
            this->freq_dictionary[word] = rec_entry;
        }
    }
}

void InvertedIndex::updateDocumentBase(std::vector<std::string> input_docs, int max_doc)
{
    /**
    * Обновить или заполнить базу документов, по которой будем совершать
    поиск
    * @param texts_input содержимое документов
    */

    size_t load_doc_count = 0;
    size_t numb_doc_size = this->numb_doc.size();
    std::vector<std::thread> threads;
    for(size_t i = 0; i < input_docs.size(); ++i) {
        std::string doc=input_docs[i];
        std::thread thr(thread_proc, this, numb_doc_size+i, doc);
        threads.push_back(std::move(thr));
        load_doc_count++;
        if(i == max_doc-1)break;
    }

    for(auto& thr : threads) {
        thr.join();
    }
    std::cout << "Load " << load_doc_count << " documents." << std::endl;
    std::vector<std::string> new_docs;
    std::vector<int> new_numb_doc;
    for(size_t doc=0; doc < this->docs.size();doc++){
        if(new_docs.empty()){
            for(size_t again_doc = 0; again_doc < this->docs.size(); again_doc++){
                if(this->numb_doc.at(again_doc) == doc){
                    new_docs.push_back(this->docs.at(again_doc));
                    new_numb_doc.push_back(doc);
                    break;
                }
            }
        }
        else{
            if(doc != this->numb_doc.at(doc)){

                for(size_t again_doc2 = 0; again_doc2 < this->docs.size(); again_doc2++){
                    if(this->numb_doc.at(again_doc2) == doc){
                        new_docs.push_back(this->docs.at(again_doc2));
                        new_numb_doc.push_back(doc);

                        break;
                    }
                }
            }
            else {
                new_docs.push_back(this->docs.at(doc));
                new_numb_doc.push_back(doc);
            }
        }
    }
    std::copy(new_docs.begin(), new_docs.end(), this->docs.begin());
    std::copy(new_numb_doc.begin(), new_numb_doc.end(), this->numb_doc.begin());

}
std::vector<Entry> InvertedIndex::getWordCount(const std::string& word) //
{
    /* Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов*/
    std::vector<Entry> rec_entry;
    for(size_t numb_doc = 0; numb_doc < this->docs.size(); numb_doc++){
        std::stringstream ss(this->docs[numb_doc]);
        std::string word_sep;
        bool get_rec = false;
        while (ss >> word_sep) { // Extract word from the stream.

            if(word_sep == word){
                if(rec_entry.empty()){
                    Entry new_rec;
                    new_rec.doc_id = numb_doc;
                    new_rec.count = 1;
                    rec_entry.push_back(new_rec);
                }
                else{
                    for(auto& records : rec_entry) {
                        if(records.doc_id == numb_doc){
                            records.count++;
                            get_rec = true;
                            break;
                        }
                    }
                    if(!get_rec){
                        Entry new_rec;
                        new_rec.doc_id = numb_doc;
                        new_rec.count = 1;
                        rec_entry.push_back(new_rec);
                    }
                    get_rec = false;
                }
            }
        }
    }
    return rec_entry;
}
void InvertedIndex::showDocs(size_t max_doc)
{
    size_t ind = 0;
    for(const auto& doc : this->docs) {
        std::cout << ind << ":" << this->numb_doc[ind] << ":\t" << doc << std::endl; //
        ind++;
        if(ind>=max_doc && max_doc!=0)break;
    }
}
