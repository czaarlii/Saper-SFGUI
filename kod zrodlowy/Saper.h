#pragma once

#include "aktywne_okno.h"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFGUI/FileResourceLoader.hpp>

#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <cctype>


class Saper
{
public:
	Saper();
	~Saper();

	void Run();

private:
	void SetRenderSize();
	void ZmienOkno();

	std::unique_ptr<Aktywne_okno>		m_okno;
	sfg::SFGUI							m_sfgui;	/// Obiekt SFGUI, potrzebny do inicjalizacji biblioteki

	sf::Mouse							m_mysz;
	sfg::Desktop						m_desktop;
	std::shared_ptr<sf::RenderWindow>	m_render_window;
};
