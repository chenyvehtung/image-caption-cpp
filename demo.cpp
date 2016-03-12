#include "settings.h"
#include "utilities/utilities.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

int main() {
    vector<utilities::DataArray> trainData, valData, testData;
    map< string, vector<double> > vggFeature;

    vggFeature = utilities::vggLexer(Settings::DATAPATH() + Settings::FEATURE_FILE());
    cout << "load vgg success!" << endl;
    trainData = utilities::dataLoad(Settings::DATAPATH() + Settings::TRAIN_FILE(), "train", vggFeature);
    valData = utilities::dataLoad(Settings::DATAPATH() + Settings::VAL_FILE(), "val", vggFeature);
    testData = utilities::dataLoad(Settings::DATAPATH() + Settings::TEST_FILE(), "test", vggFeature);

    
    return 0;
}