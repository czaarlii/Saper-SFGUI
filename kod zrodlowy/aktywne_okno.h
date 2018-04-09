#pragma once

#include "pomocnicze.h"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFGUI/FileResourceLoader.hpp>

#include <cstdlib>
#include <ctime>


class Aktywne_okno
{
public:

	virtual const sf::Vector2u		GetWindowSize() = 0;
	virtual	sfg::Window::Ptr		SetWindow() = 0;
	virtual bool					CzyZmienicOkno() = 0;
	virtual Dane					PrzekazDane(const Dane & dane = { 0,0,0 }) = 0;
	virtual char					Okno() = 0;
	virtual void					PrzekazKursor(const sf::Vector2i & kursor) {}

	virtual ~Aktywne_okno() {};

};


class Menu : public Aktywne_okno
{
public:

	Menu();

	virtual const sf::Vector2u	GetWindowSize();
	virtual sfg::Window::Ptr	SetWindow();
	virtual bool				CzyZmienicOkno();
	virtual Dane				PrzekazDane(const Dane & dane = { 0, 0, 0 });
	virtual char				Okno();

	~Menu() {}

private:

	void			OnComboSelect();
	void			OnStartClicked();
	std::string		int2string(int n);
	void			PokazNiestandardowe();
	void			SprawdzDane();
	bool			CzyLiczba(std::string s);

	int			_wysokosc;
	int			_szerokosc;
	int			_ilosc_min;
	bool		_niestandardowe;
	bool		_zmiana_okna;

	const char	M = 'M';

	sfg::Window::Ptr	m_window;
	sfg::Label::Ptr		info;
	sfg::Label::Ptr		m_blad;
	sfg::ComboBox::Ptr	m_combo_box;
	sfg::Entry::Ptr		m_wys;
	sfg::Entry::Ptr		m_szer;
	sfg::Entry::Ptr		m_miny;

	sfg::Box::Ptr		m_box;
	sfg::Button::Ptr	m_start;
	sf::Text			tekst;
};


class Gra : public Aktywne_okno
{
public:

	Gra(const Dane & param);
	//~Gra() {}

	virtual const sf::Vector2u		GetWindowSize();
	virtual sfg::Window::Ptr		SetWindow();
	virtual bool					CzyZmienicOkno();
	virtual Dane					PrzekazDane(const Dane & dane = { 0, 0, 0 });
	virtual char					Okno();
	virtual void					PrzekazKursor(const sf::Vector2i & kursor);

private:

	void				StworzPlansze();
	void				UstawMiny();
	std::pair<int, int>	OdczytajWspolrzedne();
	void				ZliczMiny(int w, int k);
	void				KoniecGry(bool wygrana);
	sf::String			WypiszIle(int w, int k, int ile);

	void OnKomLClicked();
	void OnKomPClicked();
	void OnMenuClicked();
	void OnRestartClicked();

	Dane			g_parametry;
	int				g_pozostalo_min;
	int				g_pozostalo_pol;
	Matrix			g_plansza;
	bool			g_zmiana_okna;
	const char		G = 'G';
	const char		mina = '$';
	const char		flaga = '%';
	sf::Vector2i	g_kursor;


	sfg::Window::Ptr	g_window;
	sfg::Label::Ptr		g_pozostalo;
	sfg::Button::Ptr	g_restart;
	sfg::Button::Ptr	g_koniec;
	sfg::Box::Ptr		g_box;
	sfg::Table::Ptr		g_table;

};

