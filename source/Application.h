/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <SFML/Window.hpp>


class Application
{
public:
	int run(int argc, char* argv[]);
	
	sf::Window window;
};