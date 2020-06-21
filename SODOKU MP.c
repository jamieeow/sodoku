#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define MAX 32

//'Name' - datatype for strings
typedef char Name[MAX];

/////////////////////////
/////	STRUCTURES	/////
/////////////////////////

//Move structure
typedef struct Move {
	int row; //location of number
	int col; //location of number
	char number;	
} move;

//Board State structure	
typedef struct boardstate {
	char sodokuBoard[9][9]; 
	int movesMade;
	move listMoves[81]; //array - list of all moves made, including errors
} boardState;

//Game State structure
typedef struct gamestate {
	Name player;
	boardState currentBoardState;
	int totalErrors;
} gameState;



/////////////////////////
/////	FUNCTIONS	/////
/////////////////////////

//Initialize board to 0
void clearBoard (boardState *board, gameState *game){	
	
	int i,j;
	//clears board
	for (i=0; i<9; i++)
	{
		for (j = 0; j < 9; j++){
			board->sodokuBoard[i][j]='x';
		}
	}	
	//initialize listmoves to 0	
	for (i=0; i<81; i++)
	{		
		board->listMoves[i].col = 0;
		board->listMoves[i].row = 0;
		board->listMoves[i].number = 'x';
	}

	game->totalErrors = 0;	
	board->movesMade=0;
}

//prints board
void printBoard (char board[9][9]){
	
	int i,j;
	for (i=0; i<9; i++){
//		printf("\t");
		for (j = 0; j < 9; j++)
		{
			if (board[i][j] == 'x')	//NOTE: initialize empty spaces on the board to x
				printf ("   |");
			else
				printf(" %c |", board[i][j]);
		}
		printf("\n---+---+---+---+---+---+---+---+---\n");
	}
}

//gets input, converts into board[9][9]
void getInput (boardState *board){
	char input[9];
	int i = 0, j=0;
	for (i=0; i<9; i++) {
		printf("Line %d, input 9 characters: ", i+1);
		gets(input);
		
		for (j=0; j<9; j++) {
			board->sodokuBoard[i][j] = input[j];
		}
	}
	
}

//////////////////////////////////////////////////////////////	=> may mali pa sa opening ng file / filename
//converts file
void convertFile (gameState *game, boardState *board, int *loaded){
	
	clearBoard(board, game); 
	
//	char dotTxt[10] = {'.', 't', 'x', 't'};
	char tempName[32];
	strcpy(tempName, game->player);
	
	strcat(tempName, ".txt");
	
    FILE *fp;
    fp = fopen(tempName, "r");

    if (fp == NULL){
        printf("Error Reading File\n");
        *loaded = 0;
		system("pause");
    }
    
    else
	{
	    //scans player name
		fseek(fp, 13, SEEK_SET);
		fscanf(fp, "%s", game->player);
		
		///offset to line where sodoku starts
		fseek(fp, 2+2+12+2, SEEK_CUR); 
		
		//scans board
		char input[9];
		int i,j;
	    for (i=0; i<9; i++){
			//scans each line into input[]
	        for (j=0; j<9; j++)
				fscanf(fp, "%s", &input[i]);
			//sets from input[] to board	
			for (j=0; j<9; j++)
				board->sodokuBoard[i][j] = input[j];
	        fseek(fp, 2, SEEK_CUR);  //set cursor > next line
	    }
		
		
		//scans number of moves made
		fseek(fp, 2+2+6+2, SEEK_CUR);
		fscanf (fp, "%d", &board->movesMade);
		fseek(fp, 2, SEEK_CUR);
		
		//scans moves
		Name checkUndo;
		char undo[4] = {'u','n','d','o'};
		
		for (i=0; i<board->movesMade; i++)
		{	
			fscanf(fp,"%s",checkUndo);
			if (strcmp(checkUndo,undo)==0)
			{
				board->listMoves[i].number =	'x';
				game->totalErrors++;
			}
			else
			{
				fscanf(fp,"%d",&board->listMoves[i].row);
				fseek(fp, 1, SEEK_CUR);
				fscanf(fp,"%d",&board->listMoves[i].col);
				fseek(fp, 1, SEEK_CUR);
				fscanf(fp,"%c",&board->listMoves[i].number);
				fseek(fp, 2, SEEK_CUR);
			}	
		}
		
		*loaded = 1;
	}
    	
	
    fclose(fp);	
}

//Place Move Action
void placeMove(boardState board){

		int numMove, flag, r, c;
		
		printf ("Input number: ");
		do {
			flag = 0;
			if ( scanf ("%d",&numMove) != 1) {
				printf("Invalid move.\n");
				getchar();
				flag = 1;
			} else if (numMove<1||numMove>9)
				printf("Invalid move.\n");
			
		} while ( flag==1 || (numMove<1||numMove>9) );
		
		
		printf ("Input position row: ");
		do {
			flag = 0;
			if ( scanf ("%d",&r) != 1) {
				printf("Invalid move.\n");
				getchar();
				flag = 1;
			} else if (r<0||r>8)
				printf("Invalid move.\n");
			
		} while ( flag==1 || (r<0||r>8) );
		
		printf ("Input position column: ");
		do {
			flag = 0;
			if ( scanf ("%d",&c) != 1) {
				printf("Invalid move.\n");
				getchar();
				flag = 1;
			} else if (c<0||c>0)
				printf("Invalid move.\n");
			
		} while ( flag==1 || (c<0||c>8) );
				
		
		//checks if illegal--if position is occupied, if invalid input number, if within row and column
		if (board.sodokuBoard[r][c] == 'x' &&  numMove>0&&numMove<10 && r>0&&r<10 && c>0&&c<10)
		{
			move m;
			//initializes move
			m.number = numMove;
			m.row = r;
			m.col = c;
			
			//changes to sodoku board
			board.sodokuBoard[r][c] = numMove;
					
			//adds to list of moves, using total num of moves as index
			board.listMoves[board.movesMade] = m;
		}
		
		else
		{
			printf ("Input invalid!");
			//adds as illegal move
			move illegal;
			illegal.number = 'x';	//still x, since illegal move is ignored
			board.listMoves[board.movesMade] = illegal;
		}
		
		board.movesMade++;
 }



//Checks if there is a row/column/box with complete values 1-9
int isGameOver (boardState board)
{
	/*
	RETURN CASES:
		-board incomplete == 0
		-board complete, sodoku == 1
		-board complete, no sodoku == -1
	*/
	
	int i,j;
	
	//checks if complete
	for (i=0; i<9; i++)
	{
		for (j = 0; j < 9; j++)
		{	
			if (board.sodokuBoard[i][j] == 'x')	{	//if is still empty space
				return 0;
			}
		}
	}
	
	

	//checks row
	for (i=0; i<9; i++)
		for (j=i+1; j < 9; j++)
			if (board.sodokuBoard[i] == board.sodokuBoard[j])
				return -1;
		
	
	//checks col
	int k;
	for (k = 0; k<9; k++)
		for (i=0; i<9; i++)
			for (j=i+1; j < 9; j++)
				if (board.sodokuBoard[i][k] == board.sodokuBoard[j][k])
					return -1;

	//checks box
	int nRow=1, nCol=1;
	int l;
	for (i=nRow; i<nRow+3; i++) 
		for (j=nCol; j<nCol+3; j++) 
			for (k=nRow; k<nRow+3; k++) 
				for (l=nCol; l<nCol+3; l++)	
					if (board.sodokuBoard[i][j] == board.sodokuBoard[k][l] && 
						board.sodokuBoard[i][j] != board.sodokuBoard[k][l] ) 
						return -1;

	
		
}

//gets player name as input and sets to parameter gameState game
void getName(gameState *game){
	printf ("Input name:   ");
	gets(game->player);
}



///// LOAD BOARD FUNCTIONS /////
void loadBoardInput(boardState *board, gameState *game, int *loaded){
	
//a
	getName(game);
	clearBoard(board, game);

//b
	game->totalErrors = 0;
	board->movesMade = 0;
		
	getInput(board);
	printf ("Board loaded!\n");
	*loaded = 1;
	system("pause");
		
}


void loadBoardFile(boardState *board, gameState *game, int *loaded){

//a
	getName(game);
	clearBoard(board, game);
	
//b
	convertFile(game,board, loaded);
	
}

void saveBoardFile(boardState board, gameState game){	
	/*saves current sodoku game to text file named <playername>.txt*/

	char dotTxt[10] = {'.', 't', 'x', 't'};
	char tempName[32];
	strcpy(tempName, game.player);
	strcat(tempName, dotTxt);
	
	FILE *fp;
	fp = fopen (tempName,"w");
	
	//prints playername
	fprintf (fp,"Player name: ");
	fprintf (fp, game.player); ///NOT SURE ABOUT THIS ONE???
	fprintf (fp,"\n\n");
	
	//prints board
	fprintf (fp,"Board State:\n");
	int i,j;
	for (i=0; i<9; i++){
		for (j = 0; j < 9; j++)
		{
			fprintf(fp,"%c", board.sodokuBoard[i][j]);
		}
		fprintf (fp,"\n");
	}
	fprintf (fp,"\n");
	
	//prints moves	
	fprintf (fp,"Moves: \n");
	fprintf (fp,"%d\n",board.movesMade); //number of moves made so far
	
	for (i=0; i<board.movesMade; i++)
	{
		if (board.listMoves[i].number=='x')		//if invalid move
			fprintf(fp, "undo\n");
		else
			fprintf (fp, "%d %d %d\n",board.listMoves[i].row,board.listMoves[i].col,board.listMoves[i].number);	
	}
		
	fclose (fp);
}


//displays game screen, + scans and executes actions
void gameScreen(gameState game, boardState board, move m, int numErrors){
	
//details
	
	
	//OPTIONS
	int opt;
	do {
		system("cls");
		//board
		printf ("Your current board:\n\n");
		printBoard (board.sodokuBoard);
		printf ("\n");
		
		//last move made
		if (m.number != 'x') 	//if not illegal
			printf ("Last move: '%d' on (%d,%d)\n\n", m.number, m.row, m.col);
		else
			printf ("Last move: ILLEGAL\n\n");
		
		//num of errors
		printf ("Number of errors: %d\n",numErrors);
	
		//actions
		printf ("ACTIONS: (1) Place move   (2) Undo   (3) Save Board   (4) Reset\n");
		opt = getch();
		
		//place move
		if (opt == 49) {
			placeMove(board);
			break;
		}
			
			
		//undo last move
		else if (opt == 50) {
			printf ("ILLEGAL MOVE!");
			break;
		}
		
		//save board
		else if (opt == 51){
			saveBoardFile(board, game);
			break;
		}
		//reset
		else if (opt == 52) {
			clearBoard(&board, &game);
			
			printf ("Reset finished!");
			break;
		}
		else {
			printf("Invalid move.");
			system("pause");
		}
	} while(opt != 49 || opt != 50 || opt != 51 ||opt != 52);
	
}

///// ACTUAL GAME /////
/////////////////////////////////////////////////////////
void playLoadedBoard(boardState *board, gameState *game, int *loaded){

//a
//	int loaded;
	/*check if no loaded sodoku board*/
	
	if (*loaded==0)
	{
		int opt;
		do {
			system("cls");
			printf ("This option is invalid!\n");
			printf ("(1) Load board from input   (2) Load board from file\n");
			
			opt = getch();
			
				if (opt == 49){
					loadBoardInput(board,game,loaded);
					break;
				}
				else if (opt == 50) {
					loadBoardFile(board,game,loaded);
					break;
				}
				else {
					printf("Invalid input. \n");
					system("pause");
				}
			
		} while(opt != 49 || opt != 50);
	}

//b - loop game until there is winner
	else
		while (isGameOver(*board) == 0){
			gameScreen(*game, *board, board->listMoves[board->movesMade], game->totalErrors);
		}
	
//c
	//game over
		if (isGameOver(*board) == 1) 		//game over, there is winner
		{			
			printf ("SODOKU! Congratulations, %s!\n",game->player);
			printBoard (board->sodokuBoard);
			system("pause");
//			mainScreen();
		}
		else if (isGameOver(*board) == -1)	//game over, no winner
		{		
			printf ("GAME OVER! Board is complete, you do not win.\n");
			printBoard (board->sodokuBoard);
			system("pause");
//			mainScreen();
		}

	
}


//displays main screen + main options
void mainScreen(){
	
	int loaded = 0;
	int opt = NULL;
	
	boardState board;
	gameState game;
	
	while (opt != 49 || opt != 50 || opt != 51) {
		system("cls");
		
		printf ("Welcome to sodoku!\n\n");
		
		printf ("Options:\n");
		printf ("\t1 - Load board from input\n");
		printf ("\t2 - Load board from file\n");
		printf ("\t3 - Play loaded sodoku board\n");
		
		opt = getch();		
		
		
			
		switch(opt){
			
			//load board from input
			case 49:
				loadBoardInput(&board, &game, &loaded);
				break;
				
			//load board from file
			case 50:
				loadBoardFile(&board, &game, &loaded);
				break;
				
			//play loaded sodoku board
			case 51:
				playLoadedBoard(&board, &game, &loaded);
				break;
			
			default:
				printf("Invalid input! Try again. \n");
				system("pause");
		}
			
	}
	
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
int main (){
	
	mainScreen();
	
	return 0;
		
}

/*

>>>ISSUES

etc:
-how to check if there is loaded sodoku board (sa gamescreen)???


>>>CHECK:

-game over -- ok na ba board checker?????
-reading files (convertBoard) -- tama ba offset?

-overall game maayos na ba

***If any input misformatting is encountered at any point through 
reading the files through a load action, or if the given sudoku board 
is not a legal configuration of sudoku, the load should fail and the 
player should be asked to load another file.

*/

