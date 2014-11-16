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
	Box **s = createMatrix(10);
	insertMine(s, 10, 5);
	insertNumber(s, 10);
	
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

void Box::insertNumber (Box **s, int size){

	for (int i=0; i<size; i++){
		for (int j=0; j<size; j++){
			if (s[i][j].getValue() == 0){
				int cont = 0;
				if (i-1 >=0){
					if (s[i-1][j].getValue() == -1)
						cont++;
					if (j-1 >= 0){
						if (s[i-1][j-1].getValue() == -1)
							cont++;	
					}
					if (j+1 < size){
						if (s[i-1][j+1].getValue() == -1)
							cont++;	
					}
				}
				if (i+1 < size){
					if (s[i+1][j].getValue() == -1)
						cont++;
					if (j-1 >= 0){
						if (s[i+1][j-1].getValue() == -1)
							cont++;	
					}
					if (j+1 < size){
						if (s[i+1][j+1].getValue() == -1)
							cont++;	
					}
				}
				if (j-1 >= 0){
					if (s[i][j-1].getValue() == -1)
						cont++;	
				}
				if (j+1 < size){
					if (s[i][j+1].getValue() == -1)
						cont++;	
				}
				s[i][j].setValue(cont);
				
			}
		}
	}
}

void Box::openEmptyBox (Box **s, int row, int col, int size){
	
	s[row][col].setState(-1);
	if (row-1 >=0){
		if (s[row-1][col].getValue() == 0)
			openEmptyBox(s, row-1, col, size);
		if (col-1 >= 0){
			if (s[row-1][col-1].getValue() == 0)
				openEmptyBox(s, row-1, col-1, size);	
		}
		if (col+1 < size){
			if (s[row-1][col+1].getValue() == 0)
				openEmptyBox(s, row-1, col+1, size);	
		}
	}
	if (row+1 < size){
		if (s[row+1][col].getValue() == 0)
			openEmptyBox(s, row+1, col, size);
		if (col-1 >= 0){
			if (s[row+1][col-1].getValue() == 0)
				openEmptyBox(s, row+1, col-1, size);	
		}
		if (col+1 < size){
			if (s[row+1][col+1].getValue() == 0)
				openEmptyBox(s, row+1, col+1, size);	
		}
	}
	if (col-1 >= 0){
		if (s[row][col-1].getValue() == 0)
			openEmptyBox(s, row, col-1, size);	
	}
	if (col+1 < size){
		if (s[row][col+1].getValue() == 0)
			openEmptyBox(s, row, col+1, size);	
	}
}
