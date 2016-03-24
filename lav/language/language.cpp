#include "language.h"
#include <iostream>
#include <cstdlib>  //for exit
#include <algorithm>  //for find
#include <cmath>  //for std::abs, std::pow
using std::ifstream;

vector<string> Language::loadExcludeWord(string excludeFile, string includeFile, bool needExclude, bool needInclude) {
    vector<string> excludeVec;
    //exclude stop words
    if (needExclude) {
        ifstream excludeWords(excludeFile.c_str(), std::ios::in);
        if (!excludeWords) {
            std::cout << excludeFile << " open filed!" << std::endl;
            exit(1);
        }
       string excludeWord;
       while (getline(excludeWords, excludeWord, ',')) {
           excludeVec.push_back(excludeWord);
        }
    }
    //include operator words, that means to remove then from excludeVec if exists.
    if (needInclude) {
        ifstream includeWords(includeFile.c_str(), std::ios::in);
        if (!includeWords) {
            std::cout << includeFile << " open error!" << std::endl;
            exit(1);
        }
        string includeWord;
        while (getline(includeWords, includeWord, ',')) {
            vector<string>::iterator it = std::find(excludeVec.begin(), excludeVec.end(), includeWord);
            if (it != excludeVec.end()) {
                excludeVec.erase(it);
            }
        } 
    }
    return excludeVec;
}

vector<double> Language::getSentenceVec(Word2Vec<std::string> model, const vector<string>& sentence, const vector<string>& excludeVec,
                               bool needExclude) {
    vector<double> sentenceVec;
    int modelSize = model.word_vector_size();
    //initialize sentenceVec with 0 in the size of wordVec
    for (int i = 0; i < modelSize; i++) {
        sentenceVec.push_back(0);
    }

    for (auto &word : sentence) {
        //need exclude and the word isn't in the list of exclude words.
        if (needExclude && (find(excludeVec.begin(), excludeVec.end(), word) == excludeVec.end())) {
            const vector<float> wordVec = model.word_vector(word);
            if (wordVec.size() == 0) {
                OOV++;
                continue;
            }
            //std::cout << sentenceVec.size() << "\t" << wordVec.size() << std::endl;
            if (sentenceVec.size() != wordVec.size()) {
                std::cout << "wrong word vector maybe!" << std::endl;
                exit(1);
            }
            else {
                for (int i = 0; i < sentenceVec.size(); i++) {
                    sentenceVec[i] += wordVec[i];
                }
            }
        }
    }
    return sentenceVec;
}

double Language::getSentencePenalty(const int tokenCnt, const int avgCnt) {
    double result;
    auto maxCnt = [](const int a, const int b) {if (a >= b) return a; return b;};
    result = 1 - (std::abs(tokenCnt - avgCnt)) / maxCnt(tokenCnt, avgCnt);
    return result;
}

double Language::getCosSimilarity(const vector<double>& baseVec, const vector<double>& targetVec) {
    double cosDist = 0;
    if (baseVec.size() != targetVec.size()) {
        std::cout << baseVec.size() << "\t" << targetVec.size() << std::endl;
        std::cout << "the vectors are not in the same size, cannot calculate cos distance" << std::endl;
        exit(1);
    }
    else {
        double dotProduct = 0, baseNorm = 0, targetNorm = 0;
        for (int i = 0; i < baseVec.size(); i++) {
            dotProduct += baseVec[i] * targetVec[i];
            baseNorm += std::pow(baseVec[i], 2);
            targetNorm += std::pow(targetVec[i], 2);
        }
        baseNorm = std::sqrt(baseNorm);
        targetNorm = std::sqrt(targetNorm);
        cosDist = dotProduct / (baseNorm * targetNorm);
    }
    return cosDist;
}


vector<string> Language::sentenceTokenizer(string sentence) {
    vector<string> words;
    std::istringstream tokens(sentence);
    string word;
    while (getline(tokens, word, ' ')) {
        words.push_back(word);
    }
    return words;
}