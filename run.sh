#!/bin/sh

g++-4.8 -o demo -std=c++11 demo.cpp lav/lav.h lav/lav.cpp lav/vision/vision.h lav/vision/vision.cpp lav/language/language.h lav/language/language.cpp settings.h utilities/utilities.h utilities/utilities.cpp lib/word2vec-cpp/word2vec.h
