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
    vector<size_t> candLengthSum;
    vector<size_t> candClipSum;
    size_t bestRefLengthSum;

    Bleu();
    Bleu(size_t gram, size_t ref);
    void setGram(size_t gram);
    void setRef(size_t ref);

    void addSentences(const string& candSentence, const vector<string>& refSentences);
    map<string, size_t> collectStats(const vector<string>& words, size_t gramNum);
    string getNextNGram(const vector<string>& sentence, size_t offset, size_t gramNum);
    void saveClippedHits(const vector<string>& candSentenWords, const vector< vector<string> >& refSentensWords, size_t gramNum);
    double getBleuValue();
    vector<string> sentenceTokenizer(string sentence);

};


#endif