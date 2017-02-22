#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// 'constexpr' defines an immutable compile-time value
constexpr int windowWidth{800}, windowHeight{600};

int main() {

	// Creation of the game window
	RenderWindow window{{windowWidth, windowHeight}, "Arkanoid - 1"};
	window.setFramerateLimit(60);

	// Game loop
	while(true) {

		// "Clear" the window from prev drawn graphics
		window.clear(Color::Black);
		// in some OS:
		// Event event;
		// window.pollEvent(event);

		// If "escape" is pressed, break loop
		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// show the window contents
		window.display();
	}

	return 0;
}