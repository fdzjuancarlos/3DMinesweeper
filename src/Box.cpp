#include "Box.h"


Box::Box(){
	s_value = 0;
	s_state = 0;
}

int Box::getValue(){
	return s_value;
}

int Box::getState(){
	return s_state;
}

void Box::setValue(int value){
	s_value = value;
}

void Box::setState(int state){
	s_state = state;
}


//crear matriz
void Box::Matrix(){
	insertMine(createMatrix(10), 10, 5);
	
}

Box** Box::createMatrix(int size){
	Box **s = new Box*[size];
	for(int i=0; i<size; i++){
		s[i] = new Box [size];
	}

	for (int j=0; j<size; j++){
		for (int k=0; k<size; k++){
			s[j][k].setValue(0);
			s[j][k].setState(0);		
		}	
	}
	
	return s;
}

void Box::insertMine (Box **s, int size, int mines){

	for (int i=0; i<size; i++){
		for (int j=0; j<size; j++){
			if (s[i][j].getValue() == 0){
				srand(time(NULL));
				int var= (rand() %100);
				if (var>=30 || var<=60){
					s[i][j].setValue(-1);
					mines--;
				}
			}
		}
	}
	if(mines>0)
		insertMine(s, size, mines);
}


