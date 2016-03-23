#include "htmlgen.h"
#include <fstream>

#define MAX_ERROR_MSG 0x1000

HtmlGen::HtmlGen() {
    outputFile = "index.html";
}

HtmlGen::HtmlGen(string filename) {
    outputFile = filename;
}

void HtmlGen::setInfo(const map<string, string>& dict) {
    messDict = dict;
}

bool HtmlGen::generate(const vector<imgBlock>& results) {
    inputFile("template.html", std::ios::in);
    if (!inputFile) {
        cout << "cannot open template.html" << std::endl;
        exit(2);
    }
    const string regexText = "\\{\\{[ ]*([a-zA-Z_][a-zA-Z0-9]+)[ ]*}}";
    regex_t regexT;
    compileRegex(&regexT, regexText);

    string findText;
    while(getline(inputFile, findText)) {
        processHtml(&regexT, findText);
    }

    regfree(&regexT);
    inputFile.close();
    outputFile.close();
    return 0;
}

static int HtmlGen::compileRegex (regex_t *pResult, const string regexText) {
    int status = regcomp(pResult, regexText, REG_EXTENDED|REG_NEWLINE);
    if (status != 0) {
        string errMsg;
        regerror(status, pResult, errMsg, MAX_ERROR_MSG);
        cout << "Regex error while compiling \"" << errMsg << "\": " << errMsg << std::endl;
        return 1; 
    }
    return 0;
}

static int matchRegex(regex_t *pResult, const string toMatch, vector<regmatch_t*>& matchResults) {
    matchResults.clear();
    const int matchSize = 10;
    regmatch_t matchResult[matchSize];

    while (1) {
        int noMatch = regexec(pResult, toMatch, matchSize, matchResult, 0);
        if (noMatch) {
            return noMatch;
        }
        matchResults.push_back(matchResult);
    }
    return 0;
}

static int HtmlGen::processHtml(regex_t *pResult, const string& findText) {
    vector<regmatch_t*> matchResults;
    matchRegex(pResult, findText, matchResults);
    for (vector<regmatch_t*>::iterator it = matchResults.end()-1; it >= it.begin(); it--) {
        string subStr = 
    }
}