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
        double distance = 0.0;   //to store the distance between image feature for k means algorithm
    };

    map< string, vector<double> > vggLexer(string filename);
    vector<utilities::DataArray> dataLoad(string filename, string type, map< string, vector<double> > vggFeatures);
    vector<string> sentenceTokenizer(string sentence);
};



#endif