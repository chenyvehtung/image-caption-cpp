#include "bleu.h"
#include <sstream>  //for istringstream
#include <math.h> //for exp, log
#include <stdlib.h> //for abs
#include <iostream> //for std::cout
#include <cstdlib>  //for exit
#include <climits>  //for INT_MAX

Bleu::Bleu() {
    maxGramNum = 1;
    refNum = 1;
    candLengthSum.push_back(0);
    candClipSum.push_back(0);
    bestRefLengthSum = 0;
}

Bleu::Bleu(size_t gram, size_t ref){
    maxGramNum = gram;
    refNum = ref;
    for (size_t i = 0; i < maxGramNum; i++) {
        candLengthSum.push_back(0);
        candClipSum.push_back(0);
    }
    bestRefLengthSum = 0;
}

string Bleu::getNextNGram(const vector<string>& words, size_t offset, size_t gramNum){
    string nextNGram = "";
    for (size_t i = offset; i < offset + gramNum; i++) {
        nextNGram += words[i] + " ";
    }
    nextNGram.erase(nextNGram.end() - 1); 
    return nextNGram;
}

map<string, size_t> Bleu::collectStats(const vector<string>& words, size_t gramNum) {
    map<string, size_t> statistics;
    for (size_t offset = 0; offset <= (size_t)words.size() - gramNum; offset++) {
        string nextNGram = getNextNGram(words, offset, gramNum);
        if (statistics.find(nextNGram) != statistics.end()) 
            statistics[nextNGram] = 1;
        else
            statistics[nextNGram] += 1;
    }
    return statistics;
}

void Bleu::saveClippedHits(const vector<string>& candSentenWords, const vector< vector<string> >& refSentensWords,
    size_t gramNum) {
    map<string, size_t> candStats = collectStats(candSentenWords, gramNum);
    vector< map<string, size_t> > refStats;
    for (auto& refSentenWords : refSentensWords) {
        map<string, size_t> refStat = collectStats(refSentenWords, gramNum);
        refStats.push_back(refStat);
        refStat.clear();
    }
    for (map<string, size_t>::iterator it = candStats.begin(); it != candStats.end(); it++) {
        size_t minLength = it->second;
        for (auto& refStat : refStats) {
            map<string, size_t>::iterator findIt = refStat.find(it->first);
            if (findIt != refStat.end() ) 
                minLength = (findIt->second < minLength ? findIt->second : minLength);
        }
        candClipSum[gramNum - 1] += minLength;
    }
}

void Bleu::addSentences(const string& candSentence, const vector<string>& refSentences) {
    if (refSentences.size() != refNum) {
        std::cout << "The reference sentences are not in the right number" << std::endl;
        exit(1);
    }

    vector<string> candSentenWords = sentenceTokenizer(candSentence);
    vector< vector<string> > refSentensWords;
    size_t bestRefLength = INT_MAX;
    for (auto& refSentence : refSentences) {
        if (refSentence == "") {
            std::cout << "The reference sentences is empty." << std::endl;
            exit(1);
        }
        vector<string> refSentenWords = sentenceTokenizer(refSentence);
        refSentensWords.push_back(refSentenWords);
        if (abs((int)candSentenWords.size() - (int)refSentenWords.size()) < bestRefLength) {
            bestRefLength = refSentenWords.size();
        }
    }
    bestRefLengthSum += bestRefLength;

    for (size_t gramNum = 1; gramNum <= maxGramNum; gramNum++) {
        saveClippedHits(candSentenWords, refSentensWords, gramNum);
        candLengthSum[gramNum - 1] += candSentenWords.size() - gramNum + 1;
    }
}

vector<string> Bleu::sentenceTokenizer(string sentence) {
    vector<string> words;
    std::istringstream sentenStream(sentence);
    string word;
    while (getline(sentenStream, word, ' ')) {
        words.push_back(word);
    }
    return words;
}

double Bleu::getBleuValue() {
    double brePenalty = 1;
    if (candLengthSum[0] <= bestRefLengthSum) 
        brePenalty = exp(1 - (double)bestRefLengthSum / (double)candLengthSum[0]);

    std::cout << "\n\nbestRefLengthSum: " << (double)bestRefLengthSum << std::endl;
    std::cout << "candLengthSum[0]: " << (double)candLengthSum[0] << std::endl;
    std::cout << "brePenalty: " << brePenalty << std::endl;

    double weightN = 1.0 / (double)maxGramNum;
    std::cout << "weighN: " << weightN << std::endl;

    double modifyPrecisionSum = 0; 
    for (size_t gramNum = 1; gramNum <= maxGramNum; gramNum++) {
        modifyPrecisionSum += weightN * log((double)candClipSum[gramNum-1] / (double)candLengthSum[gramNum-1]);

        std::cout << "candClipSum[" << gramNum - 1 << "]: " << candClipSum[gramNum - 1] 
            << "\ncanLengthSum[" << gramNum - 1 << "]: " << candLengthSum[gramNum - 1]
            << "\nmodifyPrecisionSum: " << modifyPrecisionSum << std::endl;
    }

    double bleu = brePenalty * exp(modifyPrecisionSum);
    return bleu;
}