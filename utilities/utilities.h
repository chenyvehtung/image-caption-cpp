#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <vector>
#include <map>
#include <string>
using namespace std;

namespace utilities {
    
    struct DataArray {        
        string id;
        string file_path;
        string file_name;
        string url;
        vector<double> features;
        vector<string> sentences;
        double distance = 0.0;   //to store the distance between image feature for first rank

        bool operator<(const DataArray& a) const {
            return distance < a.distance;
        }
    };

    struct CaptionArray {
        vector<double> sentence_vec;
        double cos_distance = 0.0; //to store the cos distance between the sentence vector and query img vector
        string caption;

        bool operator < (const CaptionArray& a) const {
            return cos_distance < a.cos_distance;
        }
    };
    
    map< string, vector<double> > vggLexer(string filename);
    vector<utilities::DataArray> dataLoad(string filename, string type, map< string, vector<double> > vggFeatures);
};



#endif