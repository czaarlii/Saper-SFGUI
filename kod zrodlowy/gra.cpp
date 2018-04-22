#include "aktywne_okno.h"


Gra::Gra(const Dane & param) :
	G				('G'),
	mina			('$'),
	flaga			('%'),
	g_parametry		(param),
	g_zmiana_okna	(false),
	g_pozostalo		(sfg::Label::Create()),
	g_koniec		(sfg::Button::Create("Menu")),
	g_restart		(sfg::Button::Create("Od nowa")),
	g_table			(sfg::Table::Create()),
	g_window		(sfg::Window::Create())

{
	// ustawienia graficzne gry
	sfg::Context::Get().GetEngine().SetProperty("*#flaga", "Color", sf::Color::Red);
	sfg::Context::Get().GetEngine().SetProperty("*#pole", "Color", sf::Color(160, 160, 160));
	sfg::Context::Get().GetEngine().SetProperty("*#pole1", "Color", sf::Color::Blue);
	sfg::Context::Get().GetEngine().SetProperty("*#pole2", "Color", sf::Color::Green);
	sfg::Context::Get().GetEngine().SetProperty("*#pole3", "Color", sf::Color::Red);
	sfg::Context::Get().GetEngine().SetProperty("*#pole4", "Color", sf::Color(75, 29, 92));
	sfg::Context::Get().GetEngine().SetProperty("*#pole5", "Color", sf::Color(131, 11, 13));
	sfg::Context::Get().GetEngine().SetProperty("*#pole6", "Color", sf::Color::Cyan);
	sfg::Context::Get().GetEngine().SetProperty("*#pole7", "Color", sf::Color::Black);
	sfg::Context::Get().GetEngine().SetProperty("*#pole8", "Color", sf::Color(222, 242, 2));
	sfg::Context::Get().GetEngine().SetProperty("*#mina", "Color", sf::Color::Black);
	sfg::Context::Get().GetEngine().SetProperty("*#pole0", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole1", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole2", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole3", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole4", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole5", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole6", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole7", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole8", "BackgroundColor", sf::Color(142, 142, 142));
	sfg::Context::Get().GetEngine().SetProperty("*#pole", "BackgroundColor", sf::Color(185, 184, 202));
	sfg::Context::Get().GetEngine().SetProperty("*#flaga", "BackgroundColor", sf::Color(185, 184, 202));
	sfg::Context::Get().GetEngine().SetProperty("*#mina", "BackgroundColor", sf::Color(185, 184, 202));

	//przygotowanie planszy
	g_koniec->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Gra::OnMenuClicked, this));

	g_restart->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Gra::OnRestartClicked, this));

	auto pasek_menu = sfg::Table::Create();
	pasek_menu->Attach(sfg::Label::Create("Miny: "), sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	pasek_menu->Attach(g_pozostalo, sf::Rect<sf::Uint32>(1, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);

	auto przyciski = sfg::Table::Create();
	przyciski->SetColumnSpacings(5.f);
	przyciski->Attach(g_restart, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	przyciski->Attach(g_koniec, sf::Rect<sf::Uint32>(1, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);

	g_plansza.PobierzDane(g_parametry.wysokosc, g_parametry.szerokosc, g_parametry.ilosc_min);

	StworzPlansze();
	UstawMiny();

	auto szerokosc_planszy = g_plansza._table->GetRequisition();
	auto rozmiar_menu = pasek_menu->GetRequisition();
	rozmiar_menu.x = szerokosc_planszy.x;
	pasek_menu->SetRequisition(rozmiar_menu);


	g_table->SetRowSpacings(5.f);
	g_table->Attach(g_plansza._table, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	g_table->Attach(pasek_menu, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	g_table->Attach(przyciski, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);

	// Inicjalizujemy teraz okno aplikacji
	g_window->SetTitle("Nowa gra");
	g_window->Add(g_table);
}


///******************************************Metody klasy Gra**********************************

const sf::Vector2u Gra::GetWindowSize()
{
	auto r = g_window->GetRequisition();
	return static_cast<const sf::Vector2u>(r);
}


sfg::Window::Ptr Gra::SetWindow()
{
	return g_window;
}


bool Gra::CzyZmienicOkno()
{
	return g_zmiana_okna;
}


Dane Gra::PrzekazDane(const Dane & dane)
{
	g_parametry = dane;
	return g_parametry;
}


char Gra::Okno()
{
	return G;
}


void Gra::PrzekazKursor(const sf::Vector2i & kursor)
{
	g_kursor = kursor;
}


std::pair<int, int> Gra::OdczytajWspolrzedne()
{
	auto poz_mf = sf::Vector2f((float)g_kursor.x - 11.f, (float)g_kursor.y - 35.f);

	float sz = poz_mf.x / 24.f, wy = poz_mf.y / 24.f;
	int szer = (int)sz, wys = (int)wy;
	std::pair<int, int> pr(szer, wys);
	return pr;
}


void Gra::OnMenuClicked()
{
	g_zmiana_okna = true;
}


void Gra::OnRestartClicked()
{
	for (int w = 0; w < g_parametry.wysokosc; w++)
	{
		for (int k = 0; k < g_parametry.szerokosc; k++)
		{
			g_plansza.at(w, k)->k_flaga = false;
			g_plansza.at(w, k)->k_mina = false;
			g_plansza.at(w, k)->k_przycisk->SetState(sfg::Widget::State::NORMAL);
			g_plansza.at(w, k)->k_przycisk->SetLabel("");
			g_plansza.at(w, k)->k_przycisk->SetId("pole");
		}
	}

	UstawMiny();
}


void Gra::StworzPlansze()
{
	g_plansza._table = sfg::Table::Create();

	for (int w = 0; w < g_parametry.wysokosc; w++)
	{
		for (int k = 0; k < g_parametry.szerokosc; k++)
		{
			g_plansza.DodajKom(w, k);
			g_plansza.at(w, k)->k_przycisk->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Gra::OnKomLClicked, this));
			g_plansza.at(w, k)->k_przycisk->GetSignal(sfg::Widget::OnRightClick).Connect(std::bind(&Gra::OnKomPClicked, this));
			g_plansza._table->Attach(g_plansza.at(w, k)->k_przycisk, sf::Rect<sf::Uint32>(k, w, 1, 1));
		}
	}

	g_plansza._szerokosc = 24 * g_parametry.szerokosc - 1;
	g_plansza._wysokosc = 24 * g_parametry.wysokosc - 1;
}


void Gra::UstawMiny()
{
	srand(time(NULL));
	g_pozostalo_min = g_parametry.ilosc_min;
	g_pozostalo_pol = g_parametry.szerokosc*g_parametry.wysokosc - g_parametry.ilosc_min;

	std::vector< std::pair<int, int>> pola;
	pola.reserve(g_parametry.szerokosc*g_parametry.wysokosc);
	std::pair<int, int> para;


	for (int w = 0; w < g_parametry.wysokosc; w++)
	{
		for (int k = 0; k < g_parametry.szerokosc; k++)
		{
			para.first = w;
			para.second = k;
			pola.push_back(para);
		}
	}

	int pole, ilosc = pola.size();
	std::vector<std::pair<int, int>>::iterator it = pola.begin();

	for (int i = 0; i < g_parametry.ilosc_min; i++)
	{
		pole = std::rand() % (ilosc);
		it = pola.begin() + pole;
		g_plansza.at(pola[pole].first, pola[pole].second)->k_mina = true;
		pola.erase(it);
		ilosc--;
	}
	g_pozostalo->SetText(std::to_string(g_pozostalo_min));
}


void Gra::ZliczMiny(int w, int k)
{
	if (g_plansza.at(w, k)->k_przycisk->GetState() == sfg::Widget::State::INSENSITIVE || g_plansza.at(w, k)->k_flaga == true)
		return;

	int ile = 0;
	std::vector<bool> jest;

	for (int i = 0; i < 8; i++)
		jest.push_back(true);

	if (k - 1 < 0) { jest[0] = false; jest[3] = false; jest[5] = false; }
	if (w - 1 < 0) { jest[0] = false; jest[1] = false; jest[2] = false; }
	if (k + 1 >= g_parametry.szerokosc) { jest[2] = false; jest[4] = false; jest[7] = false; }
	if (w + 1 >= g_parametry.wysokosc) { jest[5] = false; jest[6] = false; jest[7] = false; }

	if (jest[0] == true)
		if (g_plansza.at(w - 1, k - 1)->k_mina == true)
			ile++;

	if (jest[1] == true)
		if (g_plansza.at(w - 1, k)->k_mina == true)
			ile++;

	if (jest[2] == true)
		if (g_plansza.at(w - 1, k + 1)->k_mina == true)
			ile++;

	if (jest[3] == true)
		if (g_plansza.at(w, k - 1)->k_mina == true)
			ile++;

	if (jest[4] == true)
		if (g_plansza.at(w, k + 1)->k_mina == true)
			ile++;

	if (jest[5] == true)
		if (g_plansza.at(w + 1, k - 1)->k_mina == true)
			ile++;

	if (jest[6] == true)
		if (g_plansza.at(w + 1, k)->k_mina == true)
			ile++;

	if (jest[7] == true)
		if (g_plansza.at(w + 1, k + 1)->k_mina == true)
			ile++;

	g_plansza.at(w, k)->k_przycisk->SetLabel(WypiszIle(w, k, ile));
	g_plansza.at(w, k)->k_przycisk->SetState(sfg::Widget::State::INSENSITIVE);
	g_pozostalo_pol--;

	if (ile == 0)
	{
		if (jest[0] == true) ZliczMiny(w - 1, k - 1);
		if (jest[1] == true) ZliczMiny(w - 1, k);
		if (jest[2] == true) ZliczMiny(w - 1, k + 1);
		if (jest[3] == true) ZliczMiny(w, k - 1);
		if (jest[4] == true) ZliczMiny(w, k + 1);
		if (jest[5] == true) ZliczMiny(w + 1, k - 1);
		if (jest[6] == true) ZliczMiny(w + 1, k);
		if (jest[7] == true) ZliczMiny(w + 1, k + 1);
	}
}


sf::String Gra::WypiszIle(int w, int k, int ile)
{
	sf::String sel;

	switch (ile)
	{
	case 0: sel = "pole0"; break;
	case 1: sel = "pole1"; break;
	case 2: sel = "pole2"; break;
	case 3: sel = "pole3"; break;
	case 4: sel = "pole4"; break;
	case 5: sel = "pole5"; break;
	case 6: sel = "pole6"; break;
	case 7: sel = "pole7"; break;
	case 8: sel = "pole8"; break;
	}

	g_plansza.at(w, k)->k_przycisk->SetId(sel);
	if (ile != 0)
		sel = std::to_string(ile);
	else
		sel = "";
	return sel;
}


void Gra::KoniecGry(bool wygrana)
{
	switch (wygrana)
	{
	case true:
		g_pozostalo->SetText("WYGRALES!");
		break;
	case false:
		g_pozostalo->SetText("PRZEGRALES");
		break;
	}

	for (int w = 0; w < g_parametry.wysokosc;w++)
		for (int k = 0; k < g_parametry.szerokosc; k++)
		{
			if (wygrana == false && g_plansza.at(w, k)->k_mina == true && g_plansza.at(w, k)->k_flaga == false)
			{
				g_plansza.at(w, k)->k_przycisk->SetLabel(mina);
				g_plansza.at(w, k)->k_przycisk->SetId("mina");
			}
			else if (g_plansza.at(w, k)->k_flaga == true && g_plansza.at(w, k)->k_mina == false)
			{
				g_plansza.at(w, k)->k_przycisk->SetLabel("X");
			}

			g_plansza.at(w, k)->k_przycisk->SetState(sfg::Widget::State::INSENSITIVE);
		}
}


void Gra::OnKomLClicked()
{
	std::pair<int, int> wspolrzedne = OdczytajWspolrzedne();

	if (g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_flaga == true)
		return;

	if (g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_mina == true)
	{
		KoniecGry(false);
	}
	else
	{
		ZliczMiny(wspolrzedne.second, wspolrzedne.first);
	}


	if (g_pozostalo_pol == 0 && g_pozostalo_min == 0)
		KoniecGry(true);
}


void Gra::OnKomPClicked()
{
	std::pair<int, int> wspolrzedne = OdczytajWspolrzedne();

	if (g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_flaga == false)
	{
		if (g_pozostalo_min == 0)
			return;

		g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_przycisk->SetLabel(flaga);
		g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_flaga = true;
		g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_przycisk->SetId("flaga");
		g_pozostalo_min--;
	}
	else
	{
		g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_przycisk->SetLabel("");
		g_plansza.at(wspolrzedne.second, wspolrzedne.first)->k_flaga = false;
		g_pozostalo_min++;
	}

	g_pozostalo->SetText(std::to_string(g_pozostalo_min));

	if (g_pozostalo_pol == 0 && g_pozostalo_min == 0)
		KoniecGry(true);
}
