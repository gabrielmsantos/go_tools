#include "RandomPolicy.h"
#include <stdlib.h>
#include "GobanUtils.h"
#include "GoUtils.h"

RandomPolicy::RandomPolicy(SimpleGoGame &go_game):I_PlayoutPolicy(go_game)
{
}

short RandomPolicy::GeneratePlayOutMove()
{
    srand (time(NULL));

    //Generate all legal moves
    std::vector<short> legal_moves;
    m_go_game_ref.GenerateAllLegalMoves(legal_moves);

    std::vector<short> potential_eye_vec;

    short move = 0;

    /**
     * 1 - Never fill an eye
     * 2 - If there is a potential eye , do not fill it in the first place, just return this move in case of there is no
     *      any olther move to return;
      */
    while(!legal_moves.empty())
    {
        //Choose one of them randomly
        int position = rand() % legal_moves.size();

        move = legal_moves[position];
        std::pair<unsigned short, unsigned short> board_pos = GoUtils::MoveToBoardPosition(move);

        if( GobanUtils::IsOnePointEye(board_pos.first, board_pos.second, m_go_game_ref.GetMainBoard(), m_go_game_ref.GetCurrentPlayer()) )
        {
            // remove it from legal moves and choose another one
            legal_moves.erase(legal_moves.begin() + position);
            continue;

        }/*else if(GobanUtils::IsPotentialOnePointEye(board_pos.first, board_pos.second, m_go_game_ref.GetMainBoard(), m_go_game_ref.GetCurrentPlayer()))
        {
            //delay it
            potential_eye_vec.push_back(move);

            // remove it from legal moves and choose another one
            legal_moves.erase(legal_moves.begin() + position);
            continue;

        }*/else
        {
            return move;
        }
    }

    /**
     *Since a move was not returned in the previous loop, it means that either there is no legal move,
     * so it will be returned 0, or there are just fill potential eye move, in this case the first one
     *will be returned.
    */
    if(potential_eye_vec.empty())
    {
        return 0;
    }else
    {
        return potential_eye_vec.front();
    }
}
