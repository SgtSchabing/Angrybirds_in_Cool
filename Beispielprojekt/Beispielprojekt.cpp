#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"

// Simulationsgeschwindigkeit
const double DT = 100.0;

class GameWindow : public Gosu::Window
{
	Gosu::Image ball;
	Gosu::Image fire_circle;
	Gosu::Song game;


	double x, y, xSpeed, ySpeed,x_c = 1800,y_c, x_c_default = 1800;
	const unsigned int w_width = 1500;
	const unsigned int w_height = 900;
	const double c_height = 690 * 0.2;
	const unsigned int schleuderspitze_x = 230;
	const unsigned int schleuderspitze_y = w_height - 200;
	const double xSpeedCorrection = 0.2;
	const double ySpeedCorrection = 0.3;
	const double gravity = 1.5;
	const double wind = 6;
	bool isFlying = false;
	int score;
	bool checkedforCollision;
	bool durchRinggeflogen;
	int lifs = 4;



public:
	
	GameWindow()
		: Window(1500, 900),
		ball("planet3.png"), //direkt beim initialisieren mit bild laden
		fire_circle("fire_circle.png"),
		game("game.mp3")
	{
	set_caption("Angry Ballz");
	}

	


	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt --> KEINE LOGIK!!
	void draw() override
	{
		
		Gosu::Font::Font(80, "DS-DIGITAL").draw(std::to_string(score) + "    " + std::to_string(lifs) , 10, 25, 0.0, 1, 1, Gosu::Color::GREEN);
	

		graphics().draw_quad(							//Schleuderstab
			220, w_height, Gosu::Color::YELLOW,
			240, w_height, Gosu::Color::YELLOW,
			220, w_height - 200, Gosu::Color::YELLOW,
			240, w_height - 200, Gosu::Color::YELLOW, 0.0);

		ball.draw_rot(x, y, 0.0, 0, 0.5, 0.5, 0.05, 0.05);

		fire_circle.draw_rot(x_c, y_c, 0.0, 0, 0.5, 0.5, 0.08, 0.2);
		

		if (!isFlying) {		//ball nicht unterwegs, an schleuder
		
			graphics().draw_line(
				x, y, Gosu::Color::WHITE, schleuderspitze_x, schleuderspitze_y, Gosu::Color::WHITE, 0.0);

			
		}


		else {		//Ball losgeschossen, keine steuerung

			game.play();
			
		}
	}
	// Wird 60x pro Sekunde aufgerufen --> HIER LOGIK!
	void update() override
	{
		


		y_c = 300;						//Ring fliegt durch die Gegend
		x_c = x_c - wind;
		if (x_c < 0)					 //nochmal fliegen
		{
			x_c = x_c_default;
		}


		if (!isFlying) {				//fliegt nicht, eingabe
			x = input().mouse_x();
			y = input().mouse_y();
			if (input().down(Gosu::ButtonName::MS_LEFT)) {
				isFlying = true;
				ySpeed = (schleuderspitze_y - y)*ySpeedCorrection;
				xSpeed = (schleuderspitze_x - x)*xSpeedCorrection;
			}
		}

		else {							//fliegt, keine eingabe
			if (input().down(Gosu::ButtonName::MS_RIGHT)) { isFlying = false; score = 0; checkedforCollision = false; lifs = 4; }
			else {
				y = y + ySpeed;
				x = x + xSpeed;
				ySpeed = ySpeed + gravity;

				//Kollisionsabfrage blauer kreis
				if ((x_c < x) && !checkedforCollision) {

					checkedforCollision = true;

					if ((abs(y_c - y) < c_height / 2)) {	//durch ring geflogen
						score++;
						isFlying = false;
						checkedforCollision = false;
					}
				}

				if ((y > w_height + 150) && (lifs > 0)) {
					isFlying = false;
					checkedforCollision = false;
					lifs--;
				}

			}
		}
		if (input().down(Gosu::ButtonName::KB_ESCAPE)) { this->close(); }
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();


}
