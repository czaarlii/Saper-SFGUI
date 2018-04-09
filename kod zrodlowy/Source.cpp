#include "header.h"

#include <cctype>

///*****************************Konstruktory**************************************

Saper::Saper()
{
	// Ustawienia graficzne aplikacji
	std::shared_ptr<sf::Font> m_czcionka;
	m_czcionka = std::make_shared<sf::Font>();
	std::string s("czcionka.ttf");
	m_czcionka->loadFromFile(s);

	m_desktop.GetEngine().GetResourceManager().SetDefaultFont(m_czcionka);
	m_desktop.SetProperty("*", "FontName", s);
	m_desktop.SetProperty("*", "FontSize", 14.f);

	m_desktop.SetProperty("Window", "TitleBackgroundColor", sf::Color(40, 33, 220));
	m_desktop.SetProperty("Window", "BackgroundColor", sf::Color(6, 4, 50));
	m_desktop.SetProperty("ComboBox", "BackgroundColor", sf::Color(0, 100, 240));
	m_desktop.SetProperty("ComboBox", "HighlightedColor", sf::Color(0, 157, 182));
	m_desktop.SetProperty("ComboBox", "ArrowColor", sf::Color(40, 33, 220));
	m_desktop.SetProperty("Button", "BackgroundColor", sf::Color(0, 100, 240));

	// Inicjalizacja g³ównego okna
	m_render_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(0, 0), "Saper");
	
	m_desktop.Add(m_okno->SetWindow());

	//Ustawienie rozmiaru okna
	m_render_window->setSize(m_okno->GetWindowSize());

	// Inicjalizacja biblioteki rysuj¹cej
	m_render_window->resetGLStates();
}


Menu::Menu():
_wysokosc(8), _szerokosc(8), _ilosc_min(10), _niestandardowe(false), _zmiana_okna(false)
{
	m_combo_box = sfg::ComboBox::Create();
	m_combo_box->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&Menu::OnComboSelect, this));

	m_blad = sfg::Label::Create();

	tekst.setString("Rozpocznij gre");
	m_start = sfg::Button::Create(tekst.getString());
	m_start->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Menu::OnStartClicked, this));

	m_combo_box->AppendItem("Maly");
	m_combo_box->AppendItem("Sredni");
	m_combo_box->AppendItem("Duzy");
	m_combo_box->AppendItem("Niestandardowy");

	auto table1 = sfg::Table::Create();
	table1->SetColumnSpacings(1.f);
	table1->SetRowSpacings(8.f);

	auto label = sfg::Label::Create("Wybierz rozmiar planszy:");
	label->SetId("tekst");

	table1->Attach(label, sf::Rect<sf::Uint32>(0, 0, 3, 1), sfg::Table::FILL, sfg::Table::FILL);
	table1->Attach(m_combo_box, sf::Rect<sf::Uint32>(3, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	table1->Attach(m_start, sf::Rect<sf::Uint32>(0, 1, 2, 1), sfg::Table::FILL, sfg::Table::FILL);
	table1->Attach(m_blad, sf::Rect<sf::Uint32>(2, 1, 2, 1), sfg::Table::FILL, sfg::Table::FILL);

	m_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_box->Pack(table1);

	// Inicjalizujemy teraz okno aplikacji
	m_window = sfg::Window::Create();
	m_window->SetTitle("Menu glowne");
	m_window->Add(m_box);

	auto rozmiar = m_window->GetRequisition();
	rozmiar.y += 30.f;
	m_window->SetRequisition(rozmiar);
}

///**********************************Metody******************************************

sfg::Window::Ptr Menu::SetWindow()
{
	return m_window;
}

const sf::Vector2u Menu::GetWindowSize()
{
	auto r = m_window->GetRequisition();
	return static_cast<const sf::Vector2u>(r);
}

char Menu::Okno()
{
	return M;
}

void Saper::SetRenderSize()
{
	m_render_window->setSize(m_okno->GetWindowSize());
}

void Saper::ZmienOkno()
{
	if (m_okno->Okno() == 'M')
	{
		Dane s_parametry = m_okno->PrzekazDane();
		m_desktop.Remove(m_okno->SetWindow());
		delete m_okno;
		m_desktop.Refresh();
		m_okno = new Gra(s_parametry);
		m_desktop.Add(m_okno->SetWindow());
	}
	else if (m_okno->Okno() == 'G')
	{
		m_desktop.Remove(m_okno->SetWindow());
		delete m_okno;
		m_desktop.Refresh();
		m_okno = new Menu;
		m_desktop.Add(m_okno->SetWindow());
	}
}

Dane Menu::PrzekazDane(const Dane & dane)
{
	Dane parametry;
	parametry.wysokosc = _wysokosc;
	parametry.szerokosc = _szerokosc;
	parametry.ilosc_min = _ilosc_min;
	return parametry;
}

void Menu::OnComboSelect()
{
	std::string s = m_combo_box->GetSelectedText();

	if (s == "Maly")
	{
		_wysokosc = 8;
		_szerokosc = 8;
		_ilosc_min = 10;
	}
	if (s == "Sredni")
	{
		_wysokosc = 16;
		_szerokosc = 16;
		_ilosc_min = 40;
	}
	if (s == "Duzy")
	{
		_wysokosc = 16;
		_szerokosc = 30;
		_ilosc_min = 100;
	}

	if (_niestandardowe == true)
	{
		m_wys->SetText(int2string(_wysokosc));
		m_szer->SetText(int2string(_szerokosc));
		m_miny->SetText(int2string(_ilosc_min));
	}

	if (s == "Niestandardowy")
	{
		if (_niestandardowe==false)
			PokazNiestandardowe();
		_niestandardowe = true;
	}
}

void Menu::OnStartClicked()
{
	SprawdzDane();
}

void Menu::SprawdzDane()
{
	if (_niestandardowe == true)
	{
		std::string liczba = m_wys->GetText();

		if (CzyLiczba(liczba) == false || liczba=="0" || liczba=="00")
		{
			m_blad->SetText("Nieprawidlowe dane");
			return;
		}
		if (liczba.size() == 0)
		{
			m_blad->SetText("Za malo danych.");
			return;
		}
		if (std::stoi(liczba) > 25)
		{
			m_blad->SetText("Za wysoka plansza.");
			return;
		}
		_wysokosc = std::stoi(liczba);

		liczba = m_szer->GetText();
		if (CzyLiczba(liczba) == false || liczba == "0" || liczba == "00")
		{
			m_blad->SetText("Nieprawidlowe dane");
			return;
		}
		if (liczba.size() == 0)
		{
			m_blad->SetText("Za malo danych.");
			return;
		}
		if (std::stoi(liczba) <= 4)
		{
			m_blad->SetText("Za waska plansza.");
			return;
		}
		if (std::stoi(liczba) > 55)
		{
			m_blad->SetText("Za szeroka plansza.");
			return;
		}
		_szerokosc = std::stoi(liczba);

		liczba = m_miny->GetText();
		if (CzyLiczba(liczba) == false || liczba == "0" || liczba == "00" || liczba == "000")
		{
			m_blad->SetText("Nieprawidlowe dane");
			return;
		}
		if (liczba.size() == 0)
		{
			m_blad->SetText("Za malo danych.");
			return;
		}
		_ilosc_min = std::stoi(liczba);

		if (_ilosc_min >= _wysokosc * _szerokosc)
		{
			m_blad->SetText("Za duzo min.");
			return;
		}
	}

	_zmiana_okna = true;
}

bool Menu::CzyLiczba(std::string s)
{
	for (char i : s)
	{
		if (!isdigit(i))
			return false;
	}
	return true;
}

bool Menu::CzyZmienicOkno()
{
	return _zmiana_okna;
}

void Menu::PokazNiestandardowe()
{
	m_wys = sfg::Entry::Create();
	m_wys->SetMaximumLength(2);
	m_wys->SetText(int2string(_wysokosc));
	m_szer = sfg::Entry::Create();
	m_szer->SetMaximumLength(2);
	m_szer->SetText(int2string(_szerokosc));
	m_miny = sfg::Entry::Create();
	m_miny->SetMaximumLength(3);
	m_miny->SetText(int2string(_ilosc_min));

	auto table2 = sfg::Table::Create();
	table2->SetRowSpacings(6.f);
	table2->SetColumnSpacings(10.f);

	table2->Attach(sfg::Label::Create("Wysokosc"), sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	table2->Attach(m_wys, sf::Rect<sf::Uint32>(1, 0, 4, 1), sfg::Table::FILL, sfg::Table::FILL);
	table2->Attach(sfg::Label::Create("Szerokosc"), sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	table2->Attach(m_szer, sf::Rect<sf::Uint32>(1, 1, 4, 1), sfg::Table::FILL, sfg::Table::FILL);
	table2->Attach(sfg::Label::Create("Ilosc min"), sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
	table2->Attach(m_miny, sf::Rect<sf::Uint32>(1, 2, 4, 1), sfg::Table::FILL, sfg::Table::FILL);

	auto frame = sfg::Frame::Create("Ustawienia niestandardowe");
	frame->Add(table2);

	m_box->Pack(frame, false, false);
	m_box->RefreshAll();
	m_window->RefreshAll();

	_niestandardowe = true;

	
}

std::string Menu::int2string(int n)
{
	return std::to_string(n);
}

void Saper::Run()
{
	sf::Event event;
	sf::Clock clock;
	while (m_render_window->isOpen()) {
		// Sprawdzamy czy nast¹pi³o zdarzenie, jeœli tak, to je obs³ugujemy
		while (m_render_window->pollEvent(event)) {
			m_desktop.HandleEvent(event);

			// Jeœli zakmniêto okno, to zakañczamy program
			if (event.type == sf::Event::Closed) {
				m_render_window->close();
			}

		}

		if (m_okno->CzyZmienicOkno() == true)
			ZmienOkno();

		if (m_okno->Okno() == 'G')
		{
			m_okno->PrzekazKursor(m_mysz.getPosition(*m_render_window));
		}

		SetRenderSize();

		// Funkcja odœwie¿aj¹ca widgety na pulpicie
		m_desktop.Update(clock.restart().asSeconds());

		// Komendy rysuj¹ce aplikacjê
		m_render_window->clear();
		m_sfgui.Display(*m_render_window);
		m_render_window->display();
	}
}