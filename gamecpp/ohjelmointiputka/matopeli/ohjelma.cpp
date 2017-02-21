// ohjelma.cpp
#include "ohjelma.hpp"
#include <iostream>
#include <stdexcept>
#include <SDL2/SDL.h>

namespace ohjelma {
	// Staattisia, siis vain tämän tiedoston käyttöön.
	static SDL_Surface *ruutu;
	static void piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta = false);
	namespace kuvat {
		// Funktio kuvan lataukseen ja virheen heittämiseen.
		static SDL_Surface *lataa(const char *nimi, bool lapinakyva);

		// Kuvat.
		static SDL_Surface *tausta_valikko, *tausta_peli;
		static SDL_Surface *valikko_peli, *valikko_peli_valittu;
		static SDL_Surface *valikko_lopetus, *valikko_lopetus_valittu;
		static SDL_Surface *valikko_pistemaara;
		static SDL_Surface *omena, *matopallo, *reunapala;
	}
}

// Alustusfunktio.
void ohjelma::alku() {
	std::clog << "ohjelma::alku()" << std::endl;
	// Alustetaan SDL tai heitetään virhe.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
	// Avataan ikkuna tai heitetään virhe.
	ruutu = SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF);
	if (!ruutu) {
		throw std::runtime_error(SDL_GetError());
	}
	// Asetetaan otsikko.
	SDL_WM_SetCaption("Matopeli", "Matopeli");

	// Ladataan kuvat tai heitetään virhe.
	kuvat::tausta_valikko = kuvat::lataa("kuvat/tausta_valikko.bmp", false);
	kuvat::tausta_peli = kuvat::lataa("kuvat/tausta_peli.bmp", false);
	kuvat::valikko_peli = kuvat::lataa("kuvat/valikko_peli.bmp", true);
	kuvat::valikko_peli_valittu = kuvat::lataa("kuvat/valikko_peli_valittu.bmp", true);
	kuvat::valikko_lopetus = kuvat::lataa("kuvat/valikko_lopetus.bmp", true);
	kuvat::valikko_lopetus_valittu = kuvat::lataa("kuvat/valikko_lopetus_valittu.bmp", true);
	kuvat::valikko_pistemaara = kuvat::lataa("kuvat/valikko_pistemaara.bmp", true);
	kuvat::omena = kuvat::lataa("kuvat/omena.bmp", true);
	kuvat::matopallo = kuvat::lataa("kuvat/matopallo.bmp", true);
	kuvat::reunapala = kuvat::lataa("kuvat/reunapala.bmp", true);
}

// Lopetusfunktio.
void ohjelma::loppu() {
	std::clog << "ohjelma::loppu()" << std::endl;
	// Vapautetaan kuvat.
	SDL_FreeSurface(kuvat::tausta_valikko);
	SDL_FreeSurface(kuvat::tausta_peli);
	SDL_FreeSurface(kuvat::valikko_peli);
	SDL_FreeSurface(kuvat::valikko_peli_valittu);
	SDL_FreeSurface(kuvat::valikko_lopetus);
	SDL_FreeSurface(kuvat::valikko_lopetus_valittu);
	SDL_FreeSurface(kuvat::valikko_pistemaara);
	SDL_FreeSurface(kuvat::omena);
	SDL_FreeSurface(kuvat::matopallo);
	SDL_FreeSurface(kuvat::reunapala);
	// Suljetaan SDL.
	SDL_Quit();
}

// Lukee seuraavan napinpainalluksen.
ohjelma::nappi ohjelma::odota_nappi() {
	// Odotellaan, kunnes tulee napinpainallus.
	SDL_Event e;
	while (SDL_WaitEvent(&e)) {
		if (e.type != SDL_KEYDOWN) continue;
		switch (e.key.keysym.sym) {
			case SDLK_ESCAPE: return NAPPI_ESCAPE;
			case SDLK_RETURN: return NAPPI_ENTER;
			case SDLK_LEFT: return NAPPI_VASEN;
			case SDLK_RIGHT: return NAPPI_OIKEA;
			default: return NAPPI_MUU;
		}
	}
	// Jokin meni pieleen!
	throw std::runtime_error(SDL_GetError());
}

// Kertoo napin nykytilan.
bool ohjelma::lue_nappi(nappi n) {
	// Käsketään SDL:n hoitaa viestit, jolloin sen tieto napeista päivittyy.
	SDL_PumpEvents();

	// Tarkistetaan pyydetty nappi.
	Uint8 *napit = SDL_GetKeyState(0);
	switch (n) {
		case NAPPI_VASEN: return napit[SDLK_LEFT];
		case NAPPI_OIKEA: return napit[SDLK_RIGHT];
		case NAPPI_ENTER: return napit[SDLK_RETURN];
		case NAPPI_ESCAPE: return napit[SDLK_ESCAPE];
		default: return false;
	}
}

// Tyhjentää syötepuskurin.
void ohjelma::tyhjenna_syote() {
	SDL_Event e;
	while (SDL_PollEvent(&e));
}

// Lataa kuvan ja optimoi sen piirtoa varten.
static SDL_Surface *ohjelma::kuvat::lataa(const char *nimi, bool lapinakyva) {
	// Jos lataus onnistuu...
	if (SDL_Surface *tmp = SDL_LoadBMP(nimi)) {
		// Asetetaan läpinäkyvä väri (magenta eli pinkki).
		if (lapinakyva) {
			SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(tmp->format, 255, 0, 255));
		}
		// Yritetään optimoida.
		if (SDL_Surface *opti = SDL_DisplayFormat(tmp)) {
			// Tuhotaan alkuperäinen ja palautetaan optimoitu.
			SDL_FreeSurface(tmp);
			tmp = opti;
		}
		// Palautetaan kuva.
		return tmp;
	}
	// Muuten heitetään virhe.
	throw std::runtime_error(SDL_GetError());
}

// Piirtää yhden kuvan.
static void ohjelma::piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta) {
	SDL_Rect r = {x, y};
	if (keskikohta) {
		r.x -= kuva->w / 2;
		r.y -= kuva->h / 2;
	}
	SDL_BlitSurface(kuva, 0, ruutu, &r);
}

// Piirtää valikon.
void ohjelma::piirra_valikko(int pelin_tulos, valikko::valinta valittu) {
	std::clog << "ohjelma::piirra_valikko(tulos, valittu)" << std::endl;

	// Valitaan oikeat kuvat.
	SDL_Surface *kuva_peli = kuvat::valikko_peli;
	SDL_Surface *kuva_lopetus = kuvat::valikko_lopetus;
	switch (valittu) {
		case valikko::PELI:
			kuva_peli = kuvat::valikko_peli_valittu;
			break;
		case valikko::LOPETUS:
			kuva_lopetus = kuvat::valikko_lopetus_valittu;
			break;
	}

	// Piirretään.
	piirra_kuva(kuvat::tausta_valikko, 0, 0);

	// Ensimmäisen tekstin vasemman yläkulman sijainti, (16, 16).
	int x = 16, y = 16;

	// Päivitetään y-koordinaattia joka tekstin jälkeen
	// niin, että tekstit asettuvat siististi allekkain.
	piirra_kuva(kuva_peli, x, y);
	y += kuva_peli->h;

	piirra_kuva(kuva_lopetus, x, y);
	y += kuva_lopetus->h;

	piirra_kuva(kuvat::valikko_pistemaara, x, y);
	y += kuvat::valikko_pistemaara->h;

	// Jaetaan pistemäärä numeroiksi ja käsitellään nolla fiksusti.
	int numerot[10], maara = 0;
	for (int i = pelin_tulos; i != 0; i /= 10) {
		numerot[maara] = i % 10;
		++maara;
	}
	if (maara == 0) {
		numerot[0] = 0;
		++maara;
	}
	// Tulostetaan teksti 5x5 pisteen (3x5 + välit) digitaalinumeroilla.
	for (int i = 0; i < maara; ++i) {
		// Taulukko digitaalinumeroiden pisteistä.
		const bool diginum[10][5][5] = {
			#include "numerot.inc"
		};
		// Luvun numerot ovat taulukossa käänteisessä järjestyksessä.
		int n = numerot[maara - i - 1];
		// Piirretään diginum-taulukon mukaan 5x5-ruudukkoon palloja.
		for (int iy = 0; iy < 5; ++iy) {
			// Oikea y-sijainti lasketaan pallon kohdasta ja alkukohdasta (y).
			const int y_paikka = y + (int)(iy * kuvat::omena->w);
			for (int ix = 0; ix < 5; ++ix) {
				if (!diginum[n][iy][ix]) continue;
				// Oikea x-sijainti lasketaan pallon kohdasta (ix)
				// ja merkin indeksistä (i) sekä alkukohdasta (x).
				const int x_paikka = x + (int) ((0.5 * ix + 4 * i) * kuvat::omena->w);
				piirra_kuva(kuvat::omena, x_paikka, y_paikka);
			}
		}
	}

	// Laitetaan piirustukset esille.
	SDL_Flip(ruutu);
}
