#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <cstring>


class Settings {

public:
    static const string& DATAPATH() {
        static const string str = "data";
        return str;
    }
};


#endif