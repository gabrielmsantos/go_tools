#include "SGFParser.h"
#include <stack>
#include <boost/lexical_cast.hpp>

//============================================================
SGFParser::SGFParser()
{
}
//============================================================

void SGFParser::ParseGame(std::ifstream& in_file, GoGame* l_go_game)
{
    // get length of file:
    in_file.seekg (0, in_file.end);
    int length = in_file.tellg();
    in_file.seekg (0, in_file.beg);

    //Loading file into buffer
    char l_buffer[length];
    in_file.read(l_buffer, length);

    std::stack<GoGameNode*> l_node_check_points;

    bool game_started = false;
    bool reading_property = false;
    bool reading_value = false;
    bool escape_mode = false;

    std::string l_property;
    std::string l_value;

    for(int i =0; i<length;++i)
    {

        //Starting the game
        if(l_buffer[i] == '(' && !reading_value)
        {
            GoGameNode* l_node = l_go_game->GetGameTree().GetCurrentNode();
            //std::cout << "! checkpointing "<< l_node << std::endl;

            l_node_check_points.push( l_node);

            if(!game_started)
            {
                reading_property = true;
                game_started = true;
            }

        }
        else if (!game_started)
        {
            continue;

        }
        else if ( l_buffer[i] == ')' &&!reading_value )
        {
            GoGameNode* l_node = l_node_check_points.top();
            //@TODO: SHOULD GET BACK TO NODE l_node in GoGame (BACK_TO_NODE)
            //std::cout << "Back to node " << l_node << std::endl;
            l_node_check_points.pop();

            if(l_node_check_points.empty())
            {
                //END GAME
                //Just Read one game per SGF for now.
                break;
            }
        }
        else if(reading_property)
        {
            if(l_buffer[i] == '[')
            {
                reading_value = true;
                reading_property = false;

            }else
            {
                if(l_buffer[i] != '\n' && l_buffer[i] != '\r' && l_buffer[i] != ';')
                    l_property+=l_buffer[i];
            }
        }
        else if(reading_value)
        {
            if(l_buffer[i]== '\\' && !escape_mode)
            {
                escape_mode = true;
            }
            else if(l_buffer[i] == ']' && !escape_mode)
            {
                //Finished reading value
                reading_value = false;

                //Process property and value
                Process(l_property, l_value, l_go_game);

                //Back to reading property
                reading_property = true;
            }
            else
            {
                l_value += l_buffer[i];

                if(escape_mode)
                {
                    escape_mode = false;
                }
            }
        }
    }

}
//============================================================
void SGFParser::Process(std::string& property, std::string& value, GoGame* l_go_game)
{
//    std::cout << property << " << " << value << std::endl;

    GoGameInfo& game_info = l_go_game->GetGameInfo();
    char move_row;
    char move_column;

    if(property == "B")//Black Plays
    {
        move_column = value[0];
        move_row = value[1];

        int column = ( (int)move_column - (int)'a');
        int row = ( (int)move_row - (int)'a');
        l_go_game->PlayMove(column,row, BLACK);

    }
    else if (property == "W")//White Plays
    {

        move_column = value[0];
        move_row = value[1];

        int column = ( (int)move_column - (int)'a');
        int row = ( (int)move_row - (int)'a');

        l_go_game->PlayMove(column,row, WHITE);

    }
    else if(property == "PB")//Name of black player
    {
        game_info.m_black_player = value;

    }else if(property == "PW")//Name of White player
    {
        game_info.m_white_player = value;

    }else if(property == "KM")//Komi
    {
        double komi;
        try
        {
            komi = boost::lexical_cast<double>(value) * 10;
        }
        catch (boost::bad_lexical_cast const&)
        {
            komi = 0;
        }
        game_info.m_komi10 = komi;

    }else if(property == "BR")//black rating
    {
        game_info.m_black_rating = value;

    }else if(property == "WR")//white rating
    {
        game_info.m_white_rating = value;
    }
    else if(property == "RE")//result
    {
        game_info.m_game_result = value;
    }
    else if(property == "DT")//date
    {
        game_info.m_date = value;
    }

    property.clear();
    value.clear();
}
