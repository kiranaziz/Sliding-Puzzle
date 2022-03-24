#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * ICSI333 System Fundamentals
 * Fall 2021
 * TA Kulkarni
 * Student ID: 001440162
 * 
 * Project 2: Sliding Puzzle - Part II
 * @author Kiran Aziz
 * @version 1.0
 */

int **gameBoard;		//Global game board, which is a 2D matrix.
int gameSize = 4;		//Global variable that indicates the board size.

//Declaring Indices structure which holds the indices of the row and column of a tile.
struct Indices{

	int rowIndex, columnIndex;  
};

typedef struct Indices Struct;

/*
 * Finds the row index and the column index of a tile.
 * @param **gameBoard: 2D matrix that holds the gameboard.
 * @param tile: Tile number whose indices need to be found.
 * @return indices of the tile.
 */
Struct findIndices(int **gameBoard, int tile){
	Struct s;

	//Iterates through the gameboard until the specified tile is found.
	//Once found, the row index and the column index are stored in the Struct s.	
	for(int i=0; i<gameSize; i++){
		for(int j=0; j<gameSize; j++){
			if(gameBoard[i][j] == tile){
				s.rowIndex=i;
				s.columnIndex=j;
			}
		}
	}	    

	return s;	    
}


/*
 * Randomizes the tiles on the current gameboard.
 * @param **gameBoard: 2D matrix that holds the gameboard.
 * @param gameSize: Size of the gameboard.
 */
void Randomize(int **gameBoard, int gameSize){
	srand(time(NULL));
	
	//Iterates through the created gameboard and randomly swaps tiles 
	//to create an entirely randomized board without repetition.
	for (int i=0; i<gameSize; i++){
		int r = rand() % (gameSize);
		for (int j = 0; j < gameSize; j++){
			int temp =  gameBoard[i][r];
			gameBoard[i][r] =  gameBoard[r][j];
			gameBoard[r][j] = temp;
		}
	}
}


/*
 * Sets up the gameboard, placing the tiles starting from top-left corner of the board, 
 * inversely sorted (from fifteen down to zero) and then proceeds to randomize the tiles.
 * @param gameSize: Size of the gameboard.
 * @return Returns gameboard.
 */
int ** Initialization(int gameSize){
	//Dynamically allocates the gameboard.
	int **gameBoard = (int **)malloc(gameSize * sizeof(int *));

	//Validates that the chosen game size is a number between 2 and 10, inclusive of both 2 and 10.
	if((gameSize >= 2) && (gameSize <=10 )){
		printf("Setting up the game.\n");
		int count = (gameSize*gameSize) - 1;	//Starting tile number, before randomization.

		//Builds the gameboard, storing the tile numbers from greatest to least.       	
		//In this gameboard, the number zero is equivalent to the empty tile in the gameboard.
		for(int i = 0; i < gameSize; i++){
			gameBoard[i] = (int *)malloc(gameSize * sizeof(int));
			for (int j = 0; j < gameSize; j++){
				gameBoard[i][j] = count;
				count--;
			}
		}

		Randomize(gameBoard, gameSize); //After all the tiles are created, the tiles are randomized over the board.
		return gameBoard; 		//Returns
	}else{
		//If the chosen game size is smaller than 2 or larger than 10, it cannot be made into a gameboard.
		printf("Invalid game size. Must choose a size between 2 and 10.");
	}	
	return gameBoard;
}     


/*    
 * Tears down the game and let's the user exit.  
 * @param **gameBoard: 2D matrix that holds the gameboard.
 * @param gameSize: Size of the gameboard.
 */                                                       
void Teardown(int **gameBoard, int gameSize){
	//Frees all allocated memory used by the gameboard.
	for (int i = 0; i < gameSize; i++)
		free(gameBoard[i]);
	free(gameBoard);

	printf("Ending the game.\n");
}


/*
 * Prints the current state of the gameboard as a 4x4 matrix.
 * @param **gameBoard: 2D matrix that holds the gameboard.
 * @param gameSize: Size of the gameboard.	     		
 */
void DisplayGameState(int **gameBoard, int gameSize){
	//Goes through the 2D matrix and prints each element.
	//Creates a blank/empty space for the zero tile.
	for(int i=0; i<gameSize; i++){   
		for(int j=0; j<gameSize; j++){
			if(gameBoard[i][j] != 0){
				printf("%3d", gameBoard[i][j]);
			}else{
				printf("   ");
			}
		}
		printf("\n");
	}
}	     									


/*
 * Determines whether moving the desired user tile is possible or not.	 
 * @param **gameBoard: 2D matrix that holds the gameboard.	  
 * @param currentTile: Tile number that is to be moved.
 * @param currentTileResult: Struct that holds the indices for the current tile to be moved.
 * @param emptyTileResult: Struct that holds the indices of the blank tile.
 * @return Returns one if the tile is movable, else, returns zero if the tile is not movable.  
 */
int IsMoveValid(int **gameBoard, int currentTile, Struct currentTileResult, Struct emptyTileResult){
	int flag = 0;	//Stores the truth value for a valid or invalid move. One means a valid move, zero means an invalid move.
	
	//Checks if the current tile is already the empty tile or not.	 
	if(gameBoard[currentTileResult.rowIndex][currentTileResult.columnIndex] != 0){ 
		//Checks if the given/current tile is in the same row as the empty 
		if(currentTileResult.rowIndex==emptyTileResult.rowIndex){
			if(abs(currentTileResult.columnIndex-emptyTileResult.columnIndex)==1){
				flag = 1;
			}
		}
		//Checks if the giver/current tile is in the same column as the empty tile.
		//If so, then the row indices of each are compared.  If the absolute difference of the
		//indices is one, then the givern/current tile can indeed move on the board.
		else if(currentTileResult.columnIndex==emptyTileResult.columnIndex){
			if(abs(currentTileResult.rowIndex-emptyTileResult.rowIndex)==1){
				flag = 1;
			}
		}

		//If the given/current tile is not on the same row or not on the same column
		//as the empty tile, then it certainly cannot be the case that the move is valid.
		else{
			flag = 0;
		}		            
	}
	return flag;
}	 


/*
 * If moving a given tile is valid (using the IsMoveValid() function to check validity), perform the move by
 * updating the game state. If invalid, print a message to the user.
 * @param **gameBoard: 2D matrix that holds the gameboard.
 * @param currentTile: Tile number to move.
 */
void MoveTile(int **gameBoard, int currentTile){  
	//Finds the tile number that the user wants to move, and then
	//finds the row index and column index of that tile number.
	Struct currentTileResult;
	currentTileResult = findIndices(gameBoard, currentTile);
	
	//Finds the empty tile number, which is stored as zero in the gameboard,
	//and then finds the row index and column index of the empty number.	
	Struct emptyTileResult;
	emptyTileResult = findIndices(gameBoard, 0);
	int validity = IsMoveValid(gameBoard, currentTile, currentTileResult, emptyTileResult);

	//Stores the truth value for a valid or invalid move. 
	//One means a valid move, zero means an invalid move.	     																
	//If the move is valid, then swap the empty tile with the given/current tile number.		
	//If the move is invalid, then print a message to the user about it.
	if(validity == 1){
		printf("Moving %d.\n", currentTile);
		int temp = gameBoard[currentTileResult.rowIndex][currentTileResult.columnIndex];
		gameBoard[currentTileResult.rowIndex][currentTileResult.columnIndex] = gameBoard[emptyTileResult.rowIndex][emptyTileResult.columnIndex];
		gameBoard[emptyTileResult.rowIndex][emptyTileResult.columnIndex] = temp;
	}else{
		printf("Invalid move.\n");
	}
}


/*
 * Tests if the user has won the game i.e. the tiles are sorted from 1 to the last tile.
 * The location of the blank tile does not matter.
 * @param **gameboard: 2D matrix that holds the gameboard.
 * @param gameSize: Size of gameboard.
 * @return Integer that determines the truth value of if the user has won the game. 
 */
int WonTheGame(int **gameboard, int gameSize){

	int flag = 1;
	int array[(gameSize*gameSize)];  	//Array to hold all tiles in the gameboard, 2D to 1D matrix.
	int array2[(gameSize*gameSize)-1];	//Array to hold all tiles except the empty tile.
	int k = 0;				//Holds array element index that coordinates with 2D tile number.
	int i;					//Holds array elment index when checking the sorting of the gameboard.
	
	//Converts 2D matrix into 1D matrix.
	for(int i = 0; i < gameSize; i++){
		for(int j = 0; j<gameSize; j++){
			array[k] = gameboard[i][j];
			k++;
		}
	}


	k=0;
	//Finds blank tile within gameboard and "removes" it while creating another array to hold all non-empty tile numbers.
	//Since the winning state does not rely on the position of the empty tile, it will not be considered
	//while checking the sort of the gameboard.
	for(int m = 0; m < (gameSize*gameSize); m++){ 
		if(array[m] != 0){
			array2[k] = array[m];
			k++;
		}
	}

	i = 0;
	//Checks the order of the tiles. If the tiles are in ascending order, then the gameboard is in a winning
	//state. Otherwise, the user can continue to play.
	while((flag == 1) && (i < (gameSize*gameSize) - 1)){
		if(array2[i] > array2[i+1]){
			flag = 0;
		}
		i++;
	}
	return flag;
}


/*
 * Saves the current state of the gameboard in a file so that the user can resume their game in a later session.
 * @param **gameBoard: 2D matrix that holds the gameboard.
 * @param gameSize: Size of the gameboard.
 * @param str[]: String to hold the filename desired by the user.
 * @return Integer value indicating the success/failure of the saving action. 
 */
int Save(int **gameBoard, int gameSize, char str[]){
	FILE *fp;		//Points to the file to be created.
	fp= fopen(str,"w");	//File can only be written to.
	int flag = 0;		//Return value for either success or failure.
	
	//If saving action is unsuccessful, returns -1.
	if(fp == NULL){
		flag = -1;
		return flag;
	}else{
		//Iterates through the gameboard and prints it to the file.
		for(int i = 0; i<gameSize; i++){
			for(int j = 0; j<gameSize; j++){
				if(j == gameSize - 1){
					fprintf(fp, "%3d\n", gameBoard[i][j]);
				}else{
					fprintf(fp, "%3d", gameBoard[i][j]);
				}
			}
		}
		flag = 1;  //Value for successful saving action.
	}

	fclose(fp);	//Closes the file.
	return flag;
}


/*
 * Loads a previously saved game. The inverse of the save function.
 * Gameboards of different sizes can be loaded. 
 * In the case of the loading action failing, the user can continue their current game. 
 * @param str[]; String to hold the filename that the user wants to load.
 * @return Integer value indicating the success/failure of the loading action.
 */
int Load(char str[]){
	FILE *fp;		//Points to the file to be read and loaded.
	fp= fopen(str,"r");	//File can only be read.
	int flag = 0;		//Return value for either success or failure.
	int count = 0;		//Determines number of lines in the file, which implies the size of the gameboard.
	char ch;		//Character read during the loading process.
			        

	if(fp == NULL){
		flag = -1;
		return flag;
	}else{
		//Traverses through file to help find the dimension/size of the gameboard in the file.
		for(ch = getc(fp); ch != EOF; ch = getc(fp)){
			if(ch == '\n'){
				count++;
			}
		}

		//Returns the file pointer to the beginning of the file.
		rewind(fp);
		//Temporary gameboard to transfer elements from saved file.
		int newBoard[count][count];
		for (int i = 0; i<count; i++) {
			for (int j = 0; j<count; j++) {
				fscanf(fp, "%d", &newBoard[i][j]);
			}
		
			char eol;                         // dummy variable
			fscanf(fp, "%c", &eol);     // read \n character
		}

		//Current gameboard is teared down.
		//New gameboard with same dimensions as saved file is created.
		Teardown(gameBoard, gameSize);
		gameBoard = Initialization(count);

		//Copies over the values saved in the temporary gameboard to the global 
		//gameboard.
		for (int i = 0; i < count; i++){
			for (int j = 0; j < count; j++){
				gameBoard[i][j] = newBoard[i][j];
			}
		}
		gameSize=count; //In the case that the saved gameboard's size is different than
		                //the current gameboard size, the global game size is changed.

		flag =1;
	}
	
	fclose(fp);	//Closes file.
	return flag;
}


/*	     											
 * First, calls the Initialization() function to make a default 4x4 game.	     		
 * Then, enters the game-loop that accepts a menu input, and performs corresponding operation or
 * display the state of the game world as needed.
 * Then, accepts one of the following three menu inputs and acts accordingly after prompting
 * 	Load: Prompt for a filename and load a saved gameboard.
 * 	Save: Prompt for a filename and save the current gameboard.
 * 	New: Prompt for the size of the new board and initialize it
 * 	Print: Display the game state, DisplayGameState() function.
 * 	Move: In this case, asks the user for a tile number to move, MoveTile() function. 
 * 	Quit: Quit the game.	     			
 * Finally, after the game-loop, tears down the game with the Teardown() function.
 */
int main(){
	char str;               	//String to hold the user's input.
	int tileNumber;			//Integer to hold the user's tile number.
	char loadFilename[100];		//String to hold filename for the loading action.
	char savedFilename[100];	//String to hold filename for the saving action.
	int uploadValue;
	int saveValue;

	//Sets the gameboard.
	gameBoard = Initialization(gameSize);					     

	//Starts the game-loop, promptin the user to select an option from the menu.
	do{
		printf("Menu: [l]oad, [m]ove, [n]ew, [p]rint, [q]uit, [s]ave?\t");
		scanf(" %c", &str);	//User inputs their option using a character.
		
		switch (str){
			//Loads a saved gameboard from a file.
			case 'l':
				printf("Which file would you like to load?\t");
				scanf("%s", loadFilename);
				uploadValue = Load(loadFilename);
				if(uploadValue == -1){
					printf("Upload failed.\n");
				}else if(uploadValue == 1){
					printf("Upload successful\n");
				}
				break;
			
			//Prompts user to enter a tile number, then if the move is valid,
			//moves the tile for the user.
			case 'm':
				printf("Which tile to move?\t");
				scanf("%d", &tileNumber);
				MoveTile(gameBoard, tileNumber);
				break;

			//Creates a new gameboard with a different size inputted from the user.
			case 'n':
				Teardown(gameBoard, gameSize);
				printf("Size of the board? (Only one integer)\t");
				scanf("%d", &gameSize);
				gameBoard = Initialization(gameSize);
				break;

			//Prints the game state to the user.
			case 'p':
				DisplayGameState(gameBoard, gameSize);		
				break;
			
			//Tears down the game-loop and exits the game.	
			case 'q':
				Teardown(gameBoard, gameSize);
				break;	     								
			
			//Saves the current gameboard into a file.	
			case 's':
				printf("Saving game, what filename would you like to save it as?\t");
				scanf("%s", savedFilename);
				saveValue = Save(gameBoard, gameSize, savedFilename);
				if(saveValue == -1){
					printf("Save failed.\n");
				}else if(saveValue == 1){
					printf("Save successful.\n");
				}
				break;
		}
		
		//Tests in each loop iteration for a winning state. 
		//If the user wins, print a congratulatory message and start a new game.
		if(WonTheGame(gameBoard, gameSize) == 1){
			printf("Congratulations! You won the game! Let's start a new game!\n");
			Teardown(gameBoard, gameSize);
			gameSize = 4;
			gameBoard = Initialization(gameSize);
		}
	}while(str != 'q');

	return 0;

}	     																												                                                                                    	    		    	    	    		    		    			    			    		    	    	    	    	                              						   				   		   	   	   	     			    																																	            																																			     					        												    											            										                                                                                                                                                                                                         																																				    							    			    																																	    			    																																												    							    			    								    		    		    		    						
