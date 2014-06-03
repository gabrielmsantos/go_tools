#include <stdio.h>
#include "GoDatabase.h"
#include "Goban.h"

/** Creating the memory here*/
int main()
{

    GoDatabase m_database;
    m_database.InsertGamesFrom("/home/gabriel/Downloads/games/",true);
    std::cout <<m_database.GetNumberOfNodes() << " nodes inserted"<<std::endl;

    //TEST SEARCH
    /*Goban goban(19);
    CompactBoard* cb = goban.GetCompactBoard();

    std::vector<short> result =  m_database.NextMoveAdvisor(cb);

    std::vector<short>::iterator it = result.begin();

    for(; it != result.end(); ++it)
    {
        short move = *it;

        short row =  move/MAX_BOARD;
        short column = move%MAX_BOARD;

        if(move > 0)
            std::cout << "BLACK: "<< move<< std::endl;
        else
            std::cout << "WHITE: "<< move <<std::endl;

        std::cout << "Linha: "<< row << std::endl;
        std::cout << "Coluna: "<< column << std::endl;
        std::cout << "-----" << std::endl;
    }

    delete cb;*/
    while(true)
    {
        sleep(1);
    }
}
//=============================================================================================
