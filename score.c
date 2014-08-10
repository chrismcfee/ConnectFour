#include "connect4.h"

void AddPlayer(Player p) {
  if ((f = fopen("scoreDatabase.bin", "ab")) == NULL) {
    ErrorMessage("Error at opening score database");
  }
  fwrite(&p, sizeof(p), 1, f);
  fclose(f);
}

/* Returns 0 if Player not found
   Returns 1 otherwise */
int SearchPlayer(Player p) {
  Player temp;
  if ((f = fopen("scoreDatabase.bin", "rb")) == NULL) {
    ErrorMessage("Error at opening score database");
  }

  while((fread(&temp, sizeof(temp), 1, f)) == 1) {
    if(strcmp(p.name, temp.name) == 0) {
      fclose(f);
      return 1;
    }
  }
  fclose(f);
  return 0;
}

int GetPlayerScore(Player p) {
  Player temp;
  if ((f = fopen("scoreDatabase.bin", "rb")) == NULL) {
    ErrorMessage("Error at opening score database");
  }
  while((fread(&temp, sizeof(temp), 1, f)) == 1) {
    if(strcmp(p.name, temp.name) == 0)
      return temp.score;
  }
  fclose(f);
  return 0;
}

void UpdatePlayer(Player p) {
  Player temp;
  long pos;
  if ((f = fopen("scoreDatabase.bin", "rb+")) == NULL) {
    ErrorMessage("Error at opening score database");
  }
  while((fread(&temp, sizeof(temp), 1, f)) == 1)
    if(strcmp(temp.name, p.name) == 0) {
      pos = ftell(f) - sizeof(temp);
      break;
    }

  fseek(f, pos, 0);
  fwrite(&p, sizeof(p), 1, f);
  fclose(f);
}

void PrintDatabase() {
  f = fopen("scoreDatabase.bin", "rb");
  Player p;
  while((fread(&p, sizeof(p), 1, f)) == 1) {
    printf("%s has %d points\n", p.name, p.score);
  }
  fclose(f);
}
