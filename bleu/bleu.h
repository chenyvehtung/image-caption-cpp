#ifndef BLEU_H_
#define BLEU_H_

#include <map>
#include <vector>
using std::map;
using std::string;
using std::vector;
class Bleu {

public:

    size_t maxGramNum;
    size_t refNum;
    map<string, size_t> candStats;
    vector< map<string, size_t> > refStats;
    vector<size_t> candLengthSum;
    size_t bestRefLengthSum;
    double BrePenalty;

    Bleu();
    Bleu(size_t gram, size_t ref);

    void addSentences(const string& candSentence, const vector<string>& refSentences);
    void collectStats(string sentence);
    vector<string> sentenceTokenizer(string sentence);
    string makeNGram(const vector<string>& sentence, size_t offset, size_t gramNum);
    void saveClippedHits();
    double getBleuValue();
    void resetData();


};


#endif