
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#ifndef Square
#define Square
class Box{
	
int s_value;
   int s_state;
    	
public:
	Box();
	int getValue();
	int getState();	
	void setValue(int value);
	void setState(int state);
	void Matrix();
		
	void insertMine (Box** s, int size, int mines);
	Box** createMatrix(int size);
	
	
};

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

