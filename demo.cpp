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
#include <fstream> //for ofstream
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
    /*testData = utilities::dataLoad(Settings::DATAPATH() + Settings::TEST_FILE(), "test", vggFeature);*/
    cend = chrono::high_resolution_clock::now();
    cout << "load data success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
        << " seconds cost." << endl;

    /* append valData to the end of trainData */
    //trainData.insert(trainData.end(), valData.begin(), valData.end());

    int gram = 2, refer = 4;
    Bleu* bleu = new Bleu(gram, refer);
    Bleu* bleuHuman = new Bleu(gram, refer);
    fstream captionResult;
    captionResult.open("capresults.txt", std::fstream::trunc);
    fstream humanResult;
    humanResult.open("humanresults.txt", std::fstream::trunc);

    for (auto& valItem : valData) {
        queryData = valItem;
        cstart = chrono::high_resolution_clock::now();
        queryCaption = lav.describeImg(queryData, trainData);
        cend = chrono::high_resolution_clock::now();
        cout << queryCaption[0].caption << "\nFilename: " << queryData.file_name << " Takes "
            << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0 
            << " seconds. OOV rate is " << lav.oovRate << "%" << endl;

        vector<string> referSentences;
        int index = rand() % 5;
        for (int i = 0; i < 5; i++) {
            if (i != index) {
                referSentences.push_back(queryData.sentences[i]);
            }
        }
        bleu->addSentences(queryCaption[0].caption, referSentences);
        bleuHuman->addSentences(queryData.sentences[index], referSentences);

        captionResult << queryData.id << "\t" << queryCaption[0].caption << "\n";
        humanResult << queryData.id << "\t" << queryData.sentences[index] << "\n";
        captionResult.flush(); humanResult.flush();
    }
    double captionBleu = bleu->getBleuValue();
    double humanBleu = bleuHuman->getBleuValue();

    cout << "Bleu Value: " << captionBleu << endl
        << "Human Bleu Value: " << humanBleu << endl;

    captionResult << "Bleu Value: " << captionBleu << endl;
    humanResult << "Human Bleu Value: " << humanBleu << endl;
    captionResult.close();  humanResult.close();

    delete bleu;
    delete bleuHuman;
    vggFeature.clear();
    trainData.clear();
    valData.clear();
    testData.clear();
    
    return 0;
}