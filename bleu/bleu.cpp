#include "bleu.h"


Bleu::Bleu() {
    maxGramNum = 1;
    refNum = 1;
}

Bleu::Bleu(size_t gram, size_t ref){
    maxGramNum = gram;
    refNum = ref;
}