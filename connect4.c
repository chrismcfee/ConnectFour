#include "connect4.h"
FILE *f;
FILE *saveFile;
Player p[2];
WINDOW *board, *prompt, *title;
int maxx, maxy, boardState[8][9], winningPositions[2][7], 
  colorChoice[3] = {0}, curPointsPlayer[2], turn, colsFull = 0,
  popOutActive = 0, difTime;
char menuList[3][20] = {"NEW", "LOAD", "EXIT"},
  players[2][30];

char saveFileName[15];
time_t start_time;

int main() {
  int newchosen = 1;
  int chosen;
  Initialize();
  getmaxyx(stdscr, maxy, maxx);
  chosen = InitializeMenu();
  switch(chosen) {
  case 0: /* New game */
    start_time = time(NULL);
    PopOutSelection();
    PlayerSelect();
    DrawBoardLayout();
    Play();
    break;
  case 1: /* Load saved game */
    Load();
    break;
  case 2: /* Quit */
    Quit();
    break;
  default:
    break;
  }
  endwin();
  return 0;
}
