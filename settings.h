#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
using namespace std;


class Settings {

public:
    //for string, use function because const string is banned from giving init value
    static const string& DATAPATH() {
        static const string str = "data/";
        return str;
    }
    static const string& TRAIN_FILE() {
        static const string str = "mscoco_anno_files/anno_list_mscoco_trainModelVal_m_RNN.txt";
        return str;
    }
    static const string& VAL_FILE() {
        static const string str = "mscoco_anno_files/anno_list_mscoco_crVal_m_RNN.txt";
        return str;
    }
    static const string& TEST_FILE() {
        static const string str = "mscoco_anno_files/anno_list_mscoco_test2014.txt";
        return str;
    }
    static const string& FEATURE_FILE() {
        static const string str = "image_features_mRNN/VGG_feat_o_dct_mscoco_2014.txt";
        return str;
    }
    static const string& MRNN_FEATURE_FILE() {
        static const string str = "image_features_mRNN/VGG_feat_mRNN_refine_dct_mscoco_2014.txt";
        return str;
    }
    static const string& WORD2VEC_BIN() {
        static const string str = "files/corpora/mscoco/vectors.bin";
        return str;
    }
    static const string& EXCLUDE_FILE() {
        static const string str = "files/corpora/mscoco/exclude.txt";
        return str;
    }
    static const string& INCLUDE_FILE() {
        static const string str = "files/corpora/mscoco/include.txt";
        return str;
    }

    //for double, use function because const double is banned from giving init value
    static const double& EPSILON() {
        static const double epsilon = 0.15;
        return epsilon;
    }

    static const int FAR_NEIGHBOR_SIZE = 100;
    static const int NEIGHBOR_SIZE = 1;
    static const bool EX_STOP_WORDS = true;
    static const bool IN_OPERA_WORDS = false;
    static const bool USE_VISUAL_SIMILAR_SCORE = true;
    static const bool USE_SENTENCE_LEN_PENALTY = true;
    static const int RETURN_CPATION_NUM = 1;

};


#endif