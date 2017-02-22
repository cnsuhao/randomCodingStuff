#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// 'constexpr' defines an immutable compile-time value
constexpr int windowWidth{800}, windowHeight{600};

// constants for ball class
constexpr float ballRadius{10.f}, ballVelocity{8.f};

// paddle for player
constexpr float paddleWidth{60.f}, paddleHeight{20.f}, paddleVelocity{6.f};

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

// paddle class
struct Paddle
{
	// rectangleshape is SMFL class
	RectangleShape shape;
	Vector2f velocity;

	// initialization
	Paddle (float mX, float mY) {
		shape.setPosition(mX, mY);
		shape.setSize(Vector2f(paddleWidth, paddleHeight));
		shape.setFillColor(Color::Blue);
		shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
	}

	void update() {
		shape.move(velocity);

		// get user input to move the paddle
		// check also we keep paddle inside window
		if(Keyboard::isKeyPressed(Keyboard::Key::Left) && left() > 0) {
			velocity.x = -paddleVelocity;
		} else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < windowWidth) {
			velocity.x = paddleVelocity;
		} else {
			// if user doesn't press anything, staph!
			velocity.x = 0;
		}
	}

	// create property methods to easily get commonly used values
	float x() 		{ return shape.getPosition().x; }
	float y() 		{ return shape.getPosition().y; }
	float left() 	{ return x() - shape.getSize().x / 2.f; }
	float right() 	{ return x() + shape.getSize().x / 2.f; }
	float top() 	{ return y() - shape.getSize().y / 2.f; }
	float bottom() 	{ return y() + shape.getSize().y / 2.f; }
};

// Dealing with collisions: generic function
// to check if two hsapes are intersecting (colliding)
template<class T1, class T2> bool isIntersecting(T1& mA, T2 mB) {
	return mA.right()
}


int main() {

	// create instance of ball
	// positioned at the center of the window
	Ball ball{windowWidth / 2, windowHeight / 2};

	Paddle paddle{windowWidth / 2, windowHeight -50};

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
		paddle.update();

		// If "escape" is pressed, break loop
		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// show the window contents
		window.draw(ball.shape);
		window.draw(paddle.shape);
		
		window.display();
	}

	return 0;
}