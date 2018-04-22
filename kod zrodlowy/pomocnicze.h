#pragma once

#include <SFGUI\SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/FileResourceLoader.hpp>
#include <SFML/Graphics.hpp>


struct Dane
{
	Dane();
	Dane(int wys, int szer, int miny);

	int	wysokosc;
	int	szerokosc;
	int	ilosc_min;

	Dane & operator= (const Dane & n);
};


class Komorka
{
public:
	Komorka();

	sfg::Button::Ptr	k_przycisk;
	bool				k_mina;
	bool				k_flaga;

};


class Matrix
{
public:
	std::shared_ptr<Komorka>	at(int w, int k);
	void						PobierzDane(int w, int k, int m);
	void						DodajKom(int w, int k);
	void						UstawRozmiar(const int & w, const int & k);

	sfg::Table::Ptr		_table;
	int					_szerokosc;
	int					_wysokosc;

private:
	std::vector<std::shared_ptr<Komorka>>	_plansza;

	int			_wiersze;
	int			_kolumny;
	int			_ilosc_min;

};
