#include "Box.h"
#include <iostream>

namespace mines{
Box::Box(){
	s_value = 0;
	s_state = 0;
	s_flag = false;
	n_flag = false;
}

int Box::getValue(){
	return s_value;
}

int Box::getState(){
	return s_state;
}

bool Box::getFlag(){
	return s_flag;
}

bool Box::getNoFlag(){
	return n_flag;
}

void Box::setValue(int value){
	s_value = value;
}

void Box::setState(int state){
	s_state = state;
}

void Box::setFlag(bool flag){
//	if(s_flag == false)
//		s_flag = true;
//	else
//		s_flag = false;
	s_flag = flag;		
}

void Box::setNoFlag(bool noFlag){
	n_flag = noFlag;
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
			s[j][k].setFlag(false);
			s[j][k].setNoFlag(false);
		}	
	}
	
	return s;
}

void Box::printMatrix(Box **s, int size){
	for (int j=0; j<size; j++){
		for (int k=0; k<size; k++){
			if(s[j][k].getValue() != -1){
				std::cout << " " << s[j][k].getValue();
			}else
				std::cout << s[j][k].getValue()	;	
		}	
		std::cout << std::endl;
	}
}

void Box::insertMine (Box **s, int size, int mines){
	int actualMines= mines;
	srand(time(NULL));
	int i, j;
	
	
	if(i<size){
		
	}

	while(actualMines>0){
		i=(rand()%size);
		j=(rand()%size);
			if (s[i][j].getValue() == 0 && s[i][j].getState() == 0){
				s[i][j].setValue(-1);
				actualMines--;
			}
	}

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
	
	if(row>=0 && row < size && col>=0 && col < size){
		s[row][col].setState(-1);
		if(s[row][col].getValue() == 0){
			if (row-1 >=0){
				if (s[row-1][col].getState() == 0 && s[row-1][col].getValue() != -1){
						if(s[row-1][col].getValue() == 0)
							openEmptyBox(s, row-1, col, size);
						else
							s[row-1][col].setState(-1);
						}
				if (col-1 >= 0){
					if (s[row-1][col-1].getState() == 0 && s[row-1][col-1].getValue() != -1){
						if(s[row-1][col-1].getValue() > 0)
							s[row-1][col-1].setState(-1);
					}
				}
				if (col+1 < size){
					if (s[row-1][col+1].getState() == 0 && s[row-1][col+1].getValue() != -1){
						if(s[row-1][col+1].getValue() > 0)
							s[row-1][col+1].setState(-1);
							}
				}
			}
			if (row+1 < size){
				if (s[row+1][col].getState() == 0 && s[row+1][col].getValue() != -1){
						if(s[row+1][col].getValue() == 0)
							openEmptyBox(s, row+1, col, size);
						else
							s[row+1][col].setState(-1);
							}
				if (col-1 >= 0){
					if (s[row+1][col-1].getState() == 0 && s[row+1][col-1].getValue() != -1){
						if(s[row+1][col-1].getValue() > 0)
							s[row+1][col-1].setState(-1);
							}
				}
				if (col+1 < size){
					if (s[row+1][col+1].getState() == 0 && s[row+1][col+1].getValue() != -1){
						if(s[row+1][col+1].getValue() > 0)
							s[row+1][col+1].setState(-1);
						}
					}
				}
			if (col-1 >= 0){
				if (s[row][col-1].getState() == 0 && s[row][col-1].getValue() == 0){
						if(s[row][col-1].getValue() == 0)
							openEmptyBox(s, row, col-1, size);
						else
							s[row][col-1].setState(-1);
						}
			}
			if (col+1 < size){
				if (s[row][col+1].getState() == 0 && s[row][col+1].getValue() == 0){
						if(s[row][col+1].getValue() == 0)
							openEmptyBox(s, row, col+1, size);
						else
							s[row][col+1].setState(-1);
						}
			}
		}
	}
}

void Box::firstTouch (Box **s, int size ,int row, int col){
	
	s[row][col].setState(-1);
	if (row-1 >=0){
		//row-1
		s[row-1][col].setState(-1);
		if (col-1 >= 0){
			//row-1 col-1
			s[row-1][col-1].setState(-1);
		}
		if (col+1 < size){
			//row-1 col+1
			s[row-1][col+1].setState(-1);
		}
	}
	if (row+1 < size){
		//row+1
		if (col-1 >= 0){
			//row+1 col-1
			s[row+1][col-1].setState(-1);
		}
		if (col+1 < size){
			//row+1 col+1
			s[row+1][col+1].setState(-1);
		}
	}
	if (col-1 >= 0){
		//row col-1
		s[row][col-1].setState(-1);
	}
	if (col+1 < size){
		//row col+1
		s[row][col+1].setState(-1);
	}
}

void Box::putFlag (Box **s, int row, int col){

	if(s[row][col].getFlag() == false){
		s[row][col].setFlag(true);
	}else
		s[row][col].setFlag(false);
}

void Box::gameOver (Box **s, int row, int col, int size){
	if(s[row][col].getValue() == -1 && s[row][col].getFlag() == false){
		for (int i=0; i<size; i++){
			for (int j=0; j<size; j++){
				if(s[i][j].getValue() != -1 && s[i][j].getFlag() == true){
					s[i][j].setNoFlag(true);
				}else if(s[i][j].getValue() == -1)
					s[i][j].setState(-1);
			}
		}
	}		
}
}
