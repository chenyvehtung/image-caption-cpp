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

private:

    static int compileRegex (regex_t *pResult, const string regexText);
    static int matchRegex(regex_t *pResult, const string toMatch, vector<regmatch_t*>& matchResults);
    static int processHtml (regex_t *pResult, string& findText);

    map<string, string> name2Bleu;
    map<string, string> messDict;
    std::ofstream outputFile;
    std::ifstream inputFile;
};


#endif