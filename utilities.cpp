#include "utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>


map<string, double*> utilities::vggLexer(string filename) {
    map<string, double*> idToFeature;
    ifstream inputFile(filename.c_str(), ios::_Nocreate);
    if (!inputFile) {
        cout << filename << " Open Error!" << endl;
        exit(1);
    }
    string row;
    while (getline(inputFile, row)) {
        double dFeatures[4100];
        if (row.empty())  //ignore the empty line
            continue;

        std::istringstream rowTokens(row);
        string id, token;
        int cnt = 0;
        while (getline(rowTokens, token, '\t')) {
            if (cnt == 0) {
                id = token;
                cnt++;
                cout << id << "\n";
            }
            else {
                std::istringstream feautures(token);
                string feauture;
                int featureCnt = 0;
                while (getline(feautures, feauture, ',')) {
                    dFeatures[featureCnt] = stod(feauture);
                    featureCnt++;
                }
                idToFeature[id] = dFeatures;
            }
        }
    }
    return idToFeature;
}

struct utilities::DataArray* utilities::dataLoad(string filename, string type, map<string, double*> vggFeatures) {
    struct utilities::DataArray* dataArray = {};
    int cnt = 0;
    ifstream inputFile(filename.c_str(), ios::_Nocreate);
    if (!inputFile) {
        cout << filename << " Open Error!" << endl;
        exit(1);
    }
    
    string line;
    string* keys = {};
    int index;
    while (getline(inputFile, line)) {
        //get column title and store in keys
        if (line[0] == '#') {
            index = 0;
            std::istringstream lineStream(line.substr(1));
            string key;
            while (getline(lineStream, key, '\t')) {
                keys[index] = key;
                index++;
            }
        }
        else {
            std::istringstream lineStream(line);
            string token;
            index = 0;
            while (getline(lineStream, token, '\t')) {
                if (keys[index] == "id") {
                    dataArray[cnt].id = token;
                }
                else if (keys[index] == "file_path") {
                    dataArray[cnt].file_path = token;
                }
                else if (keys[index] == "file_name") {
                    dataArray[cnt].file_name = token;
                }
                else if (keys[index] == "url") {
                    dataArray[cnt].url == token;
                }
                else if (keys[index] == "sentences") {
                    std::istringstream tokenStream(token);
                    string sentence;
                    while (getline(tokenStream, sentence, ',')) {
                        dataArray[cnt].sentences.push_back(sentence);
                    }
                }
                else {
                    index++;
                    continue;
                }
                index++;
            }
            dataArray[cnt].features = vggFeatures[dataArray[cnt].id];
            if (type == "train") {
                dataArray[cnt].url = "http://tasviret.cs.hacettepe.edu.tr/dataset/MSCOCO/test2014/" 
                                     + dataArray[cnt].file_name;
            }
            cnt++;
        }
    }
    return dataArray;
}