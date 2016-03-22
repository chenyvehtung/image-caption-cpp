#ifndef HTMLGEN_H_
#define HTMLGEN_H_

#include "../utilities/utilities.h"
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;

class HtmlGen {

public:
    HtmlGen();
    HtmlGen(string filename);
    bool generate(map<string, double> n2b,const vector<utilities::ResultArray>& results);
    void setInfor(string authorname, string address);

private:
    bool genHeader();
    bool genNav();
    bool genBody();
    bool genFooter();
    bool genBleuShow();
    bool genImgShow(const vector<utilities::ResultArray>& results);
    bool genImgBlock(const utilities::ResultArray& result);

    map<string, double> name2bleu;
    string author;
    string proAddress;
    string outputFile;
};


#endif