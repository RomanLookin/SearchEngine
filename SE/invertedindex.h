#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <mutex>
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

    void updateDocumentBase(std::vector<std::string> input_docs, int max_doc=0);

    void showFreqDict();
    //void updateRequests(std::vector<std::string> input_req);

    /* Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> getWordCount(const std::string& word);
    void addDoc(std::vector<std::string> docs, size_t max_doc=0);
    void showDocs(size_t max_doc=0);
    void calcFreqDict(std::string input_doc, int numb_doc);

    std::vector<int> numb_doc;
    std::vector<std::string> docs;

private:

    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::vector<std::string> request;

};
#endif // INVERTEDINDEX_H
