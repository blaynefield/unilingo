#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct board {
  char cells[11][11] ;
  int letters[26];
};

struct board *board_stack = NULL;
size_t n_board_stack = 0;

char **words = NULL;
size_t n_words = 0;

struct board read_file(FILE *fp)
{
  FILE *words_file = fopen("grid", "r");
  char *w = NULL;
  size_t n_w = 0;
  ssize_t len;

  struct board b;
  for (int i = 0; i <= 10; i++) {
    for (int j = 0; j <= 10; j++) {
      b.cells[i][j] = '\0';
    }
  }

  int r = 0;
  while ((len = getline(&w, &n_w, words_file)) > 0) {
    w[len-1] = '\0';/* strip off newline */
    for (int i = 0; i <= 10; i++) {
      if (w[i] == '#') { w[i] = '\0'; }
      b.cells[r][i] = w[i];
    }
    r++;
  }
/*
  b.cells[0][0] = ' ';
  b.cells[1][0] = 'r';
  b.cells[1][6] = ' ';
  b.cells[2][0] = ' ';
  b.cells[2][1] = 'n';
  b.cells[2][2] = ' ';
  b.cells[2][3] = 'e';
  b.cells[2][4] = ' ';
  b.cells[2][5] = 't';
  b.cells[2][6] = 'e';
  b.cells[2][7] = ' ';
  b.cells[3][0] = ' ';
  b.cells[3][4] = 'r';
  b.cells[3][6] = ' ';
  b.cells[3][7] = 'o';
  b.cells[3][8] = ' ';
  b.cells[3][9] = ' ';
  b.cells[4][0] = ' ';
  b.cells[4][4] = 'o';
  b.cells[4][7] = ' ';
  b.cells[4][9] = 'e';
  b.cells[4][9] = 'e';
  b.cells[5][0] = 't';
  b.cells[5][2] = ' ';
  b.cells[5][3] = ' ';
  b.cells[5][4] = ' ';
  b.cells[5][5] = ' ';
  b.cells[5][6] = 't';
  b.cells[5][7] = 'e';
  b.cells[5][8] = ' ';
  b.cells[5][9] = 's';
  b.cells[6][0] = 'e';
  b.cells[6][4] = ' ';
  b.cells[6][6] = ' ';
  b.cells[6][7] = 'n';
  b.cells[6][8] = ' ';
  b.cells[7][0] = ' ';
  b.cells[7][1] = 'a';
  b.cells[7][2] = 'i';
  b.cells[7][3] = ' ';
  b.cells[7][4] = 'e';
  b.cells[7][6] = 'i';
  b.cells[7][8] = 'a';
  b.cells[8][4] = 'd';
  b.cells[8][6] = ' ';
  b.cells[8][7] = 'u';
  b.cells[8][8] = ' ';
  b.cells[8][9] = ' ';
  b.cells[9][6] = ' ';
*/
/*
  b.cells[0][0] = ' ';
  b.cells[0][1] = 'u';
  b.cells[0][2] = 'n';
  b.cells[0][3] = ' ';
  b.cells[0][5] = 's';
  b.cells[0][8] = ' ';
  b.cells[1][0] = 'o';
  b.cells[1][3] = 'a';
  b.cells[1][4] = ' ';
  b.cells[1][5] = ' ';
  b.cells[1][6] = 'a';
  b.cells[1][7] = ' ';
  b.cells[1][8] = 'i';
  b.cells[1][9] = ' ';
  b.cells[2][0] = 'o';
  b.cells[2][1] = 'n';
  b.cells[2][2] = ' ';
  b.cells[2][3] = ' ';
  b.cells[2][5] = 'b';
  b.cells[2][8] = ' ';
  b.cells[3][0] = ' ';
  b.cells[3][5] = ' ';
  b.cells[3][6] = ' ';
  b.cells[3][7] = ' ';
  b.cells[3][8] = 'e';
  b.cells[4][3] = ' ';
  b.cells[4][4] = 'h';
  b.cells[4][5] = ' ';
  b.cells[4][6] = 'n';
  b.cells[4][8] = 'd';
  b.cells[5][3] = 'i';
  b.cells[5][5] = ' ';
  b.cells[6][1] = ' ';
  b.cells[6][2] = ' ';
  b.cells[6][3] = 'n';
  b.cells[6][4] = ' ';
  b.cells[6][5] = 'o';
  b.cells[6][6] = ' ';
  b.cells[7][6] = 'l';
  b.cells[7][7] = ' ';
  b.cells[7][8] = ' ';
  b.cells[8][5] = ' ';
  b.cells[8][6] = 'e';
  b.cells[8][7] = 'n';
  b.cells[8][8] = 'o';
  b.cells[8][9] = ' ';
  b.cells[9][6] = ' ';
  b.cells[9][8] = 'p';
*/

  for (int i = 0; i < 26; i++) {
    b.letters[i] = 1;
  }
  return b;
}

void print_board(struct board *b)
{
  printf("BOARD:\n");
  for (int r = 0; r < 10; r++) {
    for (int c = 0; c < 10; c++) {
      char ch = b->cells[r][c] ? b->cells[r][c] : '*';
      printf("%c", ch);
    }
    printf("\n");
  }
}

int count_matches(const char *word, struct board *b)
{
  size_t word_len = strlen(word);
  int match_count = 0;
  for (int i = 0; i < n_words; i++) {
    size_t len = strlen(words[i]);
    //printf("word_len : %zu, len: %zu\n", word_len, len);
    int letters[26] = { 0 };
    if (len == word_len) {
      int matches = 1;
      for (int j = 0; j < len; j++) {
        if (word[j] != ' ' && word[j] != words[i][j]) {
          matches = 0;
        }
        if (words[i][j] < 'a' || words[i][j] > 'z') {
          matches = 0;
        } else if (word[j] == ' ' && !b->letters[words[i][j]-'a']) {
          matches = 0;
        } else if (word[j] == ' ' && letters[words[i][j]-'a']) {
          //matches = 0;
        }
        letters[words[i][j]-'a'] = 1;
      }
      if (matches) {
        match_count++;
      }
    }
  }
  return match_count;
}

int word_matches(const char *word, int start, struct board *b)
{
  size_t word_len = strlen(word);
  for (int i = start; i < n_words; i++) {
    size_t len = strlen(words[i]);
    //printf("word_len : %zu, len: %zu\n", word_len, len);
    int letters[26] = { 0 };
    if (len == word_len) {
      int matches = 1;
      for (int j = 0; j < len; j++) {
        if (word[j] != ' ' && word[j] != words[i][j]) {
          matches = 0;
        }
        if (words[i][j] < 'a' || words[i][j] > 'z') {
          matches = 0;
        } else if (word[j] == ' ' && !b->letters[words[i][j]-'a']) {
          matches = 0;
        } else if (word[j] == ' ' && letters[words[i][j]-'a']) {
      //    matches = 0;
        }
        letters[words[i][j]-'a'] = 1;
      }
      if (matches) {
        return i;
      }
    }
  }
  return -1;
}

struct board copy_board_word_right(struct board *b1, int rr, int cc, int w)
{
  struct board b;
  for (int r = 0; r <= 10; r++) {
    for (int c = 0; c <= 10; c++) {
      b.cells[r][c] = b1->cells[r][c];
    }
  }
  for (int i = 0 ; i < 26; i++) {
    b.letters[i] = b1->letters[i];
  }

  const char *word = words[w];
  size_t len = strlen(word);
  for (int i = 0; i < len; i++) {
    if (b.cells[rr][cc+i] == ' ') {
      b.letters[word[i]-'a'] = 0;
    }
    b.cells[rr][cc+i] = word[i];
  }
  return b;
}

struct board copy_board_word_down(struct board *b1, int rr, int cc, int w)
{
  struct board b;
  for (int r = 0; r <= 10; r++) {
    for (int c = 0; c <= 10; c++) {
      b.cells[r][c] = b1->cells[r][c];
    }
  }
  for (int i =0 ; i < 26; i++) {
    b.letters[i] = b1->letters[i];
  }

  const char *word = words[w];
  size_t len = strlen(word);
  for (int i = 0; i < len; i++) {
    if (b.cells[rr+i][cc] == ' ') {
      b.letters[word[i]-'a'] = 0;
    }
    b.cells[rr+i][cc] = word[i];
  }
  return b;
}

struct board copy_board(struct board *b1, int rr, int cc, char ch)
{
  struct board b;
  for (int r = 0; r <= 10; r++) {
    for (int c = 0; c <= 10; c++) {
      b.cells[r][c] = b1->cells[r][c];
    }
  }
  for (int i =0 ; i < 26; i++) {
    b.letters[i] = b1->letters[i];
  }
  b.cells[rr][cc] = ch;
  b.letters[ch-'a'] = 0;
  return b;
}

int generate_candidates(struct board *b)
{
  char stack[11];
  int n_stack = 0;
  int smallest = n_words;
  int best_r = 0;
  int best_c = 0;
  int direction = 0;
  int invalid = 0;

  int all_done = 1;
  for (int c = 0; c <= 10; c++) {
    for (int r = 0; r <= 10; r++) {
      if (b->cells[r][c] == ' ') { all_done = 0; }
    }
  }
  if (all_done) { printf("GAME WON!\n"); print_board(b); exit(42); }
      
    

  /* see what the columns look like */
  for (int c = 0; c <= 10; c++) {
    int spaces = 0;
    n_stack = 0;
    for (int r = 0; r <= 10; r++) {
      if (b->cells[r][c]) {
        stack[n_stack++] = b->cells[r][c];
        if (b->cells[r][c] == ' ') spaces++;
      } else {
        stack[n_stack] = '\0';
        if (n_stack > 1) {
          int count = count_matches(stack, b);
          //printf("(%d, %d) -> %d\n", r - n_stack, c, count);
          if (count == 0) { invalid = 1; }
          if (spaces && count < smallest) {
            best_r = r - n_stack;
            best_c = c;
            smallest = count;
            direction = 0;
          }
        }
        
        n_stack = 0;
        spaces = 0; 
      }
    }
  }

  //printf("checking rows\n");

  for (int r = 0; r <= 10; r++) {
    int spaces = 0;
    n_stack = 0;
    for (int c = 0; c <= 10; c++) {
      if (b->cells[r][c]) {
        stack[n_stack++] = b->cells[r][c];
        if (b->cells[r][c] == ' ') spaces++;
      } else {
        stack[n_stack] = '\0';
        if (n_stack > 1) {
          int count = count_matches(stack, b);
          //printf("(%d, %d) -> %d\n", r, c - n_stack, count);
          if (count == 0) { invalid = 1; }
          if (spaces && count < smallest) {
            best_r = r;
            best_c = c - n_stack;
            smallest = count;
            direction = 1;
          }
        }
        
        n_stack = 0;
        spaces = 0; 
      }
    }
  }

  int generated = 0;
  if (invalid) { return 0; }
  if (direction == 0) {
    n_stack = 0;
    for (int r = best_r; b->cells[r][best_c]; r++) {
      stack[n_stack++] = b->cells[r][best_c];
    }
    stack[n_stack] = '\0';
    int start = word_matches(stack, 0, b);
    while (start != -1) { 
      generated++;
      board_stack[n_board_stack++] = copy_board_word_down(b, best_r, best_c, start);
      start = word_matches(stack, start+1, b);
    }
  }
  if (direction == 1) {
    n_stack = 0;
    for (int c = best_c; b->cells[best_r][c]; c++) {
      stack[n_stack++] = b->cells[best_r][c];
    }
    stack[n_stack] = '\0';
    int start = word_matches(stack, 0, b);
    while (start != -1) { 
      generated++;
      board_stack[n_board_stack++] = copy_board_word_right(b, best_r, best_c, start);
      start = word_matches(stack, start+1, b);
    }
  }

  assert(generated == smallest);
}

int generate_vertical_regexes(struct board *b, int insert)
{
  char stack[10];
  int n_stack = 0;

  for (int c = 0; c < 10; c++) {
    int spaces = 0;
    n_stack = 0;
    for (int r = 0; r < 10; r++) {
      if (b->cells[r][c]) {
        stack[n_stack++] = b->cells[r][c];
        if (b->cells[r][c] == ' ') spaces++;
      }

      if ((!b->cells[r][c] || r == 9)) {
        if (spaces) {
          stack[n_stack] = '\0';
          if (n_stack > 1) {
            int count = count_matches(stack, b);
            printf("(%d, %d) -> %d\n", r, c, count);
          }
        }
        n_stack = 0;
        spaces = 0; 
      }
    }
/*
    n_stack = 0;
    for (int r = 0; r < 10; r++) {
      if (b->cells[r][c]) {
        stack[n_stack++] = b->cells[r][c];
      } 

      if (!b->cells[r][c] || r == 9) {
        stack[n_stack++] = '\0';
        if (n_stack > 2) {
          int start = word_matches(stack, 0, b);
          if (start == -1) { return 0; }
//struct board copy_board_word_down(struct board *b1, int rr, int cc, int w)
          while (start != -1) { 
            board_stack[n_board_stack++] = copy_board_word_down(b, r-n_stack+1+(r==9?1:0), c, start);  
            start = word_matches(stack, start+1, b);
          }
        }
        n_stack = 0;
      }
    }
*/
  }
  return 1;
}

int generate_horizontal_regexes(struct board *b, int insert)
{
  char stack[10];
  int n_stack = 0;

  for (int r = 0; r < 10; r++) {
    int spaces = 0;
    n_stack = 0;
    for (int c = 0; c < 10; c++) {
      if (b->cells[r][c]) {
        stack[n_stack++] = b->cells[r][c];
        if (b->cells[r][c] == ' ') spaces++;
      }

      if ((!b->cells[r][c] || c == 9)) {
        if (spaces) {
          stack[n_stack] = '\0';
          if (n_stack > 1) {
            int start = word_matches(stack, 0, b);
            if (start == -1) { return 0; }
            if (insert) {
              while (start != -1) { 
                board_stack[n_board_stack++] = copy_board_word_right(b, r, c-n_stack+1-(c==9?0:1), start);  
                start = word_matches(stack, start+1, b);
              }
            }
          }
        }
        n_stack = 0;
        spaces = 0; 
      }
    }
  }
  return 1;
}

void read_words()
{
  FILE *words_file = fopen("words", "r");
  char *w = NULL;
  size_t n_w = 0;
  ssize_t len;

  while ((len = getline(&w, &n_w, words_file)) > 0) {
    w[len-1] = '\0';/* strip off newline */
    words[n_words++] = strdup(w);
    //printf("word: (%s)\n", w);
    if (n_words % 500 == 0) {
      fprintf(stderr, "read in %d words\n", n_words);
    }
  }
}

int done(struct board *b)
{
  for (int r = 0; r < 10; r++) {
    for (int c = 0; c < 10; c++) {
      if (b->cells[r][c] == ' ') {
        return 0;
      }
    }
  }
  return 1;
}

int main(int argc, char **argv)
{
  words = calloc(128000, sizeof(char *));
  struct board b = read_file(NULL);
  read_words();
  print_board(&b);

  size_t stack_size = 1000000;
  size_t start = 0;
  size_t end = 1;
  board_stack = calloc(stack_size, sizeof(struct board));
  board_stack[n_board_stack++] = b;

  //printf("match blue? %d\n", word_matches("blue"));
  //printf("match garbleblayne? %d\n", word_matches("garbleblayne"));
  //printf("match t.i.z? %d\n", word_matches("t i z"));
  while (n_board_stack) {
    printf("stack size: %d\n", n_board_stack);
//    b = board_stack[start];//--n_board_stack];
    b = board_stack[--n_board_stack];

   // print_board(&b);

    int g = generate_candidates(&b);
    //int mv = generate_vertical_regexes(&b, 0);
    //int mh = generate_horizontal_regexes(&b, 0);
    //if (mv && mh) {
    //  if (done(&b)) { return 0; }
    //  mv = generate_vertical_regexes(&b, 1);
//      mh = generate_horizontal_regexes(&b, 1);
   // }
/*
    printf("mv: %d, mh: %d\n", mv, mh);

    if (mv && mh) {
      for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
          if (b.cells[r][c] == ' ') {
            for (int ch = 0; ch < 26; ch++) {
              if (b.letters[ch]) {
                board_stack[end % stack_size] = copy_board(&b, r, c, 'a'+ch);
                end = (end + 1) % stack_size;
                n_board_stack--;
                
  //              board_stack[n_board_stack++] = copy_board(&b, r, c, 'a'+ch);
              }
            }
          }
        }
      }
    }
*/
  }
}
