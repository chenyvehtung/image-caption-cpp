#include "settings.h"
#include "utilities.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

int main() {
    vector<utilities::DataArray> trainData, valData, testData;
    map< string, vector<double> > vggFeature;

    vggFeature = utilities::vggLexer(Settings::DATAPATH() + Settings::FEATUREFILE());
    cout << "load vgg success!" << endl;
    trainData = utilities::dataLoad(Settings::DATAPATH() + Settings::TRAINFILE(), "train", vggFeature);
    valData = utilities::dataLoad(Settings::DATAPATH() + Settings::VALFILE(), "val", vggFeature);
    testData = utilities::dataLoad(Settings::DATAPATH() + Settings::TESTFILE(), "test", vggFeature);

    
    return 0;
}