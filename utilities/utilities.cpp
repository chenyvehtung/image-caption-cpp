#include "utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>  //for exit
#include <string>  //for std:stod

map< string, vector<double> > utilities::vggLexer(string filename) {
    map< string, vector<double> > idToFeature;
    ifstream inputFile(filename.c_str(), std::ios::in);
    if (!inputFile) {
        cout << filename << " Open Error!" << endl;
        exit(1);
    }
    string row;
    while (getline(inputFile, row)) {
        vector<double> dFeatures;
        if (row.empty())  //ignore the empty line
            continue;

        std::istringstream rowTokens(row);
        string id, token;
        int cnt = 0;
        while (getline(rowTokens, token, '\t')) {
            if (cnt == 0) {
                id = token;
                cnt++;
                //cout << id << "\n";
            }
            else {
                std::istringstream feautures(token);
                string feauture;
                while (getline(feautures, feauture, ',')) {
                    dFeatures.push_back(std::atof(feauture.c_str()));
                }
                idToFeature[id] = dFeatures;
            }
        }
    }
    return idToFeature;
}

vector<utilities::DataArray> utilities::dataLoad(string filename, string type, map< string, vector<double> > vggFeatures) {
    vector<utilities::DataArray> dataArray;
    ifstream inputFile(filename.c_str(), std::ios::in);
    if (!inputFile) {
        cout << filename << " Open Error!" << endl;
        exit(1);
    }
    
    string line;
    string keys[20];
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
            index = -1;
            utilities::DataArray dataItem;
            while (getline(lineStream, token, '\t')) {
                index++;
                if (keys[index] == "id") {
                    dataItem.id = token;
                }
                else if (keys[index] == "file_path") {
                    dataItem.file_path = token;
                }
                else if (keys[index] == "file_name") {
                    dataItem.file_name = token;
                }
                else if (keys[index] == "url") {
                    dataItem.url = token;
                }
                else if (keys[index] == "sentences") {
                    std::istringstream tokenStream(token);
                    string sentence;
                    while (getline(tokenStream, sentence, ',')) {
                        dataItem.sentences.push_back(sentence);
                    }
                }
                else {
                    continue;
                }
            }
            dataItem.features = vggFeatures[dataItem.id];
            if (type == "test") {
                dataItem.url = "http://tasviret.cs.hacettepe.edu.tr/dataset/MSCOCO/test2014/" 
                                     + dataItem.file_name;
            }
            dataArray.push_back(dataItem);
        }
    }
    return dataArray;
}

