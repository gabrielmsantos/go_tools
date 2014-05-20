#ifndef GOBANGUI_H
#define GOBANGUI_H

#include "ColoredSquare.h"
#include "Goban.h"
#include "GoInfluenceStatistics/I_GobanStatistics.h"

class GobanGUI
{
public:
    GobanGUI(int board_size);
    void Draw(sf::RenderTarget& r_target, Goban& r_goban, I_GobanStatistics& r_statistics, I_InfluenceModel& r_influence, std::vector<short>& move_sugestion);
    void MapClick(const sf::Vector2i& r_clickPosition, sf::Vector2i& r_mapPosition);

private:

    double m_start_x;
    double m_start_y;
    double m_scale;
    char m_labels[25];


    std::vector<sf::Sprite > m_points_sprites;
    sf::Texture m_blackTexture;
    sf::Texture m_whiteTexture;
    sf::Texture m_pointTexture;

    sf::Texture m_blackAdvisorTexture;
    sf::Texture m_whiteAdvisorTexture;
    sf::Texture m_mixedAdvisorexture;

    sf::Font m_font;

    //board GUI
    std::vector< std::vector< ColoredSquare> >m_colored_squares;
    std::vector< std::vector <sf::Sprite > > m_stone_sprites;
};


#endif // GOBANGUI_H
