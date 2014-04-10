#ifndef IGOBANOBSERVER_H
#define IGOBANOBSERVER_H

#include <SFML/System/Vector2.hpp>

class I_GobanObserver{
public:
    virtual ~I_GobanObserver(){}
    virtual void Notify(const sf::Vector2i& pos, int infl_value) = 0;
};
#endif // IGOBANOBSERVER_H
