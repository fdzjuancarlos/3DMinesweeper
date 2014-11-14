#include "square.h"

Square::Square(){
	s_value = 0;
	s_state = 0;
}

int Square::getValue(){
	return s_value;
}

int Square::getState(){
	return s_state;
}

void Square::setValue(int value){
	s_value = value;
}

void Square::setState(int state){
	s_state = state;
}


