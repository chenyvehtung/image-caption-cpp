#ifndef IMG_BLOCK_H_
#define IMG_BLOCK_H_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
using std::map;
using std::vector;
using std::string;
using std::cout;

class imgBlock {

public:
    imgBlock(vector<string> imgCaptions, string imgUrl, string imgName) {
        captions = imgCaptions;
        url = imgUrl;
        filename = imgName;
    }
    vector<string> captions;
    string url;
    string filename;  
    
    vector<string> split(const string& str, char delim) const {
        std::istringstream strStream(str);
        string token;
        vector<string> results;
        while (getline(strStream, token, delim)) {
            results.push_back(token);
        }
        return results;
    }

};



#endif