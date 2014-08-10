#include "connect4.h"

void DrawBoardLayout() {
  clear();

  int c;
  int x, y, boardmaxx = 44, boardmaxy = 19;
  board = newwin(boardmaxy, boardmaxx, 4, 3);
  wattron(board, COLOR_PAIR(4));

  for(x = 0; x < boardmaxx; x++) {
    mvwaddch(board, 0, x, '*');
    mvwaddch(board, boardmaxy - 1, x, '*');
  }

  for(y = 0; y < boardmaxy; y++) {
    mvwaddstr(board, y, 0, "**");
    mvwaddstr(board, y, boardmaxx - 2, "**");
  }
  
  for(y = 1; y <= boardmaxy - 2; y++)
    for(x = 0; x < boardmaxx; x += 6)
      mvwaddstr(board, y, x, "**");

  for(x = 1; x <= boardmaxx - 2; x++)
    for(y = 0; y < boardmaxy; y += 3)
      mvwaddch(board, y, x, '*');

  refresh();
  wrefresh(board);
}

void DrawBoard() {
  int i, j, x, y;
  for(i = 1; i <= 6; i++) {
    y = 1 + 3 * (i - 1);
    for(j = 1; j <= 7; j++) {
      x = 2 + 6 * (j - 1);
      if(boardState[i][j] != 0) {
        switch(boardState[i][j]) {
	case 1:
	  wattrset(board, COLOR_PAIR(colorChoice[1]));
	  break;
	case 2:
	  wattrset(board, COLOR_PAIR(colorChoice[2]));
	  break;
	case 3:
	  wattrset(board, COLOR_PAIR(8));
	  break;
        }
	mvwaddstr(board, y, x, "****");
	mvwaddstr(board, y + 1, x, "****");
	wattrset(board, A_NORMAL);
      }
      else {
        wattrset(board, COLOR_PAIR(1));
        mvwaddstr(board, y, x, "    ");
        mvwaddstr(board, y + 1, x, "    ");
      }
    }
  }
  refresh();
  wrefresh(board);
}
