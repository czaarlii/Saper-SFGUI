#ifndef Gra_hpp
#define Gra_hpp

#include "klasy.h"
#include "Header.h"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

class Komorka
{
	const short int mina = 15, flaga = 16;

	sfg::Button::Ptr _przycisk;
	bool _mina, _flaga;
};

class Matrix
{
//	vector
};

class Gra : public Aktywne_okno
{
public:
	virtual const sf::Vector2u GetWindowSize();
	virtual sfg::Window::Ptr SetWindow();
	virtual bool CzyZmienicOkno();
//	Dane PrzekazDane(){ Dane p; return Dane; }

	Gra();

private:
	sfg::Window::Ptr g_window;

};

#endif