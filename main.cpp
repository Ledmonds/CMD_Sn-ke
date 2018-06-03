#include <iostream>
#include <string>
#include <list>
#include <unordered_map> //Mapping Bearings
#include <chrono> //Sleeping Function
#include <thread> //Sleeping Function
#include <stdlib.h> //srand, rand
#include <time.h> //time
using namespace std;

/*ToDo:
	Implement movement controls, thread this too please.
	Implement board class to contain member fucntions for print board.
	Implement sleep function in seperate thread.
	Recode the isMember function, i don't like the way this looks so much. Maybe hash maps with directions and opposites stored.
	Maybe create a 2D vector class for storing coordinates, vector class seems a little too difficult / fancy for this game.
	Implement score system.
*/


int const  WINDOW_X = 80;
int const WINDOW_Y = 38;
int const UPDATE_TIME = 500; //ms

/*==========
FOOD BEGIN
==========*/
class Food {
	private:
		int x;
		int y;
		char food_character;

		void generateRandomFoodCoOrds() {
			x = rand() % WINDOW_X-1 + 1;
			y = rand() % WINDOW_Y-1 + 1;
		}

	public:
		//Constructors
		Food();
		Food(int,int);
		Food(int,int,char);

		//Gettors:
		int getFoodX() {return x;}
		int getFoodY() {return y;}
		char getFoodCharacter() {return food_character;}

		//Settors
		void setFoodX(int _x) {x = _x;}
		void setFoodY(int _y) {y = _y;}
		void setFoodCharacter(char _food_character) {food_character = _food_character;}

		//Checkers
		bool isFoodCoOds(int _potential_x, int _potential_y) {
			return (_potential_x == x && _potential_y == y) ? true : false;
		}
};

Food::Food() {
	generateRandomFoodCoOrds();
	food_character = '*';
}
Food::Food(int _x, int _y) {
	x = _x;
	y = _y;
	food_character = '*';
}
Food::Food(int _x, int _y, char _food_character) {
	x = _x;
	y = _y;
	food_character = _food_character;	
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
		char snakehead_character;
		char snakebody_character;

	public:
		//Constructors
		Snake();
		Snake(int,int);

		//Gettors
		int getSnakeheadX() {return snake_body.front().first;}
		int getSnakeheadY() {return snake_body.front().second;}
		int getSnakeLength() {return snake_body.size()-1;} //Head not included in score.
		int getSnakeBearing() {return bearing;}
		string getCardinalBearing(int _bearing) {
			if (_bearing == 0) return "North";
			else if (_bearing == 1) return "West";
			else if (_bearing == 2) return "South";
			else return "East";
		}		
		char getSnakeheadCharacter() {return snakehead_character;}
		char getSnakebodyCharacter() {return snakebody_character;}

		//Checkers
		bool isMember(int _potential_x, int _potential_y) {
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

		bool isWallCollision() {
			return (snake_body.front().first == 0 || snake_body.front().first == WINDOW_X-1 || snake_body.front().second == 0 || snake_body.front().second == WINDOW_Y-1) ? true : false;
		}

		bool isBodyCollision() {
			return isMember(snake_body.front().first,snake_body.front().second);
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

		void growSnake() { //Pushes new snake tail to end of previous tail, head not affected.
			snake_body.push_back(make_pair(snake_body.back().first,snake_body.back().second));
		}

		//Ranomiser
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
		void generateBearingMap() {
			bearing_map.insert(make_pair<int,int>(0,2));
			bearing_map.insert(make_pair<int,int>(1,3));
			bearing_map.insert(make_pair<int,int>(2,0));
			bearing_map.insert(make_pair<int,int>(3,1));	
		}

		//Debuggor
		void printSnakeheadCoOrds() {
			cout << "X:" << to_string(snake_body.front().first) << ", Y: " << to_string(snake_body.front().second) << endl;
		}

		void printSnakeCoOrds() {
			list <pair<int,int> >::iterator it;
			for (it = snake_body.begin(); it != snake_body.end(); ++it) cout << "X:" << to_string(it->first) << ", Y: " << to_string(it->second) << endl;
		}
};

Snake::Snake() {
	snake_body.push_back(make_pair(WINDOW_X/2,WINDOW_Y/2));
	generateRandomSnakeDelta(3);
	generateBearingMap();
	bearing = 0;
	snakehead_character = 'O';
	snakebody_character = 'o';
	
	
}

Snake::Snake(int _headX, int _headY) {
	snake_body.push_back(make_pair(_headY,_headX));
	generateBearingMap();
	bearing = 0;
	snakehead_character = 'O';
	snakebody_character = 'o';	
}


/*==========
SNAKE END
==========*/

void printBoard(Snake & _snake, Food & _food) {
	for(int y = 0; y < WINDOW_Y; ++y) {
		for(int x = 0; x < WINDOW_X; ++x) {
			if(x == 0 || y == 0 || x == WINDOW_X-1 || y == WINDOW_Y-1) cout << "*"; //isBorder
			else if (_snake.isSnakehead(x,y)) cout << _snake.getSnakeheadCharacter();
			else if (_snake.isMember(x,y)) cout << _snake.getSnakebodyCharacter();
			else if (_food.isFoodCoOds(x,y)) cout << _food.getFoodCharacter();
			else cout << " "; //isNotBorder
		}
		cout << endl;
	}
}


int main() {

	srand(time(NULL));

	Snake snake;
	Food food;

	while(!snake.isWallCollision() && !snake.isBodyCollision()) {
		system("clear");
		printBoard(snake, food);
		cout << "Bearing: " << snake.getCardinalBearing(snake.getSnakeBearing()) << endl;
		this_thread::sleep_for(std::chrono::milliseconds(UPDATE_TIME)); //BADDDDDD
		snake.generateRandomSnakeDirection();
		snake.iterateSnake();
	}

	cout << "Snakey Died :(" << endl;
	return 0;
}