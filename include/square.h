

class Square {	
    	private:
        	int s_value;
        	int s_state;
		
		Square createMatrix(int size);
       		void insertMine (Square s, int size, int mines);
    	
	public:
		Square ();

        	int getValue();
		int getState();	
		void setValue(int value);
		void setState(int state);
		void Matrix();
		
};

