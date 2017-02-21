#include <ncurses.h> // conio.h on MSDOS
#include <iostream>
//#include <random>
#include <unistd.h>
#include <string.h>
#include <vector>




// build:
// g++ snakegame.cpp -std=c++11 -L -lncurses
// g++ -o snakegame snakegame.cpp -std=c++11 -lncurses

// run:
// ./a.out

using namespace std;

const int width = 20;
const int height = 20;

int snakeX, snakeY, fruitX, fruitY, score;


enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

bool gameOver;

class Tail {
	public:
		int x;
		int y;
		Tail(int newX, int newY) {
			x = newX;
			y = newY;
		};
};

vector<Tail> snakeTail;

void placeFruitAtRandomPlace() {
	bool noOverlap = false;
	while(noOverlap == false) {
		noOverlap = true;
		fruitX = rand() % (width-1) + 1; // 1 to (width - 1)
		fruitY = rand() % (height-1) + 1;

		// check overlapping fruit and snake
		for(int i = 0; i < snakeTail.size(); i++) {
			if(snakeTail.at(i).x == fruitX && snakeTail.at(i).y == fruitY) {
				// if overlap skip back to whileloop
				noOverlap = false;
			}
		}
	}
}

void Setup() {
	gameOver = false;
	dir = RIGHT;

	snakeX = width / 2;
	snakeY = height / 2;

	placeFruitAtRandomPlace();

	initscr();
	//cbreak();
	noecho();
	//scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	//endwin();
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	Tail head(snakeX, snakeY);
	snakeTail.push_back(head);

}

void Draw() {
	//system("clear"); //system("cls")

	// draw grid, snake head and fruit
	for (int y = 0; y < height+1; y++) {
		for(int x = 0; x < width; x++) {
			if (x == 0 || x == width -1 || y == 0 || y == height) {
				//cout << "#";
				attron(COLOR_PAIR(3));
				mvprintw(y,x,"#");
				attroff(COLOR_PAIR(3));
			}
			else if (x == snakeX && y == snakeY) {
				//cout << "O";
				attron(COLOR_PAIR(2));
				mvprintw(y,x,"O");
				attroff(COLOR_PAIR(2));
			}
			else if(x == fruitX && y == fruitY) {
				//cout << "F";

				attron(COLOR_PAIR(1));
				mvprintw(y,x,"F");
				attroff(COLOR_PAIR(1));
			}
			else {
				//cout << " ";
				mvprintw(y,x," ");
			}
		}
		// cout << endl;
	}

	// draw tail
	if (snakeTail.size() > 1) {
		for(int i = 1; i < snakeTail.size(); i++) {
			attron(COLOR_PAIR(2));
			mvprintw(snakeTail.at(i).y, snakeTail.at(i).x, "o");
			attroff(COLOR_PAIR(2));
		}
	}

	mvprintw(0, width + 1,"Score: %d", score);
	mvprintw(1, width + 1,"Size %d", snakeTail.size());
	//cout << "score: " << score;

}

int kbhit(void) {
	int ch = getch();
	if (ch != ERR) {
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

void Input() {
	if(kbhit()) {
		switch(getch()) {
			case 'a':
				if (dir != RIGHT) {dir = LEFT;}
				break;
			case 'd':
				if (dir != LEFT) {dir = RIGHT;}
				break;
			case 'w':
				if (dir != DOWN) {dir = UP;}
				break;
			case 's':
				if (dir != UP) {dir = DOWN;}
				break;
			case 'x':
				gameOver = true;
				break;
			default:
				break;
		}
	}
}

void Logic() {

	//score++;
	
	// snake movement
	switch(dir) {
		case LEFT:
			snakeX--;
			break;
		case RIGHT:
			snakeX++;
			break;
		case UP:
			snakeY--;
			break;
		case DOWN:
			snakeY++;
			break;
		default:
			break;
	}

	//check snake doesn't hit itself
	if(snakeTail.size() > 1) {
		for(int i = 1; i < snakeTail.size(); i++) {
			if(snakeTail.at(i).x == snakeX && snakeTail.at(i).y == snakeY) {
				gameOver = true;
			}
		}
	}

	// keep inside grid
	if(snakeX > width - 2 || snakeX < 1 || snakeY > height - 1 || snakeY < 1) {
		gameOver = true;
	}

	// eat fruit
	if(snakeX == fruitX && snakeY == fruitY) {
		score += 50;
		placeFruitAtRandomPlace();
		Tail t(snakeX, snakeY);
		snakeTail.push_back(t);
	}

	// move tail
	Tail nextTail = Tail(snakeX, snakeY);
	if(snakeTail.size() > 1) {
		for(int i = 0; i < snakeTail.size(); i++) {
			Tail lastTail = snakeTail.at(i);
			snakeTail.at(i).x = nextTail.x;
			snakeTail.at(i).y = nextTail.y;
			nextTail = lastTail;
		}
	}
}

int main() {

	Setup();

	while(!gameOver) {
		Draw();
		Input();
		Logic();
		usleep(150000);
	}

	sleep(1);
	endwin();

	cout << endl << "score: " << score << endl << "size: " << snakeTail.size() << endl << endl;

	return 0;
}