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
        double* features;
        vector<string> sentences;
        void destruct() { delete[] features; }
    };

    map<string, double*> vggLexer(string filename);
    struct DataArray* dataLoad(string filename, string type, map<string, double*> vggFeatures);
};



#endif