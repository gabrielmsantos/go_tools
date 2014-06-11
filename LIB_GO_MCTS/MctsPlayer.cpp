#include "MctsPlayer.h"

MctsPlayer::MctsPlayer(int board_size, boost::shared_ptr<GoReferee> l_referee)
    :m_go_game(board_size, l_referee)
{
}

void MctsPlayer::StartSearchFor(CompactBoard* cb)
{
    //Set up Go Game
    m_go_game.SetFromCompactBoard(cb);

    //Set up the Tree

    //While(Numero de simulaçoes insuficientes)

            //sequence = Play In Tree()
            //result = Play-out(Sequence) //Light Play-out
            //UpdateSequenceInTree(result)


}
