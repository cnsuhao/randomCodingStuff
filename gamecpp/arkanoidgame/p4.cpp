#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// 'constexpr' defines an immutable compile-time value
constexpr int windowWidth{800}, windowHeight{600};

// constants for ball class
constexpr float ballRadius{10.f}, ballVelocity{8.f};

// class for our ball
// struct == class in c++
struct Ball {
	// CircleShape is a SFML class that defines a renderable circle
	CircleShape shape;

	// 2D vector that stores the ball's velocity
	Vector2f velocity{-ballVelocity, -ballVelocity};

	// constructor
	Ball(float mX, float mY) {
		// Apply pos, radius, color, origin
		shape.setPosition(mX, mY);
		shape.setRadius(ballRadius);
		shape.setFillColor(Color::Red);
		shape.setOrigin(ballRadius, ballRadius);
	}

	// updating the ball: move it's shape
	// by the current velocity
	void update() {
		shape.move(velocity);

		// keep the ball inside the window
		// change velocity to other direction if ball is get's to the edge of the window.
		
		if (left() < 0) velocity.x = ballVelocity;
		else if (right() > windowWidth) velocity.x = -ballVelocity;

		if (top() < 0) velocity.y = ballVelocity;
		else if (bottom() > windowHeight) velocity.y = -ballVelocity;
	}

	// create property methods to easily get commonly used values
	float x() 		{ return shape.getPosition().x; }
	float y() 		{ return shape.getPosition().y; }
	float left() 	{ return x() - shape.getRadius(); }
	float right() 	{ return x() + shape.getRadius(); }
	float top() 	{ return y() - shape.getRadius(); }
	float bottom() 	{ return y() + shape.getRadius(); }

};

int main() {

	// create instance of ball
	// positioned at the center of the window
	Ball ball(windowWidth / 2, windowHeight / 2);

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

		// every loop iteration, update the ball
		ball.update();

		// If "escape" is pressed, break loop
		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// show the window contents
		window.draw(ball.shape);
		window.display();
	}

	return 0;
}