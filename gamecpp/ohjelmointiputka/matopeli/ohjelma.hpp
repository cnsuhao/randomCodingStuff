// ohjelma.hpp
#ifndef _OHJELMA_HPP
#define _OHJELMA_HPP

#include "valikko.hpp"
#include "peli.hpp"

namespace ohjelma {
	// Funktiot ohjelman aloitukseen ja lopetukseen.
	void alku();
	void loppu();
	
	// Pelissä tarvittavat näppäimet; voitaisiin käyttää myös suoraan
	// esimerkiksi int-tyyppiä ja SDL.h:n näppäinkoodeja (SDLK_*).
	enum nappi {
		NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU
	};
	// Funktiot painalluksen odotukseen ja napin nykytilan selvitykseen
	// sekä vielä erikseen syötepuskurin tyhjennykseen.
	nappi odota_nappi();
	bool lue_nappi(nappi n);
	void tyhjenna_syote();
	
	// Funktio valikon piirtoon. Tämä voitaisiin toteuttaa aivan hyvin
	// valikon omassakin nimiavaruudessa, jolloin ohjelma-nimiavaruus
	// sisältäisi vain keskeiset ikkunan ja kuvien hallintaan tarvittavat
	// funktiot kuten lataa_kuva, piirra_kuva jne.
	void piirra_valikko(int pelin_tulos, valikko::valinta valittu);
}

#endif
