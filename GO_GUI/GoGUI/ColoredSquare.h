//
//  ColoredSquare.h
//  GoInfluenceMap
//
//  Created by Giordano Bruno on 25/01/14.
//
//

#ifndef __GoInfluenceMap__ColoredSquare__
#define __GoInfluenceMap__ColoredSquare__

#include <SFML/Graphics.hpp>
#include "HSL.hpp"

class ColoredSquare
{
public:
    ColoredSquare();
    ColoredSquare(const HSL& r_hslColor);
    ColoredSquare(const ColoredSquare& copy_colored_square);
    ~ColoredSquare();

    void Draw(sf::RenderTarget& r_target) const;
    void SetPosition(const sf::Vector2f& r_position);
    void ChangeValue(const int influence);
    const sf::Sprite &GetSprite() const;
    sf::Vector2f Center();
    sf::Vector2f GetPosition();

    ColoredSquare& operator=(const ColoredSquare& colored_square);

        //Image builded in memory

private:
    void CreateTexture();
    void UpdateTexture();

    //Pixels of the texture
    sf::Uint8* mp_pixels;
    sf::Texture m_texture;
    //Sprite to be drawed
    sf::Sprite m_sprite;
    //Color of the square
    sf::Color m_squareColor;
    //HSL Color of the square
    HSL m_hslSquareColor;
};

#endif /* defined(__GoInfluenceMap__ColoredSquare__) */
