#ifndef Header_hpp
#define Header_hpp

#include "klasy.h"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFGUI/FileResourceLoader.hpp>

#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <utility>

class Komorka
{
public:
	sfg::Button::Ptr k_przycisk;
	bool k_mina, k_flaga;

	Komorka() 
	: k_mina(false), k_flaga(false)
	{
		k_przycisk = sfg::Button::Create();
		k_przycisk->SetRequisition(sf::Vector2f(24.f, 24.f));
	}
};


class Matrix
{
public:
	void PobierzDane(int w, int k, int m);
	Komorka* at(int w, int k) const;
	void DodajKom(int w, int k);
	void UstawRozmiar(const int & w, const int & k);

	sfg::Table::Ptr _table;
	int _szerokosc, _wysokosc;

	~Matrix();
	
private:
	int _wiersze, _kolumny, _ilosc_min ;
	std::vector<Komorka*> _plansza;
	
};



struct Dane
{
	int wysokosc, szerokosc, ilosc_min;

	Dane & operator= (const Dane & n)
	{
		this->szerokosc = n.szerokosc;
		this->wysokosc = n.wysokosc;
		this->ilosc_min = n.ilosc_min;
		return *this;
	}
};

///******************************************** okna ******************************************

class Aktywne_okno
{
public:
	virtual const sf::Vector2u GetWindowSize() = 0;
	virtual sfg::Window::Ptr SetWindow() = 0;
	virtual bool CzyZmienicOkno() = 0;
	virtual Dane PrzekazDane(const Dane & dane = {0,0,0})=0;
	virtual char Okno() = 0;
	virtual void PrzekazKursor(const sf::Vector2i & kursor) {}

	virtual ~Aktywne_okno(){};
	
};

class Gra : public Aktywne_okno
{
public:
	virtual const sf::Vector2u GetWindowSize();
	virtual sfg::Window::Ptr SetWindow();
	virtual bool CzyZmienicOkno();
	virtual Dane PrzekazDane(const Dane & dane = { 0, 0, 0 });
	virtual char Okno();
	virtual void PrzekazKursor(const sf::Vector2i & kursor);

	Gra(const Dane & param);

	~Gra() {}
	
private:
	Dane g_parametry;
	int g_pozostalo_min, g_pozostalo_pol;
	Matrix g_plansza;
	bool g_zmiana_okna;
	const char G = 'G', mina = '$', flaga = '%';
	sf::Vector2i g_kursor;
	
	void StworzPlansze();
	void UstawMiny();
	std::pair<int,int> OdczytajWspolrzedne();
	void ZliczMiny(int w, int k);
	void KoniecGry(bool wygrana);
	sf::String WypiszIle(int w, int k, int ile);

	void OnKomLClicked();
	void OnKomPClicked();
	void OnMenuClicked();
	void OnRestartClicked();

	sfg::Window::Ptr g_window;
	sfg::Label::Ptr g_pozostalo;
	sfg::Button::Ptr g_restart, g_koniec;
	sfg::Box::Ptr g_box;
	sfg::Table::Ptr g_table;
	
};

class Menu: public Aktywne_okno
{
public:
	Menu();

	virtual const sf::Vector2u GetWindowSize();
	virtual sfg::Window::Ptr SetWindow();
	virtual bool CzyZmienicOkno();
	virtual Dane PrzekazDane(const Dane & dane = { 0, 0, 0 });
	virtual char Okno();

	~Menu() {}

private:
	int _wysokosc, _szerokosc, _ilosc_min;
	bool _niestandardowe, _zmiana_okna;
	const char M = 'M';

	void OnComboSelect();
	void OnStartClicked();
	std::string int2string(int n);
	void PokazNiestandardowe();
	void SprawdzDane();
	bool CzyLiczba(std::string s);

	sfg::Window::Ptr m_window;
	sfg::Label::Ptr info, m_blad;
	sfg::ComboBox::Ptr m_combo_box;
	sfg::Entry::Ptr m_wys, m_szer, m_miny;

	sfg::Box::Ptr m_box;
	sfg::Button::Ptr m_start;
	sf::Text tekst;
};

///************************************************klasa Saper********************************************

class Saper
{
public:
	Saper();
	void Run();
	
	~Saper(){ 
		delete m_okno; 
	}

private:
	Aktywne_okno *m_okno=new Menu;
	
	void SetRenderSize();
	void ZmienOkno();

	/// Obiekt SFGUI, potrzebny do inicjalizacji biblioteki
	sfg::SFGUI m_sfgui;

	sf::Mouse m_mysz;
	sfg::Desktop m_desktop;
	std::shared_ptr<sf::RenderWindow> m_render_window;
};

#endif