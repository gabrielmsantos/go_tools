#ifndef SGFPARSER_H
#define SGFPARSER_H

#include <fstream>
#include "GoGame.h"

class SGFParser
{
public:
    SGFParser();
    void ParseGame(std::ifstream &in_file, GoGame *l_go_game);
private:
    void Process(std::string& property, std::string& value, GoGame* l_go_game);
};

#endif // SGFPARSER_H
