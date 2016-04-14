#include "bleu.h"
#include <fstream> //for ofstream
#include <iostream> 
#include <string> //for getline
#include <map>
#include <vector>
#include <sstream> //for istringstream
#include <algorithm> //for find

using std::fstream;
using std::istringstream;
using std::cout;
using std::endl;

int main() {

    string row, token, id;

    /*------------- Load Result to Memory ------------------*/
    fstream im2textResult;
    im2textResult.open("im2textResult.txt", fstream::in);
    map<string, string> results;
    while (getline(im2textResult, row)) {
        if (row.empty()) 
            continue;
        istringstream rowTokens(row);
        int cnt = 0;
        while (getline(rowTokens, token, '\t')) {
            if (cnt == 0) {
                id = token;
                cnt++;
            }
            else {
                cnt = 0;
                results[id] = token;
            }
        }
    }

    /*---------------Load Reference to Memory --------------*/
    fstream imgRef;
    imgRef.open("humanref.txt", fstream::in);
    map<string, vector<string> > refers;
    while (getline(imgRef, row)) {
        if (row[0] == '#') {
            id = row.substr(1, row.length()-1);
        }
        else {
            refers[id].push_back(row);
        }
    }

    /*--------------- Set Up Display Value --------------------*/
    vector<string> displayId;
    displayId.push_back("494438");
    displayId.push_back("114108");
    displayId.push_back("298637");
    displayId.push_back("417727");
    displayId.push_back("482994");
    displayId.push_back("30796");

    /*--------------- Get Bleu value --------------------*/
    Bleu bleuI2T[4];
    for (int cnt = 0; cnt < 4; cnt++) {
        bleuI2T[cnt].init(cnt + 1, 4);
    }
    map<string, string>::iterator it;
    vector<string>::iterator itId;
    for (it = results.begin(); it != results.end(); it++) {
        for (int cnt = 0; cnt < 4; cnt++) {
            bleuI2T[cnt].addSentences(it->second, refers[it->first]);
        }
        itId = find(displayId.begin(), displayId.end(), it->first);
        if (itId != displayId.end()) {
            cout << "#" << it->first << ":" << endl;
            cout << "Candidate: " << it->second << endl;
            for (int i = 0; i < 4; i++) {
                cout << "Reference " << i + 1 << ": " << refers[it->first][i] << endl;
            }
        }
    }
    for (int cnt = 0; cnt < 4; cnt++) {
        cout << "bleuI2T[" << cnt + 1 << "]: " << bleuI2T[cnt].getBleuValue() << endl;
    }

    /*--------------- Release Memory --------------------*/
    //delete bleuI2T;
    im2textResult.close();
    imgRef.close();

    return 0;
}
