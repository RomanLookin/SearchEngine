#include "invertedindex.h"
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
//#include <thread>
#include <mutex>
#include <vector>
std::mutex mtx;

void thread_proc(InvertedIndex* indx, int tnum, std::string doc) {
//void InvertedIndex::thread_proc(int tnum, std::string doc) {
    //for(;;) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << tnum << std::endl;
            indx->Calc_freq_dict(doc, tnum);
            //this->UpdateDocumentBaseOne(doc);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //}
}
//std::mutex mtx;
InvertedIndex::InvertedIndex()
{

}
void InvertedIndex::Calc_freq_dict(std::string input_doc, int numb_doc)
{
    //std::mutex mutex;
    //std::lock_guard<std::mutex> lock(mutex); //mutex.lock();
    //std::cout << "ID thread: " << std::this_thread::get_id() << "<" << numb_doc << ">" << std::endl;
    this->docs.push_back(input_doc);
    //if()
    this->numb_doc.push_back(numb_doc);// +
    //разбираем строку по словам.
    //если слово есть в словаре, то добавляем 1 к счетчику слов
    //если нет, то добавляем новую пару

    std::stringstream ss(input_doc);
    std::string word;

    while(ss >> word){ // Extract word from the stream.
        if(this->freq_dictionary.count(word) == 0){//если в словаре нет нужного слова
            Entry new_rec; //={k, 1};
            new_rec.doc_id = numb_doc;//this->docs.size()-1;//  k;
            new_rec.count = 1;
            std::vector<Entry> rec_entry;
            rec_entry.push_back(new_rec);
            this->freq_dictionary[word] = rec_entry;
        }
        else{//если в словаре есть нужное слово
            std::vector<Entry> rec_entry = this->freq_dictionary[word];
            bool get_word = false;
            for(size_t l =0; l< rec_entry.size(); l++){
                if(rec_entry[l].doc_id == numb_doc){//this->docs.size()-1){
                    rec_entry[l].count += 1;
                    get_word = true;
                }
            }
            if(!get_word){
                Entry new_rec;
                new_rec.doc_id = numb_doc;//this->docs.size()-1;//k;
                new_rec.count = 1;
                rec_entry.push_back(new_rec);
            }
            this->freq_dictionary[word] = rec_entry;
        }
         //std::cout << word << std::endl;

    }
    //mutex.unlock();


}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs, int max_doc)
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
        /*std::thread thr([&]()
        {
            this->thread_proc(i, doc);
        });*/
        threads.push_back(std::move(thr));



        //threads.emplace_back(std::move(thr));
        load_doc_count++;
        if(i == max_doc-1)break;
    }

    for(auto& thr : threads) {
        thr.join();
    }
    std::cout << "Load " << load_doc_count << " documents." << std::endl;
    std::vector<std::string> new_docs;
    std::vector<int> new_numb_doc;
    for(size_t l=0; l < this->docs.size();l++){
        if(new_docs.empty()){//this->numb_doc[l] != l or
            for(size_t k = 0; k < this->docs.size(); k++){
                if(this->numb_doc.at(k) == l){
                    new_docs.push_back(this->docs.at(k));
                    new_numb_doc.push_back(l);
                    break;
                }
            }
        }
        else{
            if(l != this->numb_doc.at(l)){
                //bool qqq=false;
                for(size_t k = 0; k < this->docs.size(); k++){
                    if(this->numb_doc.at(k) == l){
                        new_docs.push_back(this->docs.at(k));
                        new_numb_doc.push_back(l);
                        //qqq=true;
                        break;
                    }
                    //if(qqq)break;
                }

            }
            else {
                new_docs.push_back(this->docs.at(l));
                new_numb_doc.push_back(l);
            }

        }
        //this->docs = new_docs;
    }
    std::copy(new_docs.begin(), new_docs.end(), this->docs.begin());
    std::copy(new_numb_doc.begin(), new_numb_doc.end(), this->numb_doc.begin());

}
void InvertedIndex::show_freq_dict()
{
    //выводим словарь
    //std::vector<Entry> ggg;
    for(const auto& element : this->freq_dictionary){
        std::cout << element.first << "\t";// << element.second << std::endl;
        for(size_t k = 0; k < element.second.size(); k++){
            std::cout << "{" << element.second[k].doc_id << ", " << element.second[k].count << "}\t";
        }
        std::cout << std::endl;
    }
}
/*void InvertedIndex::UpdateRequests(std::vector<std::string> input_req)
{
    for(int l = 0; l < input_req.size(); l++){
        std::ifstream in;       // поток для чтения
        //in.setCodec("UTF-8");
        in.open(input_req[l]);  //"C:\\Roman\\search_engine\\config.json");
        //std::cout << "Read " << l+1 << " file." << std::endl;
        if (in.is_open())
        {
            //читаем строку
            int k=0;
            std::string line;
            while (std::getline(in, line))
            {
                //std::cout << k << ":" << line << std::endl;
                this->request.push_back(line);

                k++;
                //if(k>50)break;

            }
            in.close();
        }
        else{
            std::cout << "Cannot open file: " << input_req[l] << std::endl;
        }
    }

}
int InvertedIndex::sum(int a, int b)
{
    return a + b;
}*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) //
{
    /* Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов*/
    std::vector<Entry> rec_entry;
    for(size_t l = 0; l < this->docs.size(); l++){
        std::stringstream ss(this->docs[l]);//this->docs[this->numb_doc[l]]);
        std::string word_sep;
        bool get_rec = false;
        while (ss >> word_sep) { // Extract word from the stream.

            if(word_sep == word){
                if(rec_entry.empty()){
                    Entry new_rec; //={l, 1};
                    new_rec.doc_id = l;
                    new_rec.count = 1;
                    rec_entry.push_back(new_rec);
                }
                else{
                    for(auto& records : rec_entry) {
                        if(records.doc_id == l){
                            records.count++;
                            get_rec = true;
                            break;
                        }
                    }
                    if(!get_rec){
                        Entry new_rec;//={l, 1};
                        new_rec.doc_id = l;
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
void InvertedIndex::add_doc(std::vector<std::string> docs, size_t doc_max)//, int numb)
{
    size_t k = 0;
    for(auto& doc : docs) {
        this->docs.push_back(doc);
        k++;
        if(k>=doc_max && doc_max!=0)break;
    }
    std::cout << "Load " << k << " documents." << std::endl;
    //this->numb_doc.push_back(numb);
    //std::cout << "Added " << numb << " doc." << std::endl;
}
void InvertedIndex::show_docs(size_t doc_max) //std::vector<std::string> docs,
{
    size_t k = 0;
    //std::cout << "Docs size: " << this->docs.size() << std::endl;
    for(auto& doc : this->docs) {
        std::cout << k << ":" << this->numb_doc[k] << ":\t" << doc << std::endl; //
        //inv_index.add_docs(docs);
        k++;
        if(k>=doc_max && doc_max!=0)break;
    }
}
