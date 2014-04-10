#ifndef INFLUENCEMODEL_H
#define INFLUENCEMODEL_H


/**
  Interface to an Influence Model
*/

#include <SFML/System/Vector2.hpp>

class I_InfluenceModel
{
public:
    I_InfluenceModel(unsigned int board_size):
        m_goban_influence(board_size)
    {
        //Staring cleared goban
        for(unsigned int i = 0; i < m_goban_influence.size(); ++i)
            {
                m_goban_influence[i] = std::vector<double>( m_goban_influence.size());
                for(unsigned int j =0; j < m_goban_influence[i].size(); ++j)
                {
                    m_goban_influence[i][j] = 0;
                }
            }
    }
    virtual ~I_InfluenceModel(){}

    virtual void PutInfluence(const sf::Vector2i& pos, const int infl_value) = 0;

    virtual void Clear()
    {
        //Staring cleared goban
        for(unsigned int i = 0; i < m_goban_influence.size(); ++i)
            {
                for(unsigned int j =0; j < m_goban_influence[i].size(); ++j)
                {
                    m_goban_influence[i][j] = 0;
                }
            }
    }

    double GetInfluence(unsigned int i,unsigned int j)
    {
        if((i>=0)&&(i<m_goban_influence.size())&&(j>=0)&&(j<m_goban_influence.size()))
        {
            return m_goban_influence[i][j];
        }
        return 0;
    }

    unsigned int GetDimension()
    {
        return m_goban_influence.size();
    }

protected:
    std::vector<std::vector<double> > m_goban_influence;
};
#endif // INFLUENCEMODEL_H
