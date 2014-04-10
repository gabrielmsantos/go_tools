//
//  ColoredSquare.cpp
//  GoInfluenceMap
//
//  Created by Giordano Bruno on 25/01/14.
//
//

#include "ColoredSquare.h"
#include "../Config.h"

//=================================================================================
ColoredSquare::ColoredSquare()
{
    *this = ColoredSquare(HSL(0,100,0));
}
//=================================================================================
ColoredSquare& ColoredSquare::operator=(const ColoredSquare& colored_square)
{
    m_hslSquareColor = colored_square.m_hslSquareColor;

    mp_pixels = new sf::Uint8[SPRITE_WIDTH*SPRITE_HEIGHT*4];

    m_squareColor = m_hslSquareColor.TurnToRGB();

    CreateTexture();

    return *this;
}
//=================================================================================
//Copy Constructor
ColoredSquare::ColoredSquare(const ColoredSquare& copy_colored_square) :
    m_hslSquareColor(copy_colored_square.m_hslSquareColor)
{
    mp_pixels = new sf::Uint8[SPRITE_WIDTH*SPRITE_HEIGHT*4];

    m_squareColor = m_hslSquareColor.TurnToRGB();

    CreateTexture();
}

//=================================================================================
ColoredSquare::ColoredSquare(const HSL& r_hslColor) :
    m_hslSquareColor(r_hslColor)
{
    mp_pixels = new sf::Uint8[SPRITE_WIDTH*SPRITE_HEIGHT*4];

    m_squareColor = m_hslSquareColor.TurnToRGB();

    CreateTexture();
}
//=================================================================================
ColoredSquare::~ColoredSquare()
{
    delete[] mp_pixels;
}
//=================================================================================
void ColoredSquare::Draw(sf::RenderTarget& r_target) const
{
    r_target.draw(m_sprite);
}
//=================================================================================
void ColoredSquare::SetPosition(const sf::Vector2f& r_position)
{
    m_sprite.setPosition(r_position);
}
//=================================================================================
void ColoredSquare::ChangeValue(const int influence)
{
    int l_spriteInfluence = 0;
    if (influence > 0) {
        m_hslSquareColor.Hue = 215;
        l_spriteInfluence = influence;
    }
    else
    {
        m_hslSquareColor.Hue = 23;
        l_spriteInfluence = influence * -1;
    }

    m_hslSquareColor.Luminance = 5 * l_spriteInfluence;
    if (m_hslSquareColor.Luminance > 50) {
        m_hslSquareColor.Luminance = 50;
    }
    m_squareColor = m_hslSquareColor.TurnToRGB();
    UpdateTexture();
}
//=================================================================================
void ColoredSquare::CreateTexture()
{
    m_texture.create(SPRITE_WIDTH, SPRITE_HEIGHT);
    m_sprite = sf::Sprite(m_texture);

    UpdateTexture();
}
//=================================================================================
void ColoredSquare::UpdateTexture()
{
    for(register int i = 0; i < SPRITE_WIDTH*SPRITE_HEIGHT*4; i += 4) {
        mp_pixels[i] = m_squareColor.r;
        mp_pixels[i+1] = m_squareColor.g;
        mp_pixels[i+2] = m_squareColor.b;
        mp_pixels[i+3] = m_squareColor.a;
    }

    m_texture.update(mp_pixels);
}
//=================================================================================
sf::Vector2f ColoredSquare::Center()
{
    return sf::Vector2f(m_sprite.getPosition().x+ m_sprite.getLocalBounds().width/2, m_sprite.getPosition().y+ m_sprite.getLocalBounds().height/2);
}
//=================================================================================
const sf::Sprite& ColoredSquare::GetSprite() const
{
    return this->m_sprite;
}
//=================================================================================
sf::Vector2f ColoredSquare::GetPosition()
{
    return m_sprite.getPosition();
}
//=================================================================================
