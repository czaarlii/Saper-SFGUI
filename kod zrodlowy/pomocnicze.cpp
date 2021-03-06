#include "pomocnicze.h"


Dane::Dane()
{}

Dane::Dane(	int wys, 
			int szer, 
			int miny) 
	: wysokosc	(wys)
	, szerokosc	(szer)
	, ilosc_min	(miny)
{}


Dane & Dane::operator= (const Dane & n)
{
	this->szerokosc	= n.szerokosc;
	this->wysokosc = n.wysokosc;
	this->ilosc_min = n.ilosc_min;
	return *this;
}


///****************************************************Metody klasy Komorka i Matrix*************************


Komorka::Komorka() 
	: k_mina		(false)
	, k_flaga		(false)
	, k_przycisk	(sfg::Button::Create())
{
	k_przycisk->SetRequisition(sf::Vector2f(24.f, 24.f));
}


std::shared_ptr<Komorka> Matrix::at(int w, int k)
{
	return _plansza[w * _kolumny + k];
}


void Matrix::DodajKom(int w, int k)
{
	_plansza.push_back(std::make_shared<Komorka>());
	at(w, k)->k_przycisk->SetId("pole");
}


void Matrix::PobierzDane(int w, int k, int m)
{
	_wiersze = w;
	_kolumny = k;
	_ilosc_min = m;
}


void Matrix::UstawRozmiar(const int & w, const int & k)
{
	_szerokosc = 24 * k - 1;
	_wysokosc = 24 * w - 1;
}
