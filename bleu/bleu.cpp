#include "bleu.h"
#include <sstream>      //for istringstream
#include <math.h>       //for exp, log
#include <stdlib.h>     //for abs
#include <iostream>     //for std::cout
#include <cstdlib>      //for exit
#include <climits>      //for INT_MAX
#include <cctype>       //for tolower
#include <algorithm>    //for transform

#define BLEU_DEBUG false //for showing debug message

Bleu::Bleu() {
    init(1, 1);
}

Bleu::Bleu(size_t gram, size_t ref){
    init(gram, ref);
}

void Bleu::init(size_t gram, size_t ref) {
    maxGramNum = gram;
    refNum = ref;
    candLengthSum.clear();
    candClipSum.clear();
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
        /*if (statistics.find(nextNGram) != statistics.end()) //find it
            statistics[nextNGram] += 1;
        else*/
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

    if (BLEU_DEBUG) {
        std::cout << "\nGramNum: " << gramNum 
                  << "\ncandStats: " << std::endl;
        for (auto& candStat : candStats) 
            std::cout << candStat.first << " : " << candStat.second << std::endl;
        int cnt = 0;
        for (auto& refStat : refStats) {
            std::cout << "refStats[" << cnt << "]: " << std::endl;
            for (auto& item : refStat)
                std::cout << item.first << " : " << item.second << std::endl;
            cnt++;
        }
    }

    for (map<string, size_t>::iterator it = candStats.begin(); it != candStats.end(); it++) {
        size_t minLength = it->second;
        size_t maxRef = 0;
        for (auto& refStat : refStats) {
            map<string, size_t>::iterator findIt = refStat.find(it->first);
            if (findIt != refStat.end() ) {
                maxRef = (findIt->second > maxRef ? findIt->second : maxRef);
            }
        }
        minLength = (minLength < maxRef ? minLength : maxRef);
        candClipSum[gramNum - 1] += minLength;

        if (BLEU_DEBUG) {
            std::cout << it->first
                << "\tcandidateLength: " << it->second
                << "\tmaxRef: " << maxRef 
                << "\tcandClipSum[" << gramNum - 1 << "]: " << candClipSum[gramNum - 1] << std::endl;
        }
    }
}

void Bleu::addSentences(const string& candSentence, const vector<string>& refSentences) {
    if (refSentences.size() != refNum) {
        std::cout << "The reference sentences are not in the right number" << std::endl;
        exit(1);
    }

    vector<string> candSentenWords = sentenceTokenizer(candSentence);
    if (candSentenWords.size() < maxGramNum) {
        std::cout << "The candidate's words are less than the masx gram number" << std::endl;
        exit(1);
    }

    vector< vector<string> > refSentensWords;
    size_t bestRefLength = INT_MAX;
    for (auto& refSentence : refSentences) {
        if (refSentence == "") {
            std::cout << "The reference sentences is empty." << std::endl;
            exit(1);
        }
        vector<string> refSentenWords = sentenceTokenizer(refSentence);
        refSentensWords.push_back(refSentenWords);
        if (abs((int)candSentenWords.size() - (int)refSentenWords.size()) < (int)bestRefLength) {
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
        transform(word.begin(), word.end(), word.begin(), tolower);
        words.push_back(word);
    }
    return words;
}

double Bleu::getBleuValue() {
    double brePenalty = 1;
    if (candLengthSum[0] <= bestRefLengthSum) 
        brePenalty = exp(1 - (double)bestRefLengthSum / (double)candLengthSum[0]);

    double weightN = 1.0 / (double)maxGramNum;

    if (BLEU_DEBUG) {
        std::cout << "\nbestRefLengthSum: " << (double)bestRefLengthSum
            << "\ncandLengthSum[0]: " << (double)candLengthSum[0]
            << "\nbrePenalty: " << brePenalty 
            << "\nweighN: " << weightN << std::endl;
    }

    double modifyPrecisionSum = 0; 
    for (size_t gramNum = 1; gramNum <= maxGramNum; gramNum++) {
        if (candClipSum[gramNum - 1] == 0)
            candClipSum[gramNum - 1] = 1;
        modifyPrecisionSum += weightN * log((double)candClipSum[gramNum-1] / (double)candLengthSum[gramNum-1]);

        if (BLEU_DEBUG) {
            std::cout << "\ncandClipSum[" << gramNum - 1 << "]: " << candClipSum[gramNum - 1] 
                << "\ncanLengthSum[" << gramNum - 1 << "]: " << candLengthSum[gramNum - 1]
                << "\nmodifyPrecisionSum: " << modifyPrecisionSum 
                << "\nmodifyPrecision: " << (double)candClipSum[gramNum-1] / (double)candLengthSum[gramNum-1]
                << std::endl;
        }
    }

    double bleu = brePenalty * exp(modifyPrecisionSum);
    return bleu;
}