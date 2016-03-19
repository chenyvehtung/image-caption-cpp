#include "settings.h"
#include "utilities/utilities.h"
#include "lav/lav.h"
#include "bleu/bleu.h"
#include <string>
#include <map>
#include <iostream>
#include <cstdlib> //for srand and rand
#include <time.h> //for time
#include <chrono>  //for high_resolution_clock
using namespace std;

int main() {
    vector<utilities::DataArray> trainData, valData, testData;
    map< string, vector<double> > vggFeature;
    utilities::DataArray queryData;
    vector<utilities::CaptionArray> queryCaption;
    Lav lav;

    auto cstart = chrono::high_resolution_clock::now();
    vggFeature = utilities::vggLexer(Settings::DATAPATH() + Settings::FEATURE_FILE());
    auto cend = chrono::high_resolution_clock::now();
    cout << "load vgg success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0 
         << " seconds cost." << endl;

    cstart = chrono::high_resolution_clock::now();
    trainData = utilities::dataLoad(Settings::DATAPATH() + Settings::TRAIN_FILE(), "train", vggFeature);
    valData = utilities::dataLoad(Settings::DATAPATH() + Settings::VAL_FILE(), "val", vggFeature);
    testData = utilities::dataLoad(Settings::DATAPATH() + Settings::TEST_FILE(), "test", vggFeature);
    cend = chrono::high_resolution_clock::now();
    cout << "load data success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
        << " seconds cost." << endl;

    /* append valData to the end of trainData */
    //trainData.insert(trainData.end(), valData.begin(), valData.end());

    /*count the number of sentences I got to make sure the program get the right input
    int cnt = 0;
    for (auto & item : trainData) {
        cnt += item.sentences.size();
    }
    std::cout << cnt << std::endl;*/

    int gram = 4, refer = 5;
    Bleu* bleu = new Bleu(gram, refer);
    for (int i = 0; i < 10; i++) {
        srand(time(NULL));
        //queryData = testData[rand() % testData.size()];
        queryData = valData[rand() % valData.size()];
        cstart = chrono::high_resolution_clock::now();
        queryCaption = lav.describeImg(queryData, trainData);
        cend = chrono::high_resolution_clock::now();
        cout << queryCaption[0].caption << "\nURL: " << queryData.url << "\nID: " << queryData.id << "\nTakes "
            << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0 
            << " seconds. OOV rate is " << lav.oovRate << "%" << endl;
        bleu->addSentences(queryCaption[0].caption, queryData.sentences);
    }
    cout << "Bleu Value: " << bleu->getBleuValue() << endl;
    delete bleu;
    return 0;
}