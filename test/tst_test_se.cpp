#include <QtTest>
#include "../SE/invertedindex.h"
#include "../SE/searchserver.h"
#include <string>
#include <vector>

using namespace std;
// add necessary includes here

class test_se : public QObject
{
    Q_OBJECT

public:
    test_se();
    ~test_se();

private slots:
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
    void test_case5();
};

test_se::test_se()
{

}

test_se::~test_se()
{

}

void TestInvertedIndexFunctionality(const vector<string>& docs,
                                    const vector<string>& requests,
                                    const std::vector<vector<Entry>>& expected)
{
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }

    QVERIFY(result == expected); //ASSERT_EQ(result, expected);
}

void test_se::test_case1()
{
    const std::vector<string> docs = { //const
                "london is the capital of great britain",
                "big ben is the nickname for the Great bell of the striking clock"
            };
            const std::vector<string> requests = {"london", "the"}; //const
            const std::vector<std::vector<Entry>> expected = { //const
            {
                {0, 1}
            }, {
                {0, 1}, {1, 3}
            }
            };
            TestInvertedIndexFunctionality(docs, requests, expected);
}
void test_se::test_case2(){ //TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuchino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        }, {
            {0, 3}, {1, 2}, {2, 5}
        }, {
            {3, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
void test_se::test_case3(){ //TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
        {
        }, {
            {1, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
void test_se::test_case4(){
    const vector<string> docs = {
    "milk milk milk milk water water water",
    "milk water water",
    "milk milk milk milk milk water water water water water",
    "americano cappuccino"
    };
    const vector<string> request = {"milk water", "sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
    {
        {2, 1.0},
        {0, 0.7},
        {1, 0.3}
    },
        {
        }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    //int doc_max=0;
    std::vector<vector<RelativeIndex>> result = srv.search(request);//, doc_max, 5);


    QVERIFY(result == expected); //ASSERT_EQ(result, expected);
}
void test_se::test_case5(){
    const vector<string> docs = {
    "london is the capital of great britain",//0
    "paris is the capital of france",//1
    "berlin is the capital of germany",//2
    "rome is the capital of italy",//3
    "madrid is the capital of spain",//4
    "lisboa is the capital of portugal",//5
    "bern is the capital of switzerland",//6
    "moscow is the capital of russia",//7
    "kiev is the capital of ukraine",//8
    "minsk is the capital of belarus",//9
    "astana is the capital of kazakhstan",//10
    "beijing is the capital of china",//1
    "tokyo is the capital of japan",//12
    "bangkok is the capital of thailand",//14
    "welcome to moscow the capital of russia the third rome",//14
    "amsterdam is the capital of netherlands",//15
    "helsinki is the capital of finland",//16
    "oslo is the capital of norway",//17
    "stockholm is the capital of sweden",//18
    "riga is the capital of latvia",//19
    "tallinn is the capital of estonia",//20
    "warsaw is the capital of poland",//21
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
    {
        {7, 1},
        {14, 1},
        {0, 0.666666687},
        {1, 0.666666687},
        {2, 0.666666687}
    }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);

    QVERIFY(result == expected); //ASSERT_EQ(result, expected);
}

QTEST_APPLESS_MAIN(test_se)

#include "tst_test_se.moc"
