#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Planet.h"
#include "Vektor2d.h"

// Simulationsgeschwindigkeit
const double DT = 100.0;

class GameWindow : public Gosu::Window
{
	Gosu::Image rakete;
	double x, y, angle;
public:
	
	GameWindow()
		: Window(800, 600),
		rakete("rakete.png") //direkt beim initialisieren mit bild laden
	{
		set_caption("Mein Gosu Tutorial Game mit Git");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt --> KEINE LOGIK!!
	void draw() override
	{
		graphics().draw_line(
			10, 20, Gosu::Color::RED,
			200,100,Gosu::Color::GREEN,
			0.0
		);
		/*graphics().draw_triangle(
			x, y, Gosu::Color::YELLOW,
			x+10, y+30, Gosu::Color::FUCHSIA,
			x+25, y+22, Gosu::Color::WHITE,
			0.0
		);*/
		rakete.draw_rot(x, y, 0.0, angle, 0.5, 0.0);
	}
	// Wird 60x pro Sekunde aufgerufen --> HIER LOGIK!
	void update() override
	{
		if (input().down(Gosu::ButtonName::KB_UP)) { y -= 5; angle = 0; }
		if (input().down(Gosu::ButtonName::KB_DOWN)) { y += 5; angle = 180; }
		if (input().down(Gosu::ButtonName::KB_RIGHT)) { x += 5; angle = 90; }
		if (input().down(Gosu::ButtonName::KB_LEFT)) { x -= 5; angle = -90; }

		if (input().down(Gosu::ButtonName::KB_ESCAPE)) { this->close(); }

	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}