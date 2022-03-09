#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#define KEY_SPACE ' ' // not defined in ncurses.h
#define KEY_Enter 10
#define START_ROW 5
#define START_COL 5

int HEIGHT, WIDTH;

void saveGame(WINDOW *win, int **board, int row, int col, int turn, int players)
{
	char saveName[1000];
	int i, j;
	mvwprintw(win, row, col, "Enter FILE NAME : ");
	echo();
	wgetstr(win, saveName);
	noecho();
	FILE* save = fopen(saveName, "w");
	fprintf(save, "%d %d %d %d %d %d ",row, col, WIDTH, HEIGHT, turn, players);
	for(i = 0; i < HEIGHT; i++)
	{
		for(j = 0; j < WIDTH; j++)
		{
			fprintf(save, "%d ", board[i][j]);
		}
	}
	fclose(save);

}

int** initBoard(int **board, int *row, int *col, int *turn, int load);
int** readSavedGame(WINDOW *win, int **board, int *row, int *col, int *turn, int *players)
{
	
	char saveName[1000];
	int i, j;
	wprintw(win, "Enter the file name : ");
	wgetstr(win, saveName);
	FILE* save = fopen(saveName, "r");
	fscanf(save, "%d %d %d %d %d %d", row, col, &WIDTH, &HEIGHT, turn, players);
	
	board = initBoard(board, row, col, turn, 1);
	for(i = 0; i < HEIGHT; i++)
	{
		for(j = 0; j < WIDTH; j++)
		{
			fscanf(save, "%d", &board[i][j]);
		}
	}
	return board;

}
int** initBoard(int **board, int *row, int *col, int *turn, int load){
	int i=0;
	board = (int**)malloc(sizeof(int*)*HEIGHT);

	for(i=0; i<HEIGHT; i++){
		board[i] = (int*)malloc(sizeof(int)*WIDTH);
	}

	printf("%d %d\n", HEIGHT, WIDTH);
	board[0][0] = ACS_ULCORNER;//'┌'
	for (i=1; i < WIDTH-1; i++)
		board[0][i] = ACS_TTEE;//'┬'
	board[0][WIDTH-1] = ACS_URCORNER; //'┐'

	for (i=1; i<HEIGHT-1; i++){
		int j=0;
		board[i][0] = ACS_LTEE; // '├'
		for (j=1; j < WIDTH-1; j++)
			board[i][j] = ACS_PLUS; //'┼'
		board[i][WIDTH-1] = ACS_RTEE; //'┤'
	}

	board[HEIGHT-1][0] = ACS_LLCORNER; //'└'
	for (i=1; i < WIDTH-1; i++)
		board[HEIGHT-1][i] = ACS_BTEE; //'┴'
	board[HEIGHT-1][WIDTH-1] = ACS_LRCORNER; // '┘'

	return board;
}

void paintBoard(int **board, WINDOW *win, int row, int col){
	
	int i,j;
	wmove(win, 0, 0);
	for(i=0; i<HEIGHT; i++)
	{
		for(j=0;j<WIDTH;j++)
		{
			waddch(win, board[i][j]);
		}
		wmove(win, i+1, 0);
	}
	wmove(win, row, col);
	refresh();
	wrefresh(win);

}

int checkWin(int **board, int *row, int *col, int *turn, int players)
{ // parameters and return type can be modified with any form. 
	int i, j, sum_vert, sum_hori, sum_cross1, sum_cross2;
	int revTurn;
	if(players == 2)
{
	if(*turn == 'O')
		revTurn = 'X';
	else if(*turn == 'X')
		revTurn = 'O';


	for (i = -4; i<1; i++)
	{
		sum_vert = 0;
		for (j = 0; j<5; j++)
		{
			if (*row + i + j<0 || *row + i + j >= HEIGHT)
				break;
			if (board[*row + i + j][*col] == revTurn)
				sum_vert += 1;
		}
		if (sum_vert == 5)
			break;
	}
	for (i = -4; i<1; i++)
	{
		sum_hori = 0;
		for (j = 0; j<5; j++)
		{
			if (*col + i + j<0 || *col + i + j >= WIDTH)
				break;
			if (board[*row][*col + i + j] == revTurn)
				sum_hori += 1;
		}
		if (sum_hori == 5)
			break;
	}
	for (i = -4; i<1; i++)
	{
		sum_cross1 = 0;
		for (j = 0; j<5; j++)
		{
			if (*col + i + j<0 || *col + i + j >= WIDTH || *row + i + j<0 || *row + i + j >= HEIGHT)
				break;
			if (board[*row + i + j][*col + i + j] == revTurn)
				sum_cross1 += 1;
		}
		if (sum_cross1 == 5)
			break;
	}
	for (i = -4; i<1; i++)
	{
		sum_cross2 = 0;
		for (j = 0; j<5; j++)
		{
			if (*col + i + j<0 || *col + i + j >= WIDTH || *row - i - j<0 || *row - i - j >= HEIGHT)
				break;
			if (board[*row - i - j][*col + i + j] == revTurn)
				sum_cross2 += 1;
		}
		if (sum_cross2 == 5)
			break;
	}
	if (sum_vert == 5 || sum_hori == 5 || sum_cross1 == 5 || sum_cross2 == 5)
		return 1;
	else
		return 0;
}
	else if(players == 3)
{
		if(*turn == 'O')
		revTurn = 'Y';
	else if(*turn == 'X')
		revTurn = 'O';
	else if(*turn == 'Y')
		revTurn = 'X';


	for (i = -3; i<1; i++)
	{
		sum_vert = 0;
		for (j = 0; j<4; j++)
		{
			if (*row + i + j<0 || *row + i + j >= HEIGHT)
				break;
			if (board[*row + i + j][*col] == revTurn)
				sum_vert += 1;
		}
		if (sum_vert == 4)
			break;
	}
	for (i = -3; i<1; i++)
	{
		sum_hori = 0;
		for (j = 0; j<4; j++)
		{
			if (*col + i + j<0 || *col + i + j >= WIDTH)
				break;
			if (board[*row][*col + i + j] == revTurn)
				sum_hori += 1;
		}
		if (sum_hori == 4)
			break;
	}
	for (i = -3; i<1; i++)
	{
		sum_cross1 = 0;
		for (j = 0; j<4; j++)
		{
			if (*col + i + j<0 || *col + i + j >= WIDTH || *row + i + j<0 || *row + i + j >= HEIGHT)
				break;
			if (board[*row + i + j][*col + i + j] == revTurn)
				sum_cross1 += 1;
		}
		if (sum_cross1 == 4)
			break;
	}
	for (i = -3; i<1; i++)
	{
		sum_cross2 = 0;
		for (j = 0; j<4; j++)
		{
			if (*col + i + j<0 || *col + i + j >= WIDTH || *row - i - j<0 || *row - i - j >= HEIGHT)
				break;
			if (board[*row - i - j][*col + i + j] == revTurn)
				sum_cross2 += 1;
		}
		if (sum_cross2 == 4)
			break;
	}
	if (sum_vert == 4 || sum_hori == 4 || sum_cross1 == 4 || sum_cross2 == 4)
		return 1;
	else
		return 0;
}

}

int Action(WINDOW *win, int **board, int keyin, int *row, int *col, int *turn, int players)
{
	keyin = getch();
	int playerName;
	int i;
	if(players == 2)
{
	if(*turn=='O')
		{
			playerName = 1;
		}
	else if(*turn == 'X')
		{
			playerName = 2;
		}
	switch(keyin)
	{
		case '1':
			saveGame(win, board, *row, *col, *turn, players);
			break;
		case '2':
			i = 2;
			break;
		case KEY_LEFT:
			if(*col<1)
				*col=0;
			else
				*col-=1;
			break;
		case KEY_RIGHT:
			if(*col+1==WIDTH)
				*col = WIDTH-1;
			else
				*col+=1;
			break;
		case KEY_DOWN:
			if(*row+1==HEIGHT)
				*row = HEIGHT-1;
			else
				*row+=1;
			break;
		case KEY_UP:
			if(*row<1)
				*row=0;
			else
				*row-=1;
			break;//Moving Cursor

		case KEY_Enter:
			if(board[*row][*col] == 'O' || board[*row][*col] == 'X')
				break;
			board[*row][*col] = *turn;
			if(*turn == 'O')
				*turn = 'X';
			else if(*turn =='X')
				*turn = 'O';	
			break;
		case KEY_SPACE:
			if(board[*row][*col] == 'O' || board[*row][*col] == 'X')
				break;
			board[*row][*col] = *turn;
			if(*turn == 'O')
				*turn = 'X';
			else if(*turn == 'X')
				*turn = 'O';	
			break;
	}
	
	int check = checkWin(board, row, col, turn, players);
	wmove(win, *row, *col);
	refresh();
	wrefresh(win);
	if(check == 1)
	{
		mvprintw(HEIGHT+9, WIDTH+9, "PLAYER%d WIN !! Press any button to terminate the program", playerName);
		paintBoard(board, win ,*row, *col);
		getch();
		return 1;
	}
	else if(i == 2)
	{
		return 1;
	}
	else
		return 0;
	refresh();
	wrefresh(win);	
}
else if(players == 3)
{
	if(*turn=='O')
		{
			playerName = 1;
		}
	else if(*turn == 'X')
		{
			playerName = 2;
		}
	else if(*turn == 'Y')
		{
			playerName = 3;
		}
	switch(keyin)
	{
		case '1':
			saveGame(win, board, *row, *col, *turn, players);
			break;
		case '2':
			i = 2;
			break;
		case KEY_LEFT:
			if(*col<1)
				*col=0;
			else
				*col-=1;
			break;
		case KEY_RIGHT:
			if(*col+1==WIDTH)
				*col = WIDTH-1;
			else
				*col+=1;
			break;
		case KEY_DOWN:
			if(*row+1==HEIGHT)
				*row = HEIGHT-1;
			else
				*row+=1;
			break;
		case KEY_UP:
			if(*row<1)
				*row=0;
			else
				*row-=1;
			break;//Moving Cursor

		case KEY_Enter:
			if(board[*row][*col] == 'O' || board[*row][*col] == 'X' || board[*row][*col] == 'Y')
				break;
			board[*row][*col] = *turn;
			if(*turn == 'O')
				*turn = 'X';
			else if(*turn =='X')
				*turn = 'Y';
			else if(*turn == 'Y')
				*turn = 'O';
			break;
		case KEY_SPACE:
			if(board[*row][*col] == 'O' || board[*row][*col] == 'X' || board[*row][*col] == 'Y')
				break;
			board[*row][*col] = *turn;
			if(*turn == 'O')
				*turn = 'X';
			else if(*turn == 'X')
				*turn = 'Y';
			else if(*turn == 'Y')
				*turn = 'O';
			break;
	}
	
	int check = checkWin(board, row, col, turn, players);
	wmove(win, *row, *col);
	refresh();
	wrefresh(win);
	if(check == 1)
	{
		mvprintw(HEIGHT+9, WIDTH+9, "PLAYER%d WIN !! Press any button to terminate the program", playerName);
		paintBoard(board, win ,*row, *col);
		getch();
		return 1;
	}
	else if(i == 2)
	{
		return 1;
	}
	else
		return 0;
	
	refresh();
	wrefresh(win);
}
}


void gameStart(WINDOW *win, int load, int players)
{
	int **board;
	int row = 0;
	int col = 0;
	int keyin;
	int turn='O';
	int act;
	int result;
	wmove(win, row, col); 
	board = initBoard(board, &row, &col, &turn, load); // Initiating the board
	
	if(load == 1)
	{
		echo();
		board = readSavedGame(win, board, &row, &col, &turn, &players);
		win = newwin(HEIGHT, WIDTH, 0, 0);
	
		noecho();
	}
	while(1)
	{
		mvprintw(HEIGHT+10,WIDTH+10,"Current Turn : %c", turn);
		mvprintw(HEIGHT+11,WIDTH+10,"1. Press 1 to save");
		mvprintw(HEIGHT+12,WIDTH+10,"2. Exit without save");
		paintBoard(board, win, row, col);
		wmove(win, row, col);
	
		act = Action(win, board, keyin, &row, &col, &turn, players);
		if(act==1) 
			break;
		
		else
			continue;    
	}

	return;
}

int main()
{
	char load2;
	int load;
	int players;
	int row, col;
	printf("Want to load the game?[y/n] : ");
	scanf("%c", &load2);
	if(load2 == 'y')
	{
		load = 1;
	}
	else if(load2 == 'n')
	{
		load = 2;
	}
	if(load == 2)
	{
	printf("Enter the HEIGHT of the board : ");
	scanf("%d", &HEIGHT);
	printf("Enter the WIDTH of the board : ");
	scanf("%d", &WIDTH);
	printf("Enter the number of players[2/3] : ");
	scanf("%d", &players);
	}
	else if(load == 1)
	{
		HEIGHT = 100;
		WIDTH = 100;
		players = 2;
	}
	else
	{
		printf("Wrong input!\n");
		return 0;
	}
	initscr();
	WINDOW *win = newwin(HEIGHT,WIDTH,0,0);
	keypad(stdscr, TRUE);
	noecho();
	gameStart(win, load, players);
	delwin(win);
	 
	endwin();
	return 0;
}
