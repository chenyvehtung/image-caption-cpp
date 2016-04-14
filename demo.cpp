#include "settings.h"
#include "utilities/utilities.h"
#include "lav/lav.h"
#include "bleu/bleu.h"
#include "result/htmlgen.h"
#include <string>
#include <map>
#include <iostream>
#include <cstdlib> //for srand and rand
#include <time.h> //for time
#include <chrono>  //for high_resolution_clock
#include <fstream> //for ofstream
using namespace std;

int main() {
    vector<utilities::DataArray> trainData, valData, testData, mRNNTrainData, mRNNValData, mRNNTestData;
    map< string, vector<double> > vggFeature, mRNNVggFeature;
    utilities::DataArray queryData, mRNNQueryData;
    vector<utilities::CaptionArray> queryCaption, mRNNQueryCaption;
    Lav lav, mRNNLav;

    /*-------------------------------- Load VGG Feature -------------------------------------*/
    auto cstart = chrono::high_resolution_clock::now();
    vggFeature = utilities::vggLexer(Settings::DATAPATH() + Settings::FEATURE_FILE());
    auto cend = chrono::high_resolution_clock::now();
    std::cout << "load vgg success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0 
         << " seconds cost." << endl;
    cstart = chrono::high_resolution_clock::now();
    mRNNVggFeature = utilities::vggLexer(Settings::DATAPATH() + Settings::MRNN_FEATURE_FILE());
    cend = chrono::high_resolution_clock::now();
    std::cout << "load mRNN vgg success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
        << " seconds cost." << endl;

    /*-------------------------------- Load Train&Val Data ----------------------------------*/
    cstart = chrono::high_resolution_clock::now();
    trainData = utilities::dataLoad(Settings::DATAPATH() + Settings::TRAIN_FILE(), "train", vggFeature);
    valData = utilities::dataLoad(Settings::DATAPATH() + Settings::VAL_FILE(), "val", vggFeature);
    //testData = utilities::dataLoad(Settings::DATAPATH() + Settings::TEST_FILE(), "test", vggFeature);
    cend = chrono::high_resolution_clock::now();
    std::cout << "load data success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
        << " seconds cost." << endl;
    cstart = chrono::high_resolution_clock::now();
    mRNNTrainData = utilities::dataLoad(Settings::DATAPATH() + Settings::TRAIN_FILE(), "train", mRNNVggFeature);
    mRNNValData = utilities::dataLoad(Settings::DATAPATH() + Settings::VAL_FILE(), "val", mRNNVggFeature);
    //mRNNTestData = utilities::dataLoad(Settings::DATAPATH() + Settings::TEST_FILE(), "test", mRNNVggFeature);
    cend = chrono::high_resolution_clock::now();
    std::cout << "load mRNN data success! " << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
        << " seconds cost." << endl;

    /* append valData to the end of trainData */
    //trainData.insert(trainData.end(), valData.begin(), valData.end());

    /*--------------------------- Init Bleu Log File & Img Blocks ----------------------------*/
    int refer = 4;
    int maxGram = 4;
    Bleu* bleuVGG = new Bleu[4];
    Bleu* bleuMRNN = new Bleu[4];
    Bleu* bleuHuman = new Bleu[4];
    for (int i = 0; i < maxGram; i++) {
        bleuVGG[i].init(i + 1, refer);
        bleuMRNN[i].init(i + 1, refer);
        bleuHuman[i].init(i + 1, refer);
    }

    fstream captionResult;
    captionResult.open("capresults.txt", std::fstream::out | std::fstream::trunc);
    fstream humanResult;
    humanResult.open("humanresults.txt", std::fstream::out | std::fstream::trunc);
    fstream mRNNResult;
    mRNNResult.open("mrnnresults.txt", std::fstream::out | std::fstream::trunc);
    fstream humanRef;
    humanRef.open("humanref.txt", std::fstream::out | std::fstream::trunc);
    vector<imgBlock> imgBlocks;
    int valSize = valData.size();
    const int SAMPLESIZE = 100;
    int beSample = valSize / SAMPLESIZE;
    int valCnt = 0;
    
    /*-------------------------------- Describe Each Picture ----------------------------------*/
    for (auto& valItem : valData) {
        /* Describe each picture and record */
        queryData = valItem;
        cstart = chrono::high_resolution_clock::now();
        queryCaption = lav.describeImg(queryData, trainData);
        cend = chrono::high_resolution_clock::now();
        std::cout << queryCaption[0].caption << "\nFilename: " << queryData.file_name << " Takes "
            << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0 
            << " seconds. OOV rate is " << lav.oovRate << "%" << endl;
        mRNNQueryData = mRNNValData[valCnt];
        cstart = chrono::high_resolution_clock::now();
        mRNNQueryCaption = mRNNLav.describeImg(mRNNQueryData, mRNNTrainData);
        cend = chrono::high_resolution_clock::now();
        std::cout << mRNNQueryCaption[0].caption << "\nFilename: " << mRNNQueryData.file_name << " Takes "
            << chrono::duration_cast<std::chrono::microseconds>(cend - cstart).count() / 1000000.0
            << " seconds. OOV rate is " << mRNNLav.oovRate << "%" << endl;

        /* Add sentences to Bleu and log the sentences */
        vector<string> referSentences;
        int index = rand() % (refer + 1);
        for (int i = 0; i < refer + 1; i++) {
            if (i != index) {
                referSentences.push_back(queryData.sentences[i]);
            }
        }
        for (int i = 0; i < maxGram; i++) {
            bleuVGG[i].addSentences(queryCaption[0].caption, referSentences);
            bleuMRNN[i].addSentences(mRNNQueryCaption[0].caption, referSentences);
            bleuHuman[i].addSentences(queryData.sentences[index], referSentences);
        }
        captionResult << queryData.id << "\t" << queryCaption[0].caption << "\n";
        humanResult << queryData.id << "\t" << queryData.sentences[index] << "\n";
        mRNNResult << mRNNQueryData.id << "\t" << mRNNQueryCaption[0].caption << "\n";
        humanRef << "#" << queryData.id << "\n";
        for (int i = 0; i < refer; i++) {
            humanRef << referSentences[i] << "\n";
        }
        captionResult.flush(); humanResult.flush(); mRNNResult.flush(); humanRef.flush();

        /* Create imgBlock for feature display */
        if (valCnt % beSample == 0) {
            vector<string> tempVec;
            tempVec.push_back("VGG:" + queryCaption[0].caption);
            tempVec.push_back("MRNN:" + mRNNQueryCaption[0].caption);
            tempVec.push_back("Human:" + queryData.sentences[index]);
            imgBlock imgblock(tempVec, queryData.url, queryData.file_name);
            imgBlocks.push_back(imgblock);
        }

        valCnt++;
    }

    /*-------------------------------- Calculate BLEU Value ----------------------------------*/
    double captionBleu = bleuVGG->getBleuValue();
    double humanBleu = bleuHuman->getBleuValue();
    double mrnnBleu = bleuMRNN->getBleuValue();
    std::cout << "VGG Bleu Value: " << captionBleu << "\nHuman Bleu Value: " << humanBleu << endl
              << "mRNN Bleu Value: " << mrnnBleu << endl;
    captionResult << "VGG Bleu Value[0]: " << captionBleu << endl;
    humanResult << "Human Bleu Value[0]: " << humanBleu << endl;
    mRNNResult << "mRNN Bleu Value[0]: " << mrnnBleu << endl;
    captionResult.flush(); humanResult.flush(); mRNNResult.flush();
    for (int i = 1; i < maxGram; i++) {
        captionResult << "VGG Bleu Value[" << i+1 << "]: " << bleuVGG[i].getBleuValue() << endl;
        mRNNResult << "mRNN Bleu Value[" << i+1 << "]: " << bleuMRNN[i].getBleuValue() << endl;
        humanResult << "Human Bleu Value[" << i+1 << "]: " << bleuHuman[i].getBleuValue() << endl;
        captionResult.flush(); humanResult.flush(); mRNNResult.flush();
    }

    /*-------------------------------- Generate GUI Display ----------------------------------*/
    HtmlGen* htmlGen = new HtmlGen();
    map<string, double> name2bleu;
    name2bleu["meachine"] = captionBleu;
    name2bleu["human"] = humanBleu;
    name2bleu["mrnn"] = mrnnBleu; 
    htmlGen->setBleu(name2bleu);
    map<string, string> info;
    info["author"] = "Donald";
    htmlGen->setInfor(info);
    htmlGen->generate(imgBlocks);

    /*----------------------------------- Release Memory --------------------------------------*/
    captionResult.close();  
    humanResult.close();
    mRNNResult.close();
    humanRef.close();
    delete bleuVGG;
    delete bleuHuman;
    delete bleuMRNN;
    delete htmlGen;
    vggFeature.clear();
    mRNNVggFeature.clear();
    trainData.clear(); valData.clear(); testData.clear();
    mRNNTrainData.clear(); mRNNValData.clear(); mRNNTestData.clear();
    imgBlocks.clear();
    
    return 0;
}