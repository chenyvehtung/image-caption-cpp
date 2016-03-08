#ifndef VISION_H_
#define VISION_H_

#include "utilities.h"
#include <vector>

class Vision {

public:
    vector<utilities::DataArray> retrieveVisualSimilarImg(const utilities::DataArray& queryItem, 
                               vector<utilities::DataArray>& candidateSet, int numNearNeigb);
    vector<utilities::DataArray> removeOutLier(const vector<utilities::DataArray>& candidateSet, double epsilon, 
                               int numNearNeigb, double& maxDist, double& minDist);
    double visualSimilarity(double queryDist, double maxDist, double minDist);


private:
    bool comparator(const std::pair<double, int>& left, const std::pair<double, int>& right);
    double euclideanDist(const vector<double>& dataFeature, const vector<double>& queryFeature);
};


#endif