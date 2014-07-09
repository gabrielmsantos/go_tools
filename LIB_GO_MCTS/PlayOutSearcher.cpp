#include "PlayOutSearcher.h"
#include "GoUtils.h"
#include "RandomPolicy.h"
#include "GobanUtils.h"


PlayOutSearcher::PlayOutSearcher(unsigned int board_size, boost::shared_ptr<GoReferee> l_referee)
    :m_simple_go_game(board_size, l_referee)
{
    m_playout_policy_ptr = new RandomPolicy();
}

//======================================================================================
//double PlayOutSearcher::PlayOutGameFrom(CompactBoard *cb, StoneState to_play)
//{

//    //Setting the board up;
//    m_simple_go_game.SetFromCompactBoard(cb);

//    //Setting Current Player
//    m_simple_go_game.SetCurrentPlayer(to_play);

//    short move;
//    double score = 0;
//    std::pair<unsigned short, unsigned short> m_board_move;

//    unsigned short nro_passes = 0;
//    unsigned int count_moves = 0;
//    unsigned short dimension =  m_simple_go_game.Dimension();


//    /** Establishing a limit of moves*/
//    unsigned int limit_moves = 3 * dimension * dimension;

//    bool interrupted = false;

//    //Finishes a game
//    while((nro_passes < 2) && (!interrupted) )
//    {
//        /**
//          This will generate a move accordding to the play-out policy in use.
//        */
//        move = GeneratePlayOutMove();

//        if(move == 0)
//        {
//            nro_passes++;
//            m_simple_go_game.PlayPassMove();
//            continue;
//        }else
//        {
//            nro_passes = 0;
//        }

//        m_board_move = GoUtils::MoveToBoardPosition(move);

//        //Execute the move
//        m_simple_go_game.PlayMove(m_board_move.first, m_board_move.second, m_simple_go_game.GetCurrentPlayer());

//        count_moves++;

//        if(count_moves >  limit_moves)
//        {
//            std::cout << "interrupted "<<  std::endl;
//            interrupted = true;
//        }
//    }

//    //TODO: Should I input Komi ?
//    //Evaluate Board position

//    std::pair<double, double> result  = GobanUtils::ScoreSimpleEndPosition(m_simple_go_game.GetMainBoard());
//    //Chinese Scoring method

//    //BLACK - WHITE
//    score = result.first - result.second;

//    score = score/(abs(score)); //-1 or +1

//    return score;
//}

//======================================================================================
double PlayOutSearcher::PlayOutGameFrom(SimpleGoGame& simple_go_game_ref)
{

    short move;
    double score = 0;
    std::pair<unsigned short, unsigned short> m_board_move;

    unsigned short nro_passes = 0;
    unsigned int count_moves = 0;
    unsigned short dimension =  simple_go_game_ref.Dimension();


    /** Establishing a limit of moves*/
    unsigned int limit_moves = 3 * (dimension * dimension);

    bool interrupted = false;

    //Finishes a game
    while((nro_passes < 2) && (!interrupted) )
    {
        /**
          This will generate a move accordding to the play-out policy in use.
        */
        move = GeneratePlayOutMove(simple_go_game_ref);

        if(move == 0)
        {
            nro_passes++;
            simple_go_game_ref.PlayPassMove();
            continue;
        }else
        {
            nro_passes = 0;
        }

        m_board_move = GoUtils::MoveToBoardPosition(move);

        //Execute the move
        simple_go_game_ref.PlayMove(m_board_move.first, m_board_move.second, simple_go_game_ref.GetCurrentPlayer());

        count_moves++;

        if(count_moves >  limit_moves)
        {
            std::cout << "interrupted "<<  std::endl;
            interrupted = true;
        }
    }    

    std::cout << "Count: " << count_moves << std::endl;

    if(interrupted)
    {
        return 0;
    }

    //TODO: Should I input Komi ?
    //Evaluate Board position

    std::pair<double, double> result  = GobanUtils::ScoreSimpleEndPosition(simple_go_game_ref.GetMainBoard());
    //Chinese Scoring method

    //BLACK - WHITE
    score = result.first - result.second;

    if(score != 0)
    {
        score = score/(abs(score)); //-1 or +1
    }

    return score;
}

//======================================================================================
//short PlayOutSearcher::GeneratePlayOutMove()
//{
//    return m_playout_policy_ptr->GeneratePlayOutMove();
//}
//======================================================================================
short PlayOutSearcher::GeneratePlayOutMove(SimpleGoGame& simple_go_game_ref)
{
    return m_playout_policy_ptr->GeneratePlayOutMove(simple_go_game_ref);
}
