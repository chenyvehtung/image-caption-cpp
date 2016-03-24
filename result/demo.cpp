#include "htmlgen.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    HtmlGen htmlGen("homepage.html");
    map<string, double> name2bleu;
    name2bleu["Human"] = 0.7845;
    name2bleu["Meachine"] = 0.333;
    htmlGen.setBleu(name2bleu);

    map<string, string> dict;
    dict["author"] = "Donald";
    htmlGen.setInfor(dict);

    vector<string> test;
    test.push_back("ours:bright street light shining down on the street");
    test.push_back("human:a night scene of a traffic light in front of a parking lot");
    test.push_back("them:a night scene of a traffic light in front of a parking lot");
    imgBlock imgblock(test, "http://shannon.cs.illinois.edu/alice/MSCOCO/images/COCO_val2014_000000226128.jpg", "MSCOCO_000000226128.jpg");
    vector<imgBlock> imgblocks;
    for (int i = 0; i < 10; i++) {
        imgblocks.push_back(imgblock);
    }
    std::cout << "Everything was all set." << std::endl; 
    htmlGen.generate(imgblocks);
    std::cout << "Finish" << std::endl;

    return 0;
}