#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <vector>
namespace utilities {
    
    struct NpyArray {        
        int id;
        string file_path;
        string file_name;
        string url;
        vector<float> feature;
        vector<string> sentences;
    };

    

};



#endif