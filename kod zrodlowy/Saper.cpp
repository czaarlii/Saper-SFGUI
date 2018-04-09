#include "Saper.h"


Saper::Saper():
	m_okno (new Menu)
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


Saper::~Saper() 
{
	delete m_okno;
}


///**********************************Metody******************************************


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


void Saper::Run()
{
	sf::Event event;
	sf::Clock clock;
	while (m_render_window->isOpen()) 
	{

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
