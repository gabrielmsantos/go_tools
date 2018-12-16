#include "GobanGUI.h"
#include "GoUtils.h"
#include "../Config.h"

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

GobanGUI::GobanGUI(int board_size):
    m_start_x(50),
    m_start_y(50),
    m_scale(1),
    m_labels{'A','B','C','D','E','F','G','H','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
    m_colored_squares(board_size),
    m_stone_sprites(board_size)
{
    if(!m_font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf")) //<- in the same folder as exe
    //if(!m_font.loadFromFile("Fonts\\arial.ttf")) //<- in the same folder as exe
    {
        std::cout << "Failed to load font." << std::endl;
    }

    //Setting ColoredSquares
    for(unsigned int i = 0; i < m_colored_squares.size(); ++i)
    {
        m_colored_squares[i] = std::vector<ColoredSquare>( m_colored_squares.size());

        for(unsigned int j =0; j < m_colored_squares[i].size(); ++j)
        {
            m_colored_squares[i][j].SetPosition(sf::Vector2f(m_start_x+(i*m_colored_squares[i][j].GetSprite().getLocalBounds().width*m_scale)-i,m_start_y + (j*m_colored_squares[i][j].GetSprite().getLocalBounds().height*m_scale)-j));
        }
    }

    //Setting StoneSprites
    for(unsigned int i = 0; i < m_stone_sprites.size(); ++i)
    {
        m_stone_sprites[i] = std::vector<sf::Sprite>( m_colored_squares.size());
    }

//    m_blackTexture.loadFromFile( "Images\\black.png");
//    m_whiteTexture.loadFromFile( "Images\\white.png");
//    m_pointTexture.loadFromFile( "Images\\point.png");

    m_blackTexture.loadFromFile( "Images/black.png");
    m_whiteTexture.loadFromFile( "Images/white.png");
    m_pointTexture.loadFromFile( "Images/point.png");

    m_blackAdvisorTexture.loadFromFile( "Images/black_adv.png");
    m_whiteAdvisorTexture.loadFromFile( "Images/white_adv.png");
    m_mixedAdvisorexture.loadFromFile( "Images/mixed_adv.png");

    //Calculating number of points
    double board_factor = board_size/3.0f;

    if(board_factor > 2)
    {
        int num_of_points=0;

        ((board_size%2 ==0) || (board_factor < 3)) ? num_of_points = 4 : num_of_points = 9;

        if(board_factor<=4)
        {
            board_factor=3;

        }else if(board_factor>4)
        {
            board_factor=4;
        }

        double point_scale = .7f;
        int points = sqrt(num_of_points);
        for(int i=0, line = board_factor-1 ;i<points;++i, line+= (board_size-(2*board_factor))/(points-1)+1)
            for(int j=0, column = board_factor-1 ;j<points;++j, column+=(board_size-(2*board_factor))/(points-1)+1)
            {
                sf::Sprite point_sprite;

                point_sprite.setTexture(m_pointTexture);
                point_sprite.setScale(point_scale, point_scale);
                //colored square center
                sf::Vector2f cs_center = m_colored_squares[line][column].Center();
                point_sprite.setPosition(cs_center.x - (point_sprite.getLocalBounds().width*point_scale/2),cs_center.y - (point_sprite.getLocalBounds().height*point_scale/2));
                m_points_sprites.push_back(point_sprite);
            }
    }

}
//================================================================================
void GobanGUI::MapClick(const sf::Vector2i& r_clickPosition, sf::Vector2i& r_mapPosition)
{
    r_mapPosition.x =   ((r_clickPosition.x - m_start_x) / (SPRITE_WIDTH + LINE_THICKNESS));
    r_mapPosition.y =   ((r_clickPosition.y-m_start_y) / (SPRITE_HEIGHT + LINE_THICKNESS));
}
//================================================================================
void GobanGUI::Draw(sf::RenderTarget& r_target, Goban& r_goban, I_GobanStatistics &r_statistics, I_InfluenceModel& r_influence, std::vector<short>& move_sugestion)
{

    assert((unsigned int)r_goban.Dimension() == m_colored_squares.size());
    assert(m_colored_squares[0].size() == (unsigned int) r_goban.Dimension());


    //================= START MOVE ADVISOR MAP ==============================
    /** Creating move advisor map*/
    std::vector< std::vector <char> > l_move_advisor_map(MAX_BOARD);

    //Setting Move Advisor Map
    for(unsigned int i = 0; i < l_move_advisor_map.size(); ++i)
    {
        l_move_advisor_map[i] = std::vector<char>( MAX_BOARD);
    }

    for(unsigned int  i = 0; i < move_sugestion.size(); ++i)
    {
        short move = move_sugestion[i];

        std::pair<short, short> l_pair = GoUtils::MoveToBoardPosition(move);
        short row =  l_pair.first;
        short column = l_pair.second;

        if(move > 0)
        {
            if(l_move_advisor_map[row][column] == 'w')
            {
                l_move_advisor_map[row][column] = 'm';

            }else if(l_move_advisor_map[row][column] != 'm')
            {
                l_move_advisor_map[row][column] = 'b';
            }
        }else
        {
            if(l_move_advisor_map[row][column] == 'b')
            {
                l_move_advisor_map[row][column] = 'm';

            }else if(l_move_advisor_map[row][column] != 'm')
            {
                l_move_advisor_map[row][column] = 'w';
            }

        }

    }
    //================= END MOVE ADVISOR MAP ==============================

    //First Draw influences
    for(unsigned int i = 0; i < (unsigned int) r_goban.Dimension(); ++i)
    {
        for(unsigned int j =0; j < (unsigned int) r_goban.Dimension(); ++j)
        {
            //Draw the influence square
            m_colored_squares[i][j].ChangeValue(r_influence.GetInfluence(i,j));
            m_colored_squares[i][j].Draw(r_target);
        }
    }

    double x,y;
    //Then draw the board
    for(int i=0; i< r_goban.Dimension(); ++i)
    {
        sf::Text labelh;
        labelh.setCharacterSize(12);
        labelh.setFont(m_font);
        labelh.setString(to_string(r_goban.Dimension()-i));
        labelh.setColor(sf::Color::White);
        x = m_colored_squares[0][i].Center().x - 1.5f*m_colored_squares[0][i].GetSprite().getLocalBounds().width;
        y = m_colored_squares[0][i].GetPosition().y;
        labelh.setPosition(x,y);

        //Horizontal line
        sf::Vertex lineh[] =
        {
            sf::Vertex(m_colored_squares[0][i].Center()),
            sf::Vertex(m_colored_squares[r_goban.Dimension()-1][i].Center()),
        };

        sf::Text labelv;
        labelv.setCharacterSize(12);
        labelv.setFont(m_font);
        labelv.setString(m_labels[i]);
        labelv.setColor(sf::Color::Red);

        x = m_colored_squares[i][0].Center().x-4;
        y = m_colored_squares[i][0].Center().y - 1.5f*m_colored_squares[i][0].GetSprite().getLocalBounds().height;
        labelv.setPosition(x,y);

        //Vertical line
        sf::Vertex linev[] =
        {
            sf::Vertex(m_colored_squares[i][0].Center()),
            sf::Vertex(m_colored_squares[i][r_goban.Dimension()-1].Center())

        };

        r_target.draw(labelh);

        x = m_colored_squares[r_goban.Dimension()-1][i].Center().x + m_colored_squares[r_goban.Dimension()-1][i].GetSprite().getLocalBounds().width;
        y = m_colored_squares[r_goban.Dimension()-1][i].GetPosition().y;

        labelh.setPosition(x,y);
        r_target.draw(labelh);
        r_target.draw(lineh, 2, sf::Lines);

        r_target.draw(labelv);
        x = m_colored_squares[i][r_goban.Dimension()-1].Center().x-4;
        y = m_colored_squares[i][r_goban.Dimension()-1].Center().y +m_colored_squares[0][i].GetSprite().getLocalBounds().height;

        labelv.setPosition(x,y);
        r_target.draw(labelv);

        r_target.draw(linev, 2, sf::Lines);
    }

    //Now, the points (markers)
    std::vector<sf::Sprite>::iterator board_points_it;
    for(board_points_it = m_points_sprites.begin(); board_points_it!= m_points_sprites.end();++board_points_it)
    {
        r_target.draw(*board_points_it);
    }

    //Finally the stones
    for(unsigned int i = 0; i < m_stone_sprites.size(); ++i)
    {
        for(unsigned int j =0; j < m_stone_sprites[i].size(); ++j)
        {
            //Draw stone if there is one.
            if(r_goban.GetStone(i,j) == BLACK)
            {
                m_stone_sprites[i][j].setTexture(m_blackTexture);
            }else if(r_goban.GetStone(i,j) == WHITE)
            {
                m_stone_sprites[i][j].setTexture(m_whiteTexture);
            }else if (l_move_advisor_map[i][j] == 'b')
            {
                m_stone_sprites[i][j].setTexture(m_blackAdvisorTexture);
            }else if (l_move_advisor_map[i][j] == 'w')
            {
                m_stone_sprites[i][j].setTexture(m_whiteAdvisorTexture);
            }else if (l_move_advisor_map[i][j] == 'm')
            {
                m_stone_sprites[i][j].setTexture(m_mixedAdvisorexture);
            }
            else
            {                
                continue;
            }

            sf::Vector2f cs_center = m_colored_squares[i][j].Center();
            m_stone_sprites[i][j].setPosition(cs_center.x - (m_stone_sprites[i][j].getLocalBounds().width/2),cs_center.y - (m_stone_sprites[i][j].getLocalBounds().height/2));
            r_target.draw(m_stone_sprites[i][j]);
        }
    }

    /** Drawing the Goban Statistics*/

    sf::Text l_statistics;
    l_statistics.setCharacterSize(9);
    l_statistics.setFont(m_font);
    l_statistics.setColor(sf::Color::White);

    // define a 120x50 rectangle
    sf::RectangleShape rectangle(sf::Vector2f(200, 50));
    rectangle.setPosition(460, 40);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(1);


    //======== influence Power Box =========
    r_target.draw(rectangle);

    //Influence Amount Black
    l_statistics.setString("Black influence power: " + to_string( abs(r_statistics.GetInfluenceAmount(BLACK, r_goban, r_influence))));
    l_statistics.setPosition(470,50);
    r_target.draw(l_statistics);

    //Influence Amount White
    l_statistics.setString("White influence power: " + to_string( abs(r_statistics.GetInfluenceAmount(WHITE, r_goban, r_influence) )));
    l_statistics.setPosition(470,70);
    r_target.draw(l_statistics);



    //======= influenced Cells Box =========
    rectangle.setPosition(460, 100);
    r_target.draw(rectangle);

    // Black Influenced Cells
    l_statistics.setString("Black influenced cells: " + to_string(r_statistics.GetTotalInfluencedCells(BLACK, r_goban, r_influence)));
    l_statistics.setPosition(470,110);
    r_target.draw(l_statistics);

    //White Influenced Cells
    l_statistics.setString("White influenced cells: " + to_string(r_statistics.GetTotalInfluencedCells(WHITE, r_goban, r_influence)));
    l_statistics.setPosition(470,130);
    r_target.draw(l_statistics);


    //======= Board Dominance Box =========
    rectangle.setPosition(460, 160);
    r_target.draw(rectangle);

    // Black Influenced Cells
    l_statistics.setString("Black board dominance: " + to_string(r_statistics.GetTotalInfluencedCells(BLACK, r_goban, r_influence) /  (pow(r_goban.GetGoban().size(),2) ) * 100)+ " %");
    l_statistics.setPosition(470,170);
    r_target.draw(l_statistics);

    //White Influenced Cells
    l_statistics.setString("White board dominance: " + to_string(r_statistics.GetTotalInfluencedCells(WHITE, r_goban, r_influence) / (pow(r_goban.GetGoban().size(),2)  )  * 100 )+ " %");
    l_statistics.setPosition(470,190);
    r_target.draw(l_statistics);

    //======= Territory Estimative ==========
    rectangle.setPosition(460, 220);
    r_target.draw(rectangle);

    // Black Influenced Cells
    l_statistics.setString("Black territory estimative: " + to_string(r_statistics.GetEstimativeTerritory(BLACK, r_goban, r_influence)* 0.9f ));
    l_statistics.setPosition(470,230);
    r_target.draw(l_statistics);

    //White Influenced Cells
    l_statistics.setString("White territory estimative: " + to_string( r_statistics.GetEstimativeTerritory(WHITE, r_goban, r_influence) * 0.9f ));
    l_statistics.setPosition(470,250);
    r_target.draw(l_statistics);

    //======= Influence Efficiency ========== If is not efficient maybe it is overconcentrated
    double efficiency =0;
    rectangle.setPosition(460, 280);
    r_target.draw(rectangle);

    // Black Influenced Cells
    efficiency = ( (r_statistics.GetTotalInfluencedCells(BLACK, r_goban, r_influence) ) / (pow(r_goban.GetGoban().size(),2) * (r_goban.CountStones(BLACK)+1)) );
    l_statistics.setString("Black efficiency per stone: " + to_string( efficiency* 100) + " %");
    l_statistics.setPosition(470,290);
    r_target.draw(l_statistics);

    //White Influenced Cells
    efficiency = ( (r_statistics.GetTotalInfluencedCells(WHITE, r_goban, r_influence) ) / (pow(r_goban.GetGoban().size(),2) * (r_goban.CountStones(WHITE)+1)) );
    l_statistics.setString("White efficiency per stone: " + to_string( efficiency* 100) + " %");
    l_statistics.setPosition(470,310);
    r_target.draw(l_statistics);

    //======= Prisoners ==========
    rectangle.setPosition(460, 340);
    r_target.draw(rectangle);

    // Black Prisoners
    l_statistics.setString("Black prisoners: " + to_string( r_goban.GetBlackPrisoners()) );
    l_statistics.setPosition(470,350);
    r_target.draw(l_statistics);

    //White Prisoners
    l_statistics.setString("White prisoners: " + to_string( r_goban.GetWhitePrisoners()) );
    l_statistics.setPosition(470,370);
    r_target.draw(l_statistics);

}
//================================================================================
