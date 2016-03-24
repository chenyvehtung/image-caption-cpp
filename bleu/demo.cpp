#include "bleu.h"
#include <iostream>
using std::string;

int main(int argc, char const *argv[])
{
    Bleu bleu(2, 3);
    string candidate1 = "It is a guide to action which ensures that the military always obeys the commands of the party";
    string candidate2 = "It is to insure the troops forever hearing the activity guidebook that party direct";
    std::vector<string> references;
    references.push_back("It is a guide to action that ensures that the military will forever heed Party commands");
    references.push_back("It is the guiding principle which guarantees the military forces always being under the command of the Party");
    references.push_back("It is the practical guide for the army always to heed the directions of the party");
    
    string candidate3 = "of the";

    bleu.addSentences(candidate1, references);
    std::cout << bleu.getBleuValue() << std::endl;
    return 0;
}