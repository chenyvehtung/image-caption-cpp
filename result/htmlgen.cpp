#include "htmlgen.h"
#include <fstream>
#include <cstdlib>  //for exit
#include <cstdio>
using std::endl;
using std::cout;

#define MAX_ERROR_MSG 0x1000

HtmlGen::HtmlGen() {
    outputFile.open("index.html", std::fstream::out | std::fstream::trunc);
    if (!outputFile) {
        cout << "cannot create index.html" << std::endl;
        exit(2);
    }
}

HtmlGen::HtmlGen(string filename) {
    outputFile.open(filename+".html", std::fstream::out | std::fstream::trunc);
    if (!outputFile) {
        cout << "cannot create " << filename << ".html" << std::endl;
        exit(1);
    }

}

void HtmlGen::setInfor(const map<string, string>& dict) {
    messDict["title"] = "Image Caption Results";
    messDict["githubAddr"] = "#";
    messDict["intro"] = "#";
    messDict["author"] = "Anonymous";
    messDict["emailAddr"] = "someone@example.com";
    for (map<string, string>::const_iterator it = dict.begin(); it != dict.end(); it++) {
        messDict[it->first] = it->second;
    }
}

void HtmlGen::setBleu(const map<string, double>& name2bleu) {
    for (map<string, double>::const_iterator it = name2bleu.begin(); it != name2bleu.end(); it++) {
        name2Bleu[it->first] = it->second;
    }
}

bool HtmlGen::generate(const vector<imgBlock>& results) {
    inputFile.open("template.html", std::fstream::in);
    if (!inputFile) {
        cout << "cannot open template.html" << std::endl;
        exit(2);
    }
    const string regexText = "\\{\\{[ ]*([a-zA-Z_][a-zA-Z0-9]+)[ ]*}}";
    regex_t regexT;
    compileRegex(&regexT, regexText);
    cout << "Start generating..." << endl;

    string findText;
    while(getline(inputFile, findText)) {
        if (findText.empty()) {
            continue;
        }
        processHtml(&regexT, findText, results);
    }

    regfree(&regexT);
    inputFile.close();
    outputFile.close();
    return 0;
}

int HtmlGen::compileRegex (regex_t *pResult, const string regexText) {
    int status = regcomp(pResult, regexText.c_str(), REG_EXTENDED|REG_NEWLINE);
    if (status != 0) {
        char errMsg[MAX_ERROR_MSG];
        regerror(status, pResult, errMsg, MAX_ERROR_MSG);
        cout << "Regex error while compiling \"" << errMsg << "\": " << errMsg << std::endl;
        return 1; 
    }
    return 0;
}

int HtmlGen::matchRegex(regex_t *pResult, const string toMatch, vector<int>& matchResults) {
    matchResults.clear();
    const int matchSize = 10;
    regmatch_t matchResult[matchSize];
    const char* stringStart = toMatch.c_str();
    const char* preEndPointer = toMatch.c_str();

    while (1) {
        int noMatch = regexec(pResult, preEndPointer, matchSize, matchResult, 0);
        if (noMatch) {
            return noMatch;
        }
        int startPos = matchResult[1].rm_so + (preEndPointer - stringStart);
        int endPos = matchResult[1].rm_eo + (preEndPointer - stringStart);
        matchResults.push_back(startPos);
        matchResults.push_back(endPos);

        for (int i = 0; i < matchSize; i++) {
            if (matchResult[i].rm_so == -1) {
                break;
            }
            int start = matchResult[i].rm_so + (preEndPointer - stringStart);
            int finish = matchResult[i].rm_eo + (preEndPointer - stringStart);
            if (i == 0) {
                printf ("$& is ");
            }
            else {
                printf ("$%d is ", i);
            }
            printf ("'%.*s' (bytes %d:%d)\n", (finish - start),
                    stringStart + start, start, finish);
       }

        preEndPointer += matchResult[0].rm_eo;
    }

    return 0;
}

int HtmlGen::processHtml(regex_t *pResult, const string& findText, const vector<imgBlock>& results) {
    vector<int> matchResults;
    matchRegex(pResult, findText, matchResults);
    if (matchResults.size() == 0) {
        outputFile << findText << std::endl;
        outputFile.flush();
        return 0;
    }
    int preEndPos = 0;
    string replaceText = "";
    for (vector<int>::iterator it = matchResults.begin(); it != matchResults.end(); it+=2) {
        int startPos = *it;
        int endPos = *(it + 1);
        string value = findText.substr(startPos, endPos - startPos);
        if (value == "bleuValueShow") {
            if (name2Bleu.size() != 0)
                genBleuShow();
            return 0;
        }
        else if (value == "imgCapShow") {
            genImgShow(results);
            return 0;
        }
        else {
            if (messDict.find(value) == messDict.end()) {
                cout << "The value is not defined." << std::endl;
                return 1;
            }
            else {
                cout << messDict[value] << ":" << matchResults.size() << endl;
                replaceText += findText.substr(preEndPos, startPos - preEndPos) + messDict[value];
                preEndPos = endPos;
                if (it == (matchResults.end()-2) ) {
                    replaceText += findText.substr(endPos, findText.size()-endPos);
                    outputFile << replaceText << std::endl;
                    outputFile.flush();
                    return 0;
                }
            }
        }//end of other value else branch.
    }
    return 0;
}

int HtmlGen::genBleuShow() {
    outputFile << "<div class=\"bleu_value_show\">" << endl;
    for (map<string, double>::iterator it = name2Bleu.begin(); it != name2Bleu.end(); it++) {
        string name = it->first;
        double bleu = it->second; 
        outputFile << "<div class=\"row\">" << endl 
                   << "<div class=\"col-md-2 col-md-offset-2\">" << endl
                   << "<p>" << name << " BLEU value: </p>" << endl
                   << "</div>" << endl
                   << "<div class=\"col-md-6\">" << endl          
                   << "<div class=\"progress\">" << endl 
                   << "<div class=\"progress-bar\" role=\"progressbar\" aria-valuenow=\"" << (int)(bleu*100) 
                   << "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width: "
                   << (int)(bleu*100) << "%;\">" << endl
                   << bleu << "</div>\n</div>\n</div>\n</div>" << endl;
        outputFile.flush(); 
    }
    outputFile << "</div>" << endl;
    return 0;
}

int HtmlGen::genImgShow(const vector<imgBlock>& results) {
    outputFile << "<div class=\"img_caption_show\">" << endl;
    outputFile.flush();
    const int COLUMN = 3;
    int cnt = 0;
    for (vector<imgBlock>::const_iterator imgItem = results.begin(); imgItem != results.end(); imgItem++) {
        if (cnt % COLUMN == 0) {
            outputFile << "<div class=\"row\">" << endl;
            outputFile.flush();
        }

        outputFile << "<div class=\"col-md-4\">\n<div class=\"img_container\">" 
                   << "<img src=\"" << imgItem->url << "\" alt=\"" << imgItem->id 
                   << "\" title=\"" << imgItem->id << "\" class=\"origin_img img-rounded\" />" << endl;
        outputFile.flush();
        for (vector<string>::const_iterator caption = imgItem->captions.begin(); caption != imgItem->captions.end(); caption++) {
            vector<string> name2caption = imgItem->split(*caption, ':');
            outputFile << "<p><strong>" << name2caption[0] << ": </strong>"
                       << name2caption[1] << "</p>" << endl;
            outputFile.flush();
        }
        outputFile << "</div>\n</div>" << endl;
        outputFile.flush();

        if (cnt % COLUMN == 2 || imgItem == (results.end()-1) ) {
            outputFile << "</div>" << endl;
            outputFile.flush();
        }
        cnt++;
    }

    outputFile << "</div>" << endl;
    outputFile.flush();
    return 0;
}
