#ifndef _MAIN_HPP
#define _MAIN_HPP

#include <iostream>
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "const.hpp"
#include "menu.hpp"
#include "game.hpp"
#include "normalGame.hpp"
#include "timedGame.hpp"

void restart(Game ** game, sf::Font &font);

#endif