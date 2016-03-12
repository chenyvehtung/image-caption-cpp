#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include "../../utilities/utilities.h"
#include "../../lib/word2vec-cpp/word2vec.h"
#include <vector>

class Language {

public:
    vector<string> loadExcludeWord(string excludeFile, string includeFile, bool needExclude, bool needInclude);
    vector<double> getSentenceVec(Word2Vec<std::string> model, const vector<string>& sentence, const vector<string>& excludeVec,
                                  bool needExclude);

    /*Compute the penalty score using the length of the candidate sentence
    according to the average token count.*/
    double getSentencePenalty(const int tokenCnt, const int avgCnt);

    /*Compute the cosine similarity between two vectors based on the angular cosine distance
    return range -1 to 1, where 1 means two vectors are identical,
    -1 means reverse!*!, 0 means vectors are orthogonal
    where cosine(A,B) = dot(A,B) / ( || A || * || B || )*/
    double getCosSimilarity(const vector<double>& baseVec, const vector<double>& targetVec);

private:
    vector<string> sentenceTokenizer(string sentence);
};

#endif