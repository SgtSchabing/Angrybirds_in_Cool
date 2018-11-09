#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include <stdlib.h>
#include <time.h>

#include "Vektor2d.h"

// Simulationsgeschwindigkeit
const double DT = 100.0;


class GameWindow : public Gosu::Window
{
	Gosu::Image ball;
	Gosu::Image fire_circle;
	Gosu::Image heart;
	Gosu::Song game;
	Gosu::Song lose;
	Gosu::Sample treffer, herz, hintenangekommen;
	Gosu::Font digifont, digifont_big, digifont2;

	double x, y, xSpeed, ySpeed,x_c = 1600,y_c = 300, x_c_default = 1600;
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
	int lifs = 5;
	int random = 1;
	bool beh = false;
	bool beh2 = false;
	int ringcounter=1;


public:
	
	GameWindow()
		: Window(1500, 900),
		ball("planet3.png"), //direkt beim initialisieren mit bild laden
		fire_circle("fire_circle.png"),
		game("game.mp3"),
		lose("lose.wav"),
		heart("heart.png"),
		digifont(80, ".//DS-DIGI.TTF"),
		digifont_big(300, ".//DS-DIGI.TTF"), 
		digifont2(50, ".//DS-DIGI.TTF"),
		treffer("treffer.wav"),
		herz("herz.wav"), 
		hintenangekommen("hintenangekommen.wav")
	{
	set_caption("Angry Ballz");
	digifont_big.text_width("Game");
	digifont_big.text_width("Over");
	digifont2.text_width("Press enter to continue");
	}
	

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt --> KEINE LOGIK!!
	void draw() override
	{
		digifont.draw(std::to_string(score), 10, 25, 0.0, 1, 1, Gosu::Color::GREEN);

		
		

		graphics().draw_quad(							//Schleuderstab
			220, w_height, Gosu::Color::YELLOW,
			240, w_height, Gosu::Color::YELLOW,
			220, w_height - 200, Gosu::Color::YELLOW,
			240, w_height - 200, Gosu::Color::YELLOW, 0.0);

		ball.draw_rot(x, y, 0.0, 0, 0.5, 0.5, 0.05, 0.05);

		if (ringcounter % 4 == 0) {
			heart.draw_rot(x_c, y_c, 0.0, 0, 0.5, 0.5, 0.2, 0.2);
		}
		else {
			fire_circle.draw_rot(x_c, y_c, 0.0, 0, 0.5, 0.5, 0.08, 0.2);
		}
		

		switch (lifs) {
		default: {digifont_big.draw_rel("Game", w_width / 2, 450, 1.0, 0.5, 1.0, 1.0, 1.0, Gosu::Color::RED); digifont_big.draw_rel("Over", w_width / 2, 450+digifont_big.height(), 1.0, 0.5, 1.0, 1.0, 1.0, Gosu::Color::RED); digifont2.draw_rel("Press enter to continue", w_width / 2, 450 + digifont_big.height()+50, 1.0, 0.5, 1.0, 1.0, 1.0, Gosu::Color::WHITE);break; }
		case 1: {heart.draw_rot(w_width - 50, 25, 0, 0, 1, 0, 0.1, 0.1); break; }
		case 2: {heart.draw_rot(w_width - 50, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.1, 25, 0, 0, 1, 0, 0.1, 0.1); break; }
		case 3: {heart.draw_rot(w_width - 50, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width() * 0.1, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.2, 25, 0, 0, 1, 0, 0.1, 0.1); break; }
		case 4: {heart.draw_rot(w_width - 50, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.1, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.2, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.3, 25, 0, 0, 1, 0, 0.1, 0.1); break; }
		case 5: {heart.draw_rot(w_width - 50, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.1, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.2, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.3, 25, 0, 0, 1, 0, 0.1, 0.1); heart.draw_rot(w_width - 50 - heart.width()*0.4, 25, 0, 0, 1, 0, 0.1, 0.1); break; }
		}

		if (!isFlying) {		//ball nicht unterwegs, an schleuder
			if (lifs > 0) {
				graphics().draw_line(x, y, Gosu::Color::WHITE, schleuderspitze_x, schleuderspitze_y, Gosu::Color::WHITE, 0.0);
			}
		}


		else {		//Ball losgeschossen, keine steuerung
			
			
		}
	}


	// Wird 60x pro Sekunde aufgerufen --> HIER LOGIK!
	void update() override
	{
		if (lifs > 5) { lifs = 5; }
	
		if (beh == false)
		{
			srand(int(time(NULL))); //Zufallsgenerator initialisieren
			beh = true;
		}
							
		x_c = x_c - wind;				//Kreis bewegt sich
		if ((x_c < 0)&&(lifs>0))					 //nochmal fliegen
		{
			x_c = x_c_default;
			random = rand() % 700 + 50;
			y_c = random;
			ringcounter++;
			lifs-- ;
			hintenangekommen.play(1, 1, false);
		}

		if (lifs > 0)
		{
			game.play(true);
		}
		else
		{
			if (beh2 == false)
			{
				game.stop();
				lose.play(false);
				beh2 = true;
			}
			isFlying = true;
			
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
			if (input().down(Gosu::ButtonName::KB_RETURN)) 
			{ 
				isFlying = false; 
				score = 0; 
				checkedforCollision = false; 
				lifs = 5; 
				beh2 = false;
				ringcounter = 1;
				x_c = x_c_default;					//nochmal fliegen
				random = rand() % 700 + 50;
				y_c = random;
			}
			else {
				y = y + ySpeed;
				x = x + xSpeed;
				ySpeed = ySpeed + gravity;

				//Kollisionsabfrage blauer kreis
				if ((x_c < x) && !checkedforCollision) {

					checkedforCollision = true;

					if (abs(y_c - y) < c_height / 2) {	//durch ring geflogen
						
						if (ringcounter % 4 == 0) { 
							lifs++;
							herz.play();
						}
						else {
							score++;
							treffer.play();
						}
						isFlying = false;
						checkedforCollision = false;
						x_c = x_c_default;					//nochmal fliegen
						random = rand() % 700 + 50;
						y_c = random;
						ringcounter++;
					}
				}

				if ((y > w_height + 150)) {
					isFlying = false;
					checkedforCollision = false;
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
