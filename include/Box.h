
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#ifndef Square
#define Square
namespace mines{
class Box{
	
	int s_value;
   int s_state;
   bool bandera;
    	
public:
	Box();
	int getValue();
	int getState();	
	void setValue(int value);
	void setState(int state);
	void Matrix();	
	
	Box** createMatrix(int size);
	void insertMine (Box **s, int size, int mines);
	void insertNumber (Box **s, int size);
	void openEmptyBox (Box **s, int row, int col, int size);
	
	void printMatrix(Box** s, int size);
	void firstTouch (Box **s, int size ,int row, int col);
	
};
}

/*
class Square{
   int s_value;
   int s_state;
    	
public:
	Square();
	int getValue();
	int getState();	
	void setValue(int value);
	void setState(int state);
	void Matrix();
		
	void insertMine (Square* s, int size, int mines);
	Square** createMatrix(int size);
};*/

#endif

