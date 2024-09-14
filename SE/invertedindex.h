#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <mutex>
//void thread_proc(InvertedIndex* indx, int tnum, string doc);
#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H
struct Entry{
public:
    size_t doc_id, count;
    bool operator==(const Entry & rhs) const {
        return (doc_id == rhs.doc_id) && (count == rhs.count);
    }
};

class InvertedIndex
{
public:
    InvertedIndex();
    //void thread_proc(int tnum, std::string doc);
    void UpdateDocumentBase(std::vector<std::string> input_docs, int max_doc=0);
    //void UpdateDocumentBase(std::string docs, int max_doc);
    void show_freq_dict();
    void UpdateRequests(std::vector<std::string> input_req);
    //int sum(int a, int b);
    /* Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word); //
    void add_doc(std::vector<std::string> docs, size_t doc_max=0);//, int numb);
    void show_docs(size_t doc_max=0); //std::vector<std::string> docs,
    void Calc_freq_dict(std::string input_doc, int numb_doc);
    //std::vector<std::string> docs; //массив строк, это тексты
    std::vector<int> numb_doc;
    std::vector<std::string> docs;
    //std::mutex mut;
private:

    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::vector<std::string> request;

};
//static void UpdateDocumentBaseOne(std::string input_doc, int numb_doc);
//static int asd=0;
#endif // INVERTEDINDEX_H
