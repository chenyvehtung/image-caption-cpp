#include "vision.h"
#include <cstdlib>  //for exit
#include <iostream> //for cout
#include <math.h>  //for sqrt, pow
#include <algorithm>  //for sort


vector<utilities::DataArray> Vision::retrieveVisualSimilarImg(const utilities::DataArray& queryItem,
    vector<utilities::DataArray>& candidateSet, int numNearNeigb) {
    for (auto& candidate : candidateSet) {
        candidate.distance = euclideanDist(candidate.features, queryItem.features);
    }
    //sort candidateSet according to euclidean distance 
    sort(candidateSet.begin(), candidateSet.end());
    //return the first numNearNeighb candidate
    vector<utilities::DataArray> kSimilarImg(candidateSet.begin(), candidateSet.begin() + numNearNeigb);

    int cnt = 0;
    for (auto& similarImg : kSimilarImg) 
        for (auto& sentence : similarImg.sentences) {
            std::cout << cnt << "\t" << sentence << std::endl;
            cnt++;
        }

    return kSimilarImg;
}

vector<utilities::DataArray> Vision::removeOutLier(const vector<utilities::DataArray>& candidateSet, double epsilon,
    int numNearNeigb, double& maxDist, double& minDist) {

    vector<utilities::DataArray> neighbors(candidateSet);
    sort(neighbors.begin(), neighbors.end());
    minDist = neighbors[0].distance;
    maxDist = neighbors[neighbors.size() - 1].distance;
    while (neighbors.size() > numNearNeigb) {
        if (neighbors.back().distance > (1 + epsilon) * minDist) {
            neighbors.pop_back();
        }
        //because we have sorted the neighbors from near to far, so the rest would be all matched condition
        else
            break;
    }
    return neighbors;
}

/* define the similarity between 2 img as "1 - (queryDist - minDist)/(maxDist - minDist)" */
double Vision::visualSimilarity(double queryDist, double maxDist, double minDist) {

    double constZ = maxDist - minDist;
    double similarity = 1 - (queryDist - minDist) / constZ;

    //fix 
    if (similarity == 0)
        similarity = 0.000001;  //prevent division issues
    if (similarity > 1) 
        similarity = 1;  //prevent floating point issues

    return similarity;
}


/*bool Vision::comparator(const std::pair<double, int>& left, const std::pair<double, int>& right) {
    return left.first < right.first;
}*/

bool Vision::distComparator(const utilities::DataArray& left, const utilities::DataArray& right) {
    return left.distance < right.distance;
}

double Vision::euclideanDist(const vector<double>& dataFeature, const vector<double>& queryFeature) {
    if (dataFeature.size() != queryFeature.size()) {
        std::cout << "the feature's size between dataset and query is not the same.\n";
        exit(1);
    }
    double sum = 0;
    for (int i = 0; i < dataFeature.size(); i++) {
        sum += pow((dataFeature[i]-queryFeature[i]), 2);
    }
    double sqrtSum = sqrt(sum);
    return sqrtSum;
}