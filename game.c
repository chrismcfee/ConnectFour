#include "connect4.h"

void DrawBoardLayout() {
  clear();

  int c;
  int x, y, boardmaxx = 44, boardmaxy = 19;
  board = newwin(boardmaxy, boardmaxx, 4, 3);
  wattron(board, COLOR_PAIR(3));

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

void Play() {
  int c, availableRow, colChosen = 0, color = colorChoice[1];
  turn = 1;
  nodelay(stdscr, TRUE);
  while(1) {
    c = getch();
    PrintTime();
    PrintScore();
    if(c == 'q') {
      int ch;
      DrawPrompt("REALLY QUIT?\n YES(y)/NO(n)");
      do {
	ch = getch();
      }while(ch != 'y' && ch != 'n');
      if(ch == 'y') {
	UpdatePlayer(p[0]);
	UpdatePlayer(p[1]);
	Quit();
	break;
      }
      if(ch == 'n') {
	DrawBoardLayout();
	DrawBoard();
      }
    }
    if(c == 'o' && popOutActive == 1 && boardState[6][colChosen + 1] == turn) {
      if(GetAvailableRow(colChosen + 1) == 0) {
	colsFull--;
      }
      PopOut(colChosen);
      DrawBoard();
      turn = 3 - turn;
      color = colorChoice[turn];
    }
    if(c == 'p') {
      int diff = Pause();
      start_time += diff;
    }
    if(c == 's') {
      time_t t = time(NULL);
      difTime = t - start_time;
      SaveGame();
    }
    if(c == ' ' || c == 10) {
      availableRow = GetAvailableRow(colChosen + 1);
      if(availableRow > 0) {
	AnimatePiece(turn, colChosen);
	boardState[availableRow][colChosen + 1] = turn;
	DrawBoard(boardState);
	if(CheckEndOfGameFromPosition(availableRow, colChosen + 1)) {
	  GameOver();
	}
	turn = 3 - turn;
	color = colorChoice[turn];
	if(availableRow == 1) {
	  colsFull++;
	  if(colsFull == 7) {
	    colsFull = 0;
	    GameIsDraw();
	  }
	}
      }
    }
    PreviewPiece(2, colChosen, color);
    if(c == KEY_LEFT || c == 'a') {
      colChosen = (colChosen + 6) % 7;
      PreviewPiece(2, colChosen, color);
    }
    if(c == KEY_RIGHT || c == 'd') {
      colChosen = (colChosen + 1) % 7;
      PreviewPiece(2, colChosen, color);
    }
  }
}
int CheckEndOfGameFromPosition(int row, int col) {
  int ok = 0, count = 0, i = row, j = col;
  InitializeWinningPositions();
  /* check vertical */
  while(boardState[i][j] == boardState[row][col] && i <= 6) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    i++;
  }
  if(count >= 4) {
    return 1;
  }
  /* check horizontal */
  count = 0; i = row; j = col;
  InitializeWinningPositions();
  while(boardState[i][j] == boardState[row][col] && j >= 1) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    j--;
  }
  j = col + 1;
  while(boardState[i][j] == boardState[row][col] && j <= 7) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    j++;
  }
  if(count >= 4) {
    return 1;
  }
  /* check first diagonal */
  count = 0; i = row; j = col;
  InitializeWinningPositions();
  while(boardState[i][j] == boardState[row][col] && j <=7 && i >= 1) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    j++;
    i--;
  }
  i = row + 1;
  j = col - 1;
  while(boardState[i][j] == boardState[row][col] && j >=1 && i <= 6) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    j--;
    i++;
  }
  if(count >= 4) {
    return 1;
  }
  /* check second diagonal */
  count = 0; i = row; j = col;
  InitializeWinningPositions();
  while(boardState[i][j] == boardState[row][col] && j >=1 && i >= 1) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    j--;
    i--;
  }
  i = row + 1;
  j = col + 1;
  while(boardState[i][j] == boardState[row][col] && j <= 7 && i <= 6) {
    count++;
    winningPositions[0][count - 1] = i;
    winningPositions[1][count - 1] = j;
    j++;
    i++;
  }
  if(count >= 4) {
    return 1;
  }
  return 0;
}
void InitializeWinningPositions() {
  int i, j;
  for(i = 0; i < 2; i++)
    for(j = 0; j < 7; j++)
      winningPositions[i][j] = 0;
}
void BlinkWinningPositions() {
  int i, blinked = 0, prevValue;
  while(blinked < 5) {
    i = 0;
    while(i < 7 && winningPositions[0][i] != 0) {
      prevValue = boardState[winningPositions[0][i]][winningPositions[1][i]];
      boardState[winningPositions[0][i]][winningPositions[1][i]] = 3;
      i++;
    }
    DrawBoard(boardState);
    napms(150);
    i = 0;
    while(i < 7 && winningPositions[0][i] != 0) {
      boardState[winningPositions[0][i]][winningPositions[1][i]] = prevValue;
      i++;
    }
    DrawBoard(boardState);
    napms(120);
    blinked++;
  }
}
void AnimatePiece(int turn, int colChosen) {
  int i = 1, availableRow = GetAvailableRow(colChosen + 1);
  while(i < availableRow) {
    boardState[i][colChosen + 1] = turn;
    DrawBoard(boardState);
    refresh();
    wrefresh(board);
    napms(120);
    boardState[i][colChosen + 1] = 0;
    DrawBoard(boardState);
    refresh();
    i++;
  }
}

void PreviewPiece(int row, int colChosen, int color) {
  int i;
  for(i = 0; i < 7; i++) {
    if(i == colChosen) {
      attron(COLOR_PAIR(color));
      mvprintw(row, 5 + 6 * i, "****");
      mvprintw(row + 1, 5 + 6 * i, "****");
      attroff(COLOR_PAIR(color));
    }
    else {
      mvprintw(row, 5 + 6 * i, " ");
      mvprintw(row + 1, 5 + 6 * i, " ");
    }
    refresh();
  }
}

int GetAvailableRow(int col) {
  int i = 0;
  while(boardState[i + 1][col] == 0 && i <= 5)
    i++;
  return i;
}
/* Prints current time and time spent since the beginning of the game */
void PrintTime() {
  struct tm *cur_time;
  time_t t, dif;
  t = time(NULL);
  int hours, minutes, seconds;
  cur_time = localtime(&t);
  mvprintw(2, 50, "Local Time:");
  mvprintw(3, 50, "%02d:%02d:%02d", cur_time -> tm_hour,
	   cur_time -> tm_min, cur_time -> tm_sec);
  dif = t - start_time;
  seconds = dif % 60;
  dif= dif / 60;
  minutes = dif% 60;
  hours = dif / 60;
  mvprintw(15, 50, "In-game time:");
  mvprintw(16, 50, "%02d:%02d:%02d", hours, minutes, seconds);
}
void PrintScore() {
  switch(turn) {
  case 1:
    mvprintw(5, 51 + strlen(p[0].name) +
	     strlen(" vs ") + strlen(p[1].name), " ");
    attron(COLOR_PAIR(colorChoice[1]));
    mvprintw(5, 48, "*");
    attroff(COLOR_PAIR(colorChoice[1]));
    break;
  case 2:
    mvprintw(5, 48, " ");
    attron(COLOR_PAIR(colorChoice[2]));
    mvprintw(5, 51 + strlen(p[0].name) +
	     strlen(" vs ") + strlen(p[1].name), "*");
    attroff(COLOR_PAIR(colorChoice[2]));
    break;
  }
  attron(A_BOLD);
  mvprintw(5, 50, "%s VS %s", p[0].name, p[1].name);
  attroff(A_BOLD);
  /* print current score */
  mvprintw(7, 50, "Score for the current session:");
  mvprintw(8, 50, "%s: %d", p[0].name, curPointsPlayer[0]);
  mvprintw(9, 50, "%s: %d", p[1].name, curPointsPlayer[1]);
  /* print total score for each player */
  mvprintw(11, 50, "Lifetime Player Score:");
  mvprintw(12, 50, "%s: %d", p[0].name, p[0].score);
  mvprintw(13, 50, "%s: %d", p[1].name, p[1].score);
  if(popOutActive == 1) {
    mvprintw(18, 50, "POPOUT: O");
  }
  else {
    mvprintw(18, 50, "Default Key bindings:");
  }
  mvprintw(19, 50, "LEFT: A | LEFT: <-");
  mvprintw(20, 50, "RIGHT: D | RIGHT: ->");
  mvprintw(21, 50, "_____________________________");
  mvprintw(22, 50, "ACTION: SPACE | ACTION: ENTER");
  mvprintw(23, 50, "SAVE:S | QUIT:Q | PAUSE:P");
}
/* Put zeroes in the boardState matrix */
void ResetBoard() {
  int i, j;
  for(i = 0; i < 8; i++)
    for(j = 0; j < 9; j++)
      boardState[i][j] = 0;
}
void GameIsDraw() {
  char *msg = "DRAW!\n PLAY AGAIN?\n YES(y) / NO(n)";
  int ch;
  DrawPrompt(msg);
  do {
    ch = getch();
  }while(ch != 'y' && ch != 'n');
  if(ch == 'n') {
    UpdatePlayer(p[0]);
    UpdatePlayer(p[1]);
    Quit();
    endwin();
    exit(0);
  }
  if(ch == 'y') {
    ResetBoard();
    DrawBoardLayout();
    DrawBoard();
  }
}
void PopOut(int colChosen) {
  int i, winningCombinations[2] = {0};
  for(i = 6; i >= 1; i--) {
    if(boardState[i][colChosen + 1] != 0) {
      boardState[i][colChosen + 1] = 0;
      DrawBoard();
      napms(180);
      boardState[i][colChosen + 1] = boardState[i - 1][colChosen + 1];
    }
  }
  for(i = 6; i >= 1; i--) {
    if(boardState[i][colChosen + 1] != 0) {
      if(CheckEndOfGameFromPosition(i, colChosen + 1)) {
	BlinkWinningPositions();
	winningCombinations[boardState[i][colChosen + 1] - 1]++;
      }
    }
  }
  if(winningCombinations[0] > 0 && winningCombinations[1] > 0) {
    GameIsDraw();
  }
  else
    for(i = 0; i < 2; i++) {
      if(winningCombinations[i] > 0) {
	char msg[100];
	int ch;
	colsFull = 0;
	sprintf(msg, "%s WINS!\n PLAY AGAIN?\n YES(y)/NO(n)",
		p[i].name);
	curPointsPlayer[i]++;
	p[i].score++;
	PrintScore();
	DrawPrompt(msg);
	while((ch = getch()) != 'y' && ch != 'n');
	if(ch == 'n') {
	  UpdatePlayer(p[0]);
	  UpdatePlayer(p[1]);
	  Quit();
	  endwin();
	  exit(0);
	}
	if(ch == 'y') {
	  ResetBoard();
	  DrawBoardLayout();
	  DrawBoard();
	}
      }
    }
}
/* Update variables and print message when the game is over */
void GameOver() {
  char msg[100];
  int ch;
  colsFull = 0;
  sprintf(msg, "%s WINS!\n PLAY AGAIN OR EXIT?\n YES(y)/NO(n)",
	  p[turn - 1].name);
  curPointsPlayer[turn - 1]++;
  p[turn - 1].score++;
  PrintScore();
  BlinkWinningPositions();
  DrawPrompt(msg);
  while((ch = getch()) != 'y' && ch != 'n');
  if(ch == 'n') {
    UpdatePlayer(p[0]);
    UpdatePlayer(p[1]);
    Quit();
    endwin();
    exit(0);
  }
  if(ch == 'y') {
    ResetBoard();
    DrawBoardLayout();
    DrawBoard();
  }
}
