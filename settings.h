#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
using namespace std;


class Settings {

public:
    static const string& DATAPATH() {
        static const string str = "data/";
        return str;
    }
    static const string& TRAINFILE() {
        static const string str = "mscoco_anno_files/anno_list_mscoco_trainModelVal_m_RNN.txt";
        return str;
    }
    static const string& VALFILE() {
        static const string str = "mscoco_anno_files/anno_list_mscoco_crVal_m_RNN.txt";
        return str;
    }
    static const string& TESTFILE() {
        static const string str = "mscoco_anno_files/anno_list_mscoco_test2014.txt";
        return str;
    }
    static const string& FEATUREFILE() {
        static const string str = "image_features_mRNN/VGG_feat_o_dct_mscoco_2014.txt";
        return str;
    }
    static const int& FARNEIGHBORSIZE() {
        static const int k = 100;
        return k;
    }
    static const int& NEIGHBORSIZE() {
        static const int k = 1;
        return k;
    }
};


#endif