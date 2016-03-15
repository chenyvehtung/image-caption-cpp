#include "lav.h"
#include "../settings.h"
#include "language/language.h"
#include "vision/vision.h"
#include <map>
#include <chrono>  //for high_resolution_clock
#include <iostream>
#include <algorithm> //for std::sort

const string Lav::describeImg(const utilities::DataArray& queryItem,
             vector<utilities::DataArray>& candidateSet) {
    Language language;
    Vision vision;
    /*---------------get similar image--------------*/
    vector<utilities::DataArray> neighbors = vision.retrieveVisualSimilarImg(queryItem, candidateSet, Settings::FAR_NEIGHBOR_SIZE);
    //vector<string> closestNeighCaptions(neighbors[0].sentences);

    /*---------------remove outlier-----------------*/
    double maxDist = 0;
    double minDist = 0;
    neighbors = vision.removeOutLier(neighbors, Settings::EPSILON(), Settings::NEIGHBOR_SIZE, maxDist, minDist);

    /*---------------load language model word2vec------------*/
    Word2Vec<std::string> model;
    //auto cstart = chrono::high_resolution_clock::now();
    model.load(Settings::WORD2VEC_BIN());
    //auto cend = chrono::high_resolution_clock::now();
    //cout << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
    //    << " seconds cost." << endl;

    vector<string> excludeWords = language.loadExcludeWord(Settings::EXCLUDE_FILE(), Settings::INCLUDE_FILE(), 
                                                        Settings::EX_STOP_WORDS, Settings::IN_OPERA_WORDS);
    int avgCnt = 0, totalNum = 0;
    if (Settings::USE_SENTENCE_LEN_PENALTY) {
        for (auto& neighbor : neighbors) {
            for (auto& caption : neighbor.sentences) {
                totalNum++;
                avgCnt += language.sentenceTokenizer(caption).size();
            }
        }
        avgCnt /= totalNum;
    }

    /*--------------calculate query img caption vector------------*/
    vector<double> queryImg;
    vector<utilities::CaptionArray> captionSet;
    bool flag = true;
    for (auto& neighbor : neighbors) {
        double visSimScore = 1.0;
        if (Settings::USE_VISUAL_SIMILAR_SCORE)
            visSimScore = vision.visualSimilarity(neighbor.distance, maxDist, minDist);
        for (auto& caption : neighbor.sentences) {
            utilities::CaptionArray captionItem;
            vector<string> captionTokens = language.sentenceTokenizer(caption);
            captionItem.caption = caption;
            captionItem.sentence_vec = language.getSentenceVec(model, captionTokens, excludeWords, Settings::EX_STOP_WORDS);
            if (flag) {
                flag = false;
                for (int i = 0; i < captionItem.sentence_vec.size(); i++)
                    queryImg.push_back(0.0);
            }
            double penalty = 1.0;
            if (Settings::USE_SENTENCE_LEN_PENALTY)
                penalty = language.getSentencePenalty(captionTokens.size(), avgCnt);
            for (int j = 0; j < captionItem.sentence_vec.size(); j++) {
                queryImg[j] += captionItem.sentence_vec[j] * visSimScore * penalty;
            }
            captionSet.push_back(captionItem);
        }//end of 5 caption for one neighbor
    }//end of all neighbors
    totalNum = captionSet.size();
    for (int k = 0; k < queryImg.size(); k++) {
        queryImg[k] /= totalNum;
    }

    /*--------------rerank by cos distance------------------*/
    for (auto& captionItem : captionSet) {
        captionItem.cos_distance = language.getCosSimilarity(queryImg, captionItem.sentence_vec);
    }
    std::sort(captionSet.begin(), captionSet.end());

    OOV = language.OOV;

    return captionSet.begin()->caption;
}
