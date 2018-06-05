#include <iostream>
#include <string>
#include <list>
#include <unordered_map> //Mapping Bearings
#include <chrono> //Sleeping Function
#include <thread> //Sleeping Function
#include <stdlib.h> //srand, rand
#include <time.h> //time
using namespace std;


int const  WINDOW_X = 120;
int const WINDOW_Y = 40;
int const UPDATE_TIME = 50; //ms


/*==========
HELPER FUNCTIONS
==========*/
string outputBlueText(char _char) { //Code color 34
	string _string = "\033[34m";
	_string += _char;
	_string += "\033[0m";
	return _string;
}
string outputGreenText(char _char) { //Code color 32
	string _string = "\033[32m";
	_string += _char;
	_string += "\033[0m";
	return _string;
}
string outputRedText(char _char) { //Code color 31
	string _string = "\033[31m";
	_string += _char;
	_string += "\033[0m";
	return _string;
}


/*==========
FOOD BEGIN
==========*/
class Food {
	private:
		int food_x;
		int food_y;
		static const char food_character = 'o';

	public:
		//Constructors
		Food();
		Food(int,int);

		//Gettors:
		int getFoodX() {return food_x;}
		int getFoodY() {return food_y;}
		char getFoodCharacter() {return food_character;}

		//Checkers
		bool isFoodCoOds(int _potential_x, int _potential_y) {return (_potential_x == food_x && _potential_y == food_y) ? true : false;}

		//Assistors
		void generateRandomFoodCoOrds() {
			food_x = (rand() % WINDOW_X-1) + 1;
			food_y = (rand() % WINDOW_Y-1) + 1;
		}
};
Food::Food() {
	generateRandomFoodCoOrds();
}
Food::Food(int _food_x, int _food_y) {
	food_x = _food_x;
	food_y = _food_y;
}
/*==========
FOOD END
==========*/


/*==========
SNAKE BEGIN
==========*/
class Snake {
	private:
		int bearing;
		list<pair<int,int> >  snake_body;
		unordered_map<int,int> bearing_map; //Holds reveresed bearings for snake directions.
		static const char snakehead_character = 'O';
		static const char snakebody_character = '+';

	public:
		//Constructors
		Snake();

		//Gettors
		int getSnakeheadX() {return snake_body.front().first;}
		int getSnakeheadY() {return snake_body.front().second;}
		int getSnakeLength() {return snake_body.size()-1;} //Head not included in score.
		int getSnakeBearing() {return bearing;}
		string getSnakeCardinalBearing() {
			if (bearing == 0) return "North";
			else if (bearing == 1) return "West";
			else if (bearing == 2) return "South";
			else return "East";
		}		
		char getSnakeheadCharacter() {return snakehead_character;}
		char getSnakebodyCharacter() {return snakebody_character;}

		//Checkers
		bool isSnakebodyMember(int _potential_x, int _potential_y) {
			if (snake_body.size() <= 1) return false;
			else {
				list <pair<int,int> >::iterator it = snake_body.begin();
				++it;
				for (it = it; it != snake_body.end(); ++it) if (it->first == _potential_x && it->second == _potential_y) return true;
			}
			return false;
		}

		bool isSnakehead(int _potential_x, int _potential_y) {
			return (snake_body.front().first == _potential_x && snake_body.front().second == _potential_y) ? true : false;
		}

		bool isBoarderCollision() {
			return (snake_body.front().first == 0 || snake_body.front().first == WINDOW_X-1 || snake_body.front().second == 0 || snake_body.front().second == WINDOW_Y-1) ? true : false;
		}

		bool isBodyCollision() {
			return isSnakebodyMember(snake_body.front().first,snake_body.front().second);
		}

		bool isValidBearingChange(int _potential_bearing) {
			return (bearing != _potential_bearing && bearing != bearing_map.at(_potential_bearing)) ? true : false;
		}

		//Assistors
		void iterateSnake() {
			if (bearing == 0) snake_body.push_front(make_pair(snake_body.front().first,snake_body.front().second-1));
			else if (bearing == 1) snake_body.push_front(make_pair(snake_body.front().first-1,snake_body.front().second));
			else if (bearing == 2) snake_body.push_front(make_pair(snake_body.front().first,snake_body.front().second+1));
			else snake_body.push_front(make_pair(snake_body.front().first+1,snake_body.front().second));

			snake_body.pop_back();
		}

		void growSnake() {
			snake_body.push_back(make_pair(snake_body.back().first,snake_body.back().second));
		}

		//Randomisers
		void generateRandomSnakeDelta(int _it) {
			for (int i = 0; i < _it; ++i) {
				if (rand()%2 == 0) snake_body.push_back(make_pair(snake_body.back().first+1,snake_body.back().second));
				else snake_body.push_back(make_pair(snake_body.back().first,snake_body.back().second+1));
			}
		}
		void generateRandomSnakeDirection() {
			int _potential_bearing = rand() % 4;
			if (rand()%2 == 0 && isValidBearingChange(_potential_bearing)) bearing = _potential_bearing;
		}

		//Assistors
		void generateBearingMap() {
			bearing_map.insert(make_pair<int,int>(0,2));
			bearing_map.insert(make_pair<int,int>(1,3));
			bearing_map.insert(make_pair<int,int>(2,0));
			bearing_map.insert(make_pair<int,int>(3,1));	
		}

		//Debuggors
		void printSnakeheadCoOrds() {
			cout << "X:" << to_string(snake_body.front().first) << ", Y: " << to_string(snake_body.front().second) << endl;
		}

		void printSnakeCoOrds() {
			list <pair<int,int> >::iterator it;
			for (it = snake_body.begin(); it != snake_body.end(); ++it) cout << "X:" << to_string(it->first) << ", Y: " << to_string(it->second) << endl;
		}

		//Magical Unicorn Stuff
		void startLearingHowToTeleport() {
			snake_body.front().first = (rand()%WINDOW_X-1)+1;
			snake_body.front().second = (rand()%WINDOW_Y-1)+1;
		}
};
Snake::Snake() {
	snake_body.push_back(make_pair(WINDOW_X/2,WINDOW_Y/2));
	generateRandomSnakeDelta(4);
	generateBearingMap();
	bearing = 0;
}
/*==========
SNAKE END
==========*/


/*==========
BOARD BEGIN
==========*/
class Board {
	private:
		int board_x;
		int board_y;
		int score;
		static const char boarder_character = '*';
		Snake snake;
		Food food;

	public:
		//Constructors
		Board();

		//Gettors
		int getBoardX() {return board_x;}
		int getBoardY() {return board_y;}
		int getScore() {return score;}
		char getBoarderCharacter() {return boarder_character;}

		//Checkers
		bool isBoarder(int _potential_x, int _potential_y) {
			return (_potential_x == 0 || _potential_x == board_x-1 || _potential_y == 0 || _potential_y == board_y-1) ? true : false;
		}
		bool boardCollisionCheck() {
			// if (snake.isBoarderCollision() || snake.isBodyCollision()) {
			// 	snake.startLearingHowToTeleport();
			// 	return false;
			// }
			// return false;
			return (snake.isBoarderCollision() || snake.isBodyCollision());
		}

		//Assistors
		void printBoard() {
			system("clear");
			for(int y = 0; y < board_y; ++y) {
				for(int x = 0; x < board_x; ++x) {
					if(isBoarder(x,y)) cout << outputRedText(boarder_character); //isBorder
					else if (snake.isSnakehead(x,y)) cout << outputGreenText(snake.getSnakeheadCharacter());
					else if (snake.isSnakebodyMember(x,y)) cout << outputGreenText(snake.getSnakebodyCharacter());
					else if (food.isFoodCoOds(x,y)) cout << outputBlueText(food.getFoodCharacter());
					else cout << " "; //isNotBorder
				}
				cout << endl;
			}
			cout << "Score: " << score << ", Bearing: " << snake.getSnakeCardinalBearing() << endl;
		}

		void iterateBoard() {
			this_thread::sleep_for(std::chrono::milliseconds(UPDATE_TIME)); //BADDDDDD

			snake.generateRandomSnakeDirection();

			snake.iterateSnake();
			if (snake.getSnakeheadX() == food.getFoodX() && snake.getSnakeheadY() == food.getFoodY()) {
				snake.growSnake();
				++score;
				food.generateRandomFoodCoOrds();
			}
		}


};
Board::Board() {
	board_x = WINDOW_X;
	board_y = WINDOW_Y;
	score = 0;
	//food = Food(WINDOW_X/2,WINDOW_Y/4);
}

/*==========
BOARD END
==========*/



int main() {

	srand(time(NULL));

	Board board;

	while(!board.boardCollisionCheck()) {
		board.printBoard();
		board.iterateBoard();
	}

	return 0;
}