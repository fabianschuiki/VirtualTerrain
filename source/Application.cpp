/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include <SFML/OpenGL.hpp>

#include "Application.h"
#include "Framebuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"


static void glaux_dumpError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "*** error " << (int)err << std::endl;
}


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
	ShaderProgram extractHighlightsShader("data/bypass.vert", "data/extract_highlights.frag");
	
	//Start the clock.
	sf::Clock clock;
	
	//Setup OpenGL state.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	//Initialize the framebuffer.
	glEnable(GL_TEXTURE_2D);
	Framebuffer sceneFBO;
	Texture sceneColor(window.getSize().x, window.getSize().y, GL_RGBA16F_ARB, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	Texture sceneDepth(window.getSize().x, window.getSize().y, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	sceneFBO.attachTexture(GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, sceneColor.texture);
	sceneFBO.attachTexture(GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, sceneDepth.texture);
	sceneFBO.validate();
	
	//Camera
	float rotation = M_PI, inclination = M_PI / 8;
	float radius = 15;
	
	//Main loop.
	GLUquadric* quadric = gluNewQuadric();
	float aspect = 1280.0 / 768;
	int mouseX, mouseY;
	bool mouseDown = false;
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
						extractHighlightsShader.reload();
					} break;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				mouseX = event.mouseButton.x;
				mouseY = event.mouseButton.y;
				mouseDown = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				mouseDown = false;
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				radius = std::max<float>(std::min<float>(radius + event.mouseWheel.delta*0.1, 50), 2);
			}
		}
		if (mouseDown) {
			sf::Vector2i v = sf::Mouse::getPosition(window);
			rotation    += (v.x - mouseX) * 0.005;
			inclination = std::max<float>(std::min<float>(inclination + (v.y - mouseY) * 0.005, M_PI/2*0.99), -M_PI/2*0.99);
			mouseX = v.x;
			mouseY = v.y;
		}
		
		//Measure time.
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		
		//Draw the scene into the FBO.
		sceneFBO.bind();
		glDisable(GL_TEXTURE_2D);
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, aspect, 1, 100);
		
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(radius * sin(rotation) * cos(inclination),
				  radius * sin(inclination),
				  radius * cos(rotation) * cos(inclination),  0, 0, 0,  0, 1, 0);
		
		glUseProgram(0);
		glPushMatrix();
		glTranslatef(5, 5, -5);
		glColor3f(1, 1, 0);
		gluSphere(quadric, 0.2, 4, 8);
		glPopMatrix();
		
		normalsShader.use();
		glPushMatrix();
		glColor3f(1, 0, 0);
		gluSphere(quadric, 1, 18, 36);
		
		glTranslatef(4, 0, 0);
		glColor3f(0, 1, 0);
		gluSphere(quadric, 1, 18, 36);
		
		glTranslatef(-4, 3, 0);
		glColor3f(0, 1, 0);
		gluSphere(quadric, 1, 18, 36);
		glPopMatrix();
		
		
		//Draw the FBO onto the screen.
		sceneFBO.unbind();
		glColor3f(1, 1, 1);
		glUseProgram(0);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glEnable(GL_TEXTURE_2D);
		
		sceneColor.bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(-1, 1);
		glTexCoord2f(1, 1); glVertex2f( 0, 1);
		glTexCoord2f(1, 0); glVertex2f( 0, 0);
		glTexCoord2f(0, 0); glVertex2f(-1, 0);
		glEnd();
		
		sceneDepth.bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(0, 1);
		glTexCoord2f(1, 1); glVertex2f(1, 1);
		glTexCoord2f(1, 0); glVertex2f(1, 0);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glEnd();
		
		sceneColor.bind();
		extractHighlightsShader.use();
		glUniform1i(extractHighlightsShader.uniform("tex"), 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(-1,  0);
		glTexCoord2f(1, 1); glVertex2f( 0,  0);
		glTexCoord2f(1, 0); glVertex2f( 0, -1);
		glTexCoord2f(0, 0); glVertex2f(-1, -1);
		glEnd();
		
		window.display();
	}
	
	return 0;
}