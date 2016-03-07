#include "settings.h"
#include "utilities.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

int main() {
    struct utilities::DataArray *trainData, *valData, *testData;
    map<string, double*> vggFeature;

    vggFeature = utilities::vggLexer(Settings::DATAPATH() + Settings::FEATUREFILE());
    cout << "load vgg success!" << endl;
    trainData = utilities::dataLoad(Settings::DATAPATH() + Settings::TRAINFILE(), "train", vggFeature);
    //valData = utilities::dataLoad(Settings::DATAPATH() + Settings::VALFILE(), "val", vggFeature);
    //testData = utilities::dataLoad(Settings::DATAPATH() + Settings::TESTFILE(), "test", vggFeature);

    cout << trainData[0].file_name << " " << trainData[0].file_path << " " << trainData[0].url 
         << " " << trainData[0].id << endl;
    cout << trainData[0].features[0] << endl;
    for (vector<string>::iterator it = trainData[0].sentences.begin(); it != trainData[0].sentences.end(); it++) {
        cout << *it << endl;
    }
    
    return 0;
}