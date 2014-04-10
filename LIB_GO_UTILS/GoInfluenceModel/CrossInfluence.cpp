#include "../Goban.h"
#include "CrossInfluence.h"


CrossInfluence::CrossInfluence(unsigned int board_size)
:I_InfluenceModel(board_size)
{
}

void CrossInfluence::Notify(const sf::Vector2i& pos, int infl_value)
{
    PutInfluence(pos, infl_value);
}
// Influência fixa
void CrossInfluence::PutInfluence(const sf::Vector2i& pos,  int infl_value)
{
    infl_value *= INFLUENCE;

//    if((pos.x== r_goban.Dimension()) || (pos.y==r_goban.Dimension()) || (pos.x<0) || (pos.y<0))
//        return;

    m_goban_influence[pos.x][pos.y]+=infl_value;

    //direct space
    if( pos.x+1 < (int)m_goban_influence.size())
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y), DIRECT*infl_value);
        m_goban_influence[pos.x+1][pos.y]+=DIRECT*infl_value;
    if(pos.x-1 >= 0)
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y), DIRECT*infl_value);
        m_goban_influence[pos.x-1][pos.y]+=DIRECT*infl_value;
    if(pos.y+1 < (int)m_goban_influence.size())
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x, pos.y+1), DIRECT*infl_value);
        m_goban_influence[pos.x][pos.y+1]+=DIRECT*infl_value;
    if(pos.y-1 >= 0)
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x, pos.y-1), DIRECT*infl_value);
        m_goban_influence[pos.x][pos.y-1]+=DIRECT*infl_value;

    //diagonal
    if((pos.x-1 >=0)&&(pos.y-1 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y-1), DIAGONAL*infl_value);
        m_goban_influence[pos.x-1][pos.y-1]+=DIAGONAL*infl_value;

    if((pos.x-1 >=0)&&(pos.y+1 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y+1), DIAGONAL*infl_value);
        m_goban_influence[pos.x-1][pos.y+1]+=DIAGONAL*infl_value;

    if((pos.x+1 < (int)m_goban_influence.size())&&(pos.y-1 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y-1), DIAGONAL*infl_value);
        m_goban_influence[pos.x+1][pos.y-1]+=DIAGONAL*infl_value;

    if((pos.x+1 < (int)m_goban_influence.size())&&(pos.y+1 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y+1), DIAGONAL*infl_value);
        m_goban_influence[pos.x+1][pos.y+1]+=DIAGONAL*infl_value;

    //One Space Jump
    if(pos.x+2 < (int)m_goban_influence.size())
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+2, pos.y), OSJ*infl_value);
        m_goban_influence[pos.x+2][pos.y]+=OSJ*infl_value;
    if(pos.x-2 >= 0)
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-2, pos.y), OSJ*infl_value);
        m_goban_influence[pos.x-2][pos.y]+=OSJ*infl_value;
    if(pos.y+2 < (int)m_goban_influence.size())
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x, pos.y+2), OSJ*infl_value);
        m_goban_influence[pos.x][pos.y+2]+=OSJ*infl_value;
    if(pos.y-2 >= 0)
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x, pos.y-2), OSJ*infl_value);
        m_goban_influence[pos.x][pos.y-2]+=OSJ*infl_value;

    //Knight Move
    if((pos.x-1 >=0)&&(pos.y-2 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y-2), KNIGHT*infl_value);
        m_goban_influence[pos.x-1][pos.y-2]+=KNIGHT*infl_value;

    if((pos.x-2 >=0)&&(pos.y-1 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-2, pos.y-1), KNIGHT*infl_value);
        m_goban_influence[pos.x-2][pos.y-1]+=KNIGHT*infl_value;

    if((pos.x-2 >=0)&&(pos.y+1 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-2, pos.y+1), KNIGHT*infl_value);
        m_goban_influence[pos.x-2][pos.y+1]+=KNIGHT*infl_value;

    if((pos.x-1 >=0)&&(pos.y+2 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y+2), KNIGHT*infl_value);
        m_goban_influence[pos.x-1][pos.y+2]+=KNIGHT*infl_value;

    if((pos.x+2 < (int)m_goban_influence.size())&&(pos.y-1 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+2, pos.y-1), KNIGHT*infl_value);
        m_goban_influence[pos.x+2][pos.y-1]+=KNIGHT*infl_value;

    if((pos.x+1 < (int)m_goban_influence.size())&&(pos.y-2 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y-2), KNIGHT*infl_value);
        m_goban_influence[pos.x+1][pos.y-2]+=KNIGHT*infl_value;

    if((pos.x+2 < (int)m_goban_influence.size())&&(pos.y+1 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+2, pos.y+1), KNIGHT*infl_value);
        m_goban_influence[pos.x+2][pos.y+1]+=KNIGHT*infl_value;

    if((pos.x+1 < (int)m_goban_influence.size())&&(pos.y+2 <  (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y+2), KNIGHT*infl_value);
        m_goban_influence[pos.x+1][pos.y+2]+=KNIGHT*infl_value;

    //Two Space Jump
    if(pos.x+3 < (int)m_goban_influence.size())
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+3, pos.y), TSJ*infl_value);
        m_goban_influence[pos.x+3][pos.y]+=TSJ*infl_value;
    if(pos.x-3 >= 0)
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-3, pos.y), TSJ*infl_value);
        m_goban_influence[pos.x-3][pos.y]+=TSJ*infl_value;
    if(pos.y+3 < (int)m_goban_influence.size())
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x, pos.y+3), TSJ*infl_value);
        m_goban_influence[pos.x][pos.y+3]+=TSJ*infl_value;
    if(pos.y-3 >= 0)
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x, pos.y-3), TSJ*infl_value);
        m_goban_influence[pos.x][pos.y-3]+=TSJ*infl_value;

    //Large Knight Move
    if((pos.x-1 >=0)&&(pos.y-3 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y-3), LKNIGHT*infl_value);
        m_goban_influence[pos.x-1][pos.y-3]+=LKNIGHT*infl_value;

    if((pos.x-3 >=0)&&(pos.y-1 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-3, pos.y-1), LKNIGHT*infl_value);
        m_goban_influence[pos.x-3][pos.y-1]+=LKNIGHT*infl_value;

    if((pos.x-3 >=0)&&(pos.y+1 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-3, pos.y+1), LKNIGHT*infl_value);
        m_goban_influence[pos.x-3][pos.y+1]+=LKNIGHT*infl_value;

    if((pos.x-1 >=0)&&(pos.y+3 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x-1, pos.y+3), LKNIGHT*infl_value);
        m_goban_influence[pos.x-1][pos.y+3]+=LKNIGHT*infl_value;

    if((pos.x+3 < (int)m_goban_influence.size())&&(pos.y-1 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+3, pos.y-1), LKNIGHT*infl_value);
        m_goban_influence[pos.x+3][pos.y-1]+=LKNIGHT*infl_value;

    if((pos.x+1 < (int)m_goban_influence.size())&&(pos.y-3 >= 0))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y-3), LKNIGHT*infl_value);
        m_goban_influence[pos.x+1][pos.y-3]+=LKNIGHT*infl_value;

    if((pos.x+3 < (int)m_goban_influence.size())&&(pos.y+1 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+3, pos.y+1), LKNIGHT*infl_value);
        m_goban_influence[pos.x+3][pos.y+1]+=LKNIGHT*infl_value;

    if((pos.x+1 < (int)m_goban_influence.size())&&(pos.y+3 < (int)m_goban_influence.size()))
        //r_goban.ChangeInfluence(sf::Vector2i(pos.x+1, pos.y+3), LKNIGHT*infl_value);
        m_goban_influence[pos.x+1][pos.y+3]+=LKNIGHT*infl_value;
}

