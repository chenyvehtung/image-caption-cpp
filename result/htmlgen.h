#ifndef HTMLGEN_H_
#define HTMLGEN_H_

#include "imgblock.h"
#include <vector>
#include <map>
#include <regex.h>
#include <fstream>
using std::vector;
using std::string;
using std::map;

class HtmlGen {

public:
    HtmlGen();
    HtmlGen(string filename);
    bool generate(const vector<imgBlock>& results);
    void setInfor(const map<string, string>& dict);
    void setBleu(const map<string, double>& name2bleu);

private:

    int compileRegex (regex_t *pResult, const string regexText);
    int matchRegex(regex_t *pResult, const string toMatch, vector<int>& matchResults);
    int processHtml(regex_t *pResult, const string& findText, const vector<imgBlock>& results);
    int genBleuShow();
    int genImgShow(const vector<imgBlock>& results);

    map<string, double> name2Bleu;
    map<string, string> messDict;
    std::fstream outputFile;
    std::fstream inputFile;
};


#endif