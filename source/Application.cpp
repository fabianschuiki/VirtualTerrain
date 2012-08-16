/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include <SFML/OpenGL.hpp>

#include "Application.h"
#include "ShaderProgram.h"


int Application::run(int argc, char* argv[])
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	
	std::ofstream f("test.txt");
	f.write("Hello", 6);
	f.close();
	
	//Setup the window.
	window.create(sf::VideoMode(1280, 768), "VirtualTerrain", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	
	//Load shaders.
	ShaderProgram bypassShader("data/bypass.vert", "data/bypass.frag");
	ShaderProgram normalsShader("data/normals.vert", "data/normals.frag");
	
	//Start the clock.
	sf::Clock clock;
	
	//Setup OpenGL state.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	//Main loop.
	GLUquadric* quadric = gluNewQuadric();
	float aspect = 1280.0 / 768;
	float angle = M_PI, z = 5, y = 1;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized) {
				aspect = (float)event.size.width / event.size.height;
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::TextEntered) {
				switch (event.text.unicode) {
					case 'r': {
						normalsShader.reload();
					} break;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) y += 0.5;
				if (event.key.code == sf::Keyboard::Down) y -= 0.5;
			}
		}
		
		//Measure time.
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		
		//Animate the angle.
		angle += M_PI / 5 * dt;
		
		//Draw stuff.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		normalsShader.use();
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, aspect, 1, 100);
		gluLookAt(z*sin(angle), y, z*cos(angle),  0, 0, 0,  0, 1, 0);
		
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(5, 5, -5);
		gluSphere(quadric, 0.2, 4, 8);
		
		glLoadIdentity();
		glColor3f(1, 0, 0);
		gluSphere(quadric, 1, 18, 36);
		
		glTranslatef(2, 0, 0);
		glColor3f(0, 2, 0);
		gluSphere(quadric, 1, 18, 36);
		
		window.display();
	}
	
	return 0;
}