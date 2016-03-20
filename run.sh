#!/bin/sh

g++-4.8 -o demo -fopenmp -std=c++11 -Ofast -march=native -funroll-loops demo.cpp lav/lav.h lav/lav.cpp lav/vision/vision.h lav/vision/vision.cpp lav/language/language.h lav/language/language.cpp settings.h utilities/utilities.h utilities/utilities.cpp bleu/bleu.h bleu/bleu.cpp lib/word2vec-cpp/word2vec.h -lpthread
