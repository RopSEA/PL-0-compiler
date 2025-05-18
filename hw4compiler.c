// Remi Roper , Hayden sandler
// System Softwares
// Euripides montagne
// COP 3402

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token Struct
typedef enum {
  oddsym = 1,
  identsym = 2,
  numbersym = 3,
  plussym = 4,
  minussym = 5,
  multsym = 6,
  slashsym = 7,
  eqsym = 9,
  neqsym = 10,
  lessym = 11,
  leqsym = 12,
  gtrsym = 13,
  geqsym = 14,
  lparentsym = 15,
  rparentsym = 16,
  commasym = 17,
  semicolonsym = 18,
  periodsym = 19,
  becomessym = 20,
  beginsym = 21,
  endsym = 22,
  ifsym = 23,
  thensym = 24,
  whilesym = 25,
  dosym = 26,
  callsym = 27,
  constsym = 28,
  varsym = 29,
  procsym = 30,
  writesym = 31,
  readsym = 32,
} token_type;

//  for symbol table
typedef struct {
  int kind;      // const = 1, var = 2, proc = 3
  char name[10]; // name up to 11 chars
  int val;       // number (ASCII value)
  int level;     // L level
  int addr;      // M address
  int mark;      // to indicate unavailable or deleted
} symbol;

typedef struct {
  char op[3];
  int l, m;
} assem;

assem assemCode[500];
int assemIndex = 0;

int elf[1500];

symbol symbolTable[500];
int currsymb = 0;
int tokenIndex = 0;
int currLevel = 0;

char **variables;
char **values;

token_type t[500] = {0};

// function sigs
void printTokenList(token_type *tokens, char **vars, char **val, int len);
void LexLoop(token_type *t, char *lex, char **var, char **val, int len,
             FILE *out);
char *getTemp(char *input, int i);
int checkLen(token_type *tokens, char **vars, char **val);

int symbolTableCheck();
void program();
void block();
void const_declaration();
int var_declaration();
void statement();
void expression();
void condition();
void term();
void factor();
void printSymTable();
void emit(char op[3], int m);
int getcurrVal();
void outputElf();

int main(int argc, char *argv[]) {

  int i = 0, j = 0;
  int len;
  // token_type t[1048] = {0};
  FILE *file;
  FILE *out;
  char lex[1048] = {0};
  // char **variables;
  // char **values;

  // allocate Memory for 2D arrays
  variables = malloc(sizeof(char *) * 256);
  values = malloc(sizeof(char *) * 256);

  // File IO
  if ((file = fopen(argv[1], "r")) == NULL) {
    return '\0';
  }

  out = fopen("output.txt", "w");

  // prints out file to output
  printf("Source Program:\n");
  while (fscanf(file, "%c", &lex[i]) != EOF) {
    printf("%c", lex[i]);
    fprintf(out, "%c", lex[i]);
    // printf("%c", lex[i]);
    i++;
  }
  len = i;
  fprintf(out, "\n");
  printf("\n");
  fclose(file);

  i = 0;

  // printf("Lexeme Table:\n\n");
  // printf("lexeme \t\t token type\n");
  fprintf(out, "Lexeme Table:\n\n");
  fprintf(out, "lexeme \t\t token type\n");

  fclose(out);

  // Lexical Analyzer Loop
  LexLoop(t, lex, variables, values, len, out);

  // Token List
  int count = checkLen(t, variables, values);
  printTokenList(t, variables, values, count);

  program();
  printf("No errors, program is syntactically correct\n\n");
  printSymTable();
  outputElf();

  return 0;
}

// Calculates length of token list
int checkLen(token_type *tokens, char **vars, char **val) {
  int i = 0;
  int count = 0;
  while (tokens[i] != 0) {
    i++;
    count++;
  }
  return count;
}

// Lexicalogical analyzer loop
void LexLoop(token_type *t, char *lex, char **var, char **val, int len,
             FILE *out) {
  char *temp;
  int tindex = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  int m = 0;

  out = fopen("output.txt", "a");

  temp = malloc(sizeof(char) * 12);

  while (i < len) {

    if (lex[i] == -1) {
      break;
    }
    // token 1
    else if (strcmp("odd", getTemp(lex, i)) == 0) {
      // printf("skip          \t\t1\n");
      fprintf(out, "odd          \t\t1\n");
      i += 4;
      t[tindex] = 1;
      tindex++;
    } // checks for comments and skips them
    else if (lex[i] == '/' && lex[i + 1] == '*') {
      i += 2;
      while (lex[i] != '*' && lex[i + 1] != '/') {
        i++;
      }
      i += 2;
      continue;
    }
    // check for num token:3
    else if (isdigit(lex[i]) != 0) {

      val[l] = malloc(sizeof(char) * 6);
      while (isdigit(lex[i]) != 0) {
        if (m > 5) {
          break;
        }
        val[l][m] = lex[i];
        // printf("%c", lex[i]);
        fprintf(out, "%c", lex[i]);
        i++;
        m++;
      }
      if (m > 5) {
        // printf("...       \t\terror the number is too long\n");
        fprintf(out, "...       \t\terror the number is too long\n");
        free(val[l]);
      } else {
        // printf("       \t\t3\n");
        fprintf(out, "       \t\t3\n");
        l++;
        t[tindex] = 3;
        tindex++;
      }

      while (isdigit(lex[i]) != 0) {
        i++;
      }
      m = 0;
      continue;
    }
    // + sign token:4
    else if (lex[i] == '+') {
      // printf("+       \t\t4\n");
      fprintf(out, "+       \t\t4\n");
      t[tindex] = 4;
      i++;
      tindex++;
      continue;
    }
    // token:5
    else if (lex[i] == '-') {
      // printf("-          \t\t 5\n");
      fprintf(out, "-          \t\t 5\n");
      i++;
      t[tindex] = 5;
      tindex++;
      continue;
    }
    // multiply symbol (asterik) token: 6
    else if (lex[i] == '*') {
      // printf("*          \t\t6\n");
      fprintf(out, "*          \t\t6\n");
      i++;
      t[tindex] = 6;
      tindex++;
      continue;
    }
    // divide symbol token: 7
    else if (lex[i] == '/') {
      // printf("/          \t\t7\n");
      fprintf(out, "/          \t\t7\n");
      i++;
      t[tindex] = 7;
      tindex++;
      continue;
    }
    // token:9 equal symbol
    else if (lex[i] == '=') {
      // printf("=          \t\t9\n");
      fprintf(out, "=          \t\t9\n");
      i++;
      t[tindex] = 9;
      tindex++;
      continue;
    }
    // token: 10
    else if (lex[i] == '<' && lex[i + 1] == '>') {
      // printf("<>          \t\t10\n");
      fprintf(out, "<>          \t\t10\n");
      i += 2;
      t[tindex] = 10;
      tindex++;
      continue;
    }

    // token: 14
    else if (lex[i] == '<' && lex[i + 1] == '=') {
      // printf(">=          \t\t14\n");
      fprintf(out, ">=          \t\t14\n");
      i += 2;
      t[tindex] = 14;
      tindex++;
      continue;
    }
    // token: 12
    else if (lex[i] == '<' && lex[i + 1] == '=') {
      // printf("<=          \t\t12\n");
      fprintf(out, "<=          \t\t12\n");
      i += 2;
      t[tindex] = 12;
      tindex++;
      continue;

    }
    // token: 11
    else if (lex[i] == '<') {
      // printf("<          \t\t11\n");
      fprintf(out, "<          \t\t11\n");
      i++;
      t[tindex] = 11;
      tindex++;
      continue;
    }
    // token: 12
    else if (lex[i] == '<' && lex[i + 1] == '=') {
      //  printf("<=          \t\t12\n");
      fprintf(out, "<=          \t\t12\n");
      i += 2;
      t[tindex] = 12;
      tindex++;
      continue;

    }
    // token: 13
    else if (lex[i] == '>') {
      //  printf(">          \t\t13\n");
      fprintf(out, ">          \t\t13\n");
      i++;
      t[tindex] = 13;
      tindex++;
      continue;

    }
    // token: 14
    else if (lex[i] == '<' && lex[i + 1] == '=') {
      // printf(">=          \t\t14\n");
      fprintf(out, ">=          \t\t14\n");
      i += 2;
      t[tindex] = 14;
      tindex++;
      continue;
    }
    // token: 15
    else if (lex[i] == '(') {
      // printf("(          \t\t15\n");
      fprintf(out, "(          \t\t15\n");
      i++;
      t[tindex] = 15;
      tindex++;
      continue;

    }
    // token: 16
    else if (lex[i] == ')') {
      // printf("(          \t\t16\n");
      fprintf(out, "(          \t\t16\n");
      i++;
      t[tindex] = 16;
      tindex++;
      continue;
    }
    // token: 17
    else if (lex[i] == ',') {
      // printf(", 	       \t\t17\n");
      fprintf(out, ", 	       \t\t17\n");
      i++;
      t[tindex] = 17;
      tindex++;
      continue;
    }
    // token: 18
    else if (lex[i] == ';') {
      // printf(";          \t\t18\n");
      fprintf(out, ";          \t\t18\n");
      i++;
      t[tindex] = 18;
      tindex++;
      continue;
    }
    // token: 19
    else if (lex[i] == '.') {
      // printf(".          \t\t19\n");
      fprintf(out, ".          \t\t19\n");
      i++;
      t[tindex] = 19;
      tindex++;
      continue;
    }
    // token: 20
    else if (lex[i] == ':' && lex[i + 1] == '=') {
      // printf(":=         \t\t20\n");
      fprintf(out, ":=         \t\t20\n");
      i += 2;
      t[tindex] = 20;
      tindex++;
      continue;
    }
    // token: 21
    else if (strcmp("begin", getTemp(lex, i)) == 0) {
      // printf("begin      \t\t21\n");
      fprintf(out, "begin      \t\t21\n");
      i += 5;
      t[tindex] = 21;
      tindex++;
      continue;
    }
    // token: 22
    else if (strcmp("end", getTemp(lex, i)) == 0) {
      // printf("end        \t\t22\n");
      fprintf(out, "end        \t\t22\n");
      i += 3;
      t[tindex] = 22;
      tindex++;
      continue;
    }
    // token: 23
    else if (strcmp("if", getTemp(lex, i)) == 0) {
      //  printf("if         \t\t23\n");
      fprintf(out, "if         \t\t23\n");
      i += 2;
      t[tindex] = 23;
      tindex++;
      continue;
    }
    // token : 24
    else if (strcmp("then", getTemp(lex, i)) == 0) {
      //  printf("then       \t\t24\n");
      fprintf(out, "then       \t\t24\n");
      i += 4;
      t[tindex] = 24;
      tindex++;
      continue;
    }
    // token: 25
    else if (strcmp("while", getTemp(lex, i)) == 0) {
      //  printf("while      \t\t25\n");
      fprintf(out, "while      \t\t25\n");

      i += 5;
      t[tindex] = 25;
      tindex++;
      continue;
    }
    // token: 26
    else if (strcmp("do", getTemp(lex, i)) == 0) {
      // printf("do         \t\t26\n");
      fprintf(out, "do         \t\t26\n");

      i += 2;
      t[tindex] = 26;
      tindex++;
      continue;
    }
    // token: 27
    else if (strcmp("call", getTemp(lex, i)) == 0) {
      fprintf(out, "call       \t\t27\n");

      i += 4;
      t[tindex] = 27;
      tindex++;
      continue;
    }
    // token: 28
    else if (strcmp("const", getTemp(lex, i)) == 0) {
      // printf("const      \t\t28\n");
      fprintf(out, "const      \t\t28\n");

      i += 5;
      t[tindex] = 28;
      tindex++;
      continue;
    }
    // token: 29
    else if (strcmp("var", getTemp(lex, i)) == 0) {
      // printf("var        \t\t29\n");
      fprintf(out, "var        \t\t29\n");
      i += 3;
      t[tindex] = 29;
      tindex++;
      continue;
    }
    // token: 30
    else if (strcmp("procedure", getTemp(lex, i)) == 0) {
      fprintf(out, "procedure  \t\t 30\n");

      i += 9;
      t[tindex] = 30;
      tindex++;
      continue;
    }
    // token: 31
    else if (strcmp("write", getTemp(lex, i)) == 0) {
      // printf("write      \t\t31\n");
      fprintf(out, "write      \t\t31\n");

      i += 5;
      t[tindex] = 31;
      tindex++;
      continue;
    }
    // token: 32
    else if (strcmp("read", getTemp(lex, i)) == 0) {
      // printf("read       \t\t32\n");
      fprintf(out, "read       \t\t32\n");

      i += 4;
      t[tindex] = 32;
      tindex++;
      continue;
    }
    // token: 33
    else if (strcmp("else", getTemp(lex, i)) == 0) {
      // printf("else       \t\t33\n");
      fprintf(out, "else       \t\t33\n");
      i += 4;
      t[tindex] = 33;
      tindex++;
      continue;

    }
    // Token: 2
    else if (isalpha(lex[i]) != 0) {

      var[j] = malloc(sizeof(char) * 12);
      int index = 0;

      while (isalpha(lex[i]) != 0 || isdigit(lex[i]) != 0) {
        if (k > 11) {
          break;
        }
        // printf("%c", lex[i]);
        fprintf(out, "%c", lex[i]);
        var[j][k] = lex[i];
        k++;
        i++;
      }

      if (k > 11) {
        printf("...    error: inentifier name is too long\n");
        fprintf(out, "...    error: inentifier name is too long\n");
        free(var[j]);
      } else {
        // printf("       \t\t 2\n");
        fprintf(out, "       \t\t 2\n");
        j++;
        t[tindex] = 2;
        tindex++;
      }

      while (isalpha(lex[i]) != 0 || isdigit(lex[i]) != 0) {
        i++;
      }
      k = 0;
      continue;
    }
    // checks for null terminater/whitespace token:1
    else if (isgraph(lex[i]) == 0) {
      i++;
      continue;
    } else if (isascii(lex[i]) != 0 && isalpha(lex[i]) == 0 &&
               isdigit(lex[i]) == 0) {
      // printf("%c       \t\t Error: Non valid symbol\n", lex[i]);
      fprintf(out, "%c       \t\t Error: Non valid symbol\n", lex[i]);
      i++;
      continue;
    }

    i++;
  }

  fclose(out);
}

// Collects next word
char *getTemp(char *input, int i) {
  int ind = 0;
  char *temp;
  temp = calloc(0, sizeof(char) * 12);
  while (ind < 11) {

    if (isspace(input[i]) || input[i] == 0) {
      break;
    }

    if ((input[i] > 64 && input[i] < 91) || (input[i] > 96 && input[i] < 123) ||
        (input[i] > 47 && input[i] < 58)) {
      temp[ind] = input[i];
      ind++;
    } else {
      break;
    }

    i++;
  }
  return temp;
}

// print helper function for final tokens
void printTokenList(token_type *tokens, char **vars, char **val, int len) {
  FILE *out;
  int j = 0;
  int k = 0;
  int varIndx = 0, valIndx = 0;

  out = fopen("output.txt", "a");

  printf("\nToken List:\n");
  fprintf(out, "\nToken List:\n");

  for (int i = 0; i < len; i++) {
    printf("%d ", tokens[i]);
    fprintf(out, "%d ", tokens[i]);
    if (tokens[i] == 2) { // var name
      printf("%s ", vars[varIndx]);
      fprintf(out, "%s ", vars[varIndx]);
      varIndx++;
    } else if (tokens[i] == 3) { // var value
      printf("%s ", val[valIndx]);
      fprintf(out, "%s ", val[valIndx]);
      valIndx++;
    }
  }
  printf("\n");
  fprintf(out, "\n");

  fclose(out);
}

// gets current variable name
char *getCurrStr() {
  int i;
  int currName = -1;

  for (i = 0; i < tokenIndex + 2; i++) {
    if (t[i] == 2) {
      currName++;
    }
  }

  return variables[currName];
}

// gets current value from tokens
int getcurrVal() {
  int indx = 0;
  for (int i = 0; i < tokenIndex; i++) {
    if (t[i] == 3) {
      indx++;
    }
  }
  return indx;
}

// checks if  current string is in the symbol table
int symbolTableCheck() {
  int i;
  char *str;
  str = getCurrStr();
  for (i = 0; i < 500; i++) {
    if (strcmp(str, symbolTable[i].name) == 0 &&
        symbolTable[i].level <= currLevel && symbolTable[i].mark == 0) {
      return i;
    }
  }
  return -1;
}

// program
void program() {
  int currToken = -1;
  emit("JMP", 3);
  block();
  if (t[tokenIndex] != periodsym) {
    printf("Error: program should end with period");
    exit(0);
  }
  emit("SYS", 3);
}

// block
void block() {
  int marksInd, j;
  if (t[tokenIndex] == constsym) {
    const_declaration();
  }
  int numVars = var_declaration();

  emit("INC", 3 + numVars);

  // procedure
  while (t[tokenIndex] == procsym) {

    tokenIndex++;
    if (t[tokenIndex] != identsym) {
      printf("Error: identifier symbol expected");
      exit(0);
    }
    tokenIndex++;

    if (t[tokenIndex] != semicolonsym) {
      printf("Error: ; symbol expected");
      exit(0);
    }

    char *currStr;
    currStr = malloc(sizeof(char) * 12);
    currStr = getCurrStr();
    int currStrlen = strlen(currStr);
    for (int varlen = 0; varlen < currStrlen; varlen++) {
      symbolTable[currsymb].name[varlen] = currStr[varlen];
    }

    symbolTable[currsymb].addr = numVars;
    symbolTable[currsymb].kind = 3;
    symbolTable[currsymb].level = currLevel;

    marksInd = currsymb;

    currsymb++;
    numVars++;
    currLevel++;
    tokenIndex++;

    block();

    if (t[tokenIndex] != semicolonsym) {
      printf("Error: ; symbol expected");
      exit(0);
      // error
    }

    tokenIndex++;
    currLevel--;
    // mark symbols
    while (marksInd < currsymb) {
      symbolTable[marksInd].mark = 1;
      marksInd++;
    }
  }

  statement();
  symbolTable[currsymb].kind = 1;
}

void const_declaration() {
  if (t[tokenIndex] == constsym) {
    tokenIndex++;

    if (symbolTableCheck() != -1) {
      printf("Error: symbol not found");
      exit(0);
      // error
    }

    tokenIndex++;
    if (t[tokenIndex] != eqsym) {
      printf("Error: equal symbol expected");
      exit(0);
      // error
    }
    tokenIndex++;
    if (t[tokenIndex] != numbersym) {
      printf("Error: number symbol expected ");
      exit(0);
      // error
    }

    char *currStr;
    currStr = malloc(sizeof(char) * 12);
    currStr = getCurrStr();
    int currStrlen = strlen(currStr);
    for (int varlen = 0; varlen < currStrlen; varlen++) {
      symbolTable[currsymb].name[varlen] = currStr[varlen];
    }
    symbolTable[currsymb].kind = 1;
    symbolTable[currsymb].val = atoi(values[getcurrVal()]);
    symbolTable[currsymb].addr = 0;
    symbolTable[currsymb].mark = 0;
    currsymb++;
    tokenIndex += 1;
  }

  while (t[tokenIndex] == commasym) {
    tokenIndex++;
    if (t[tokenIndex] != identsym) {
      printf("Error: indentifeir expected");
      exit(0);
      // error
    }
    if (symbolTableCheck() != -1) {
      printf("Error: symbol not found");
      exit(0);
      // error
    }
    tokenIndex++;
    if (t[tokenIndex] != eqsym) {
      printf("Error: equal symbol expected");
      exit(0);
      // error
    }
    tokenIndex++;
    if (t[tokenIndex] != numbersym) {
      printf("Error: number symbol expected ");
      exit(0);
      // error
    }

    char *currStr;
    currStr = malloc(sizeof(char) * 12);
    currStr = getCurrStr();
    int currStrlen = strlen(currStr);
    for (int varlen = 0; varlen < currStrlen; varlen++) {
      symbolTable[currsymb].name[varlen] = currStr[varlen];
    }
    symbolTable[currsymb].kind = 1;
    symbolTable[currsymb].val = atoi(values[getcurrVal()]);
    symbolTable[currsymb].addr = 0;
    symbolTable[currsymb].mark = 0;
    currsymb++;
    // add to symbol table (kind 1, saved name, number, 0, 0)

    currsymb++;
    tokenIndex++;
  }

  if (t[tokenIndex] != semicolonsym) {
    // error
    printf("%d", tokenIndex);
    printf("Error: ; symbol expected");
    exit(0);
  }

  tokenIndex++;
}

int var_declaration() {
  int numVars = 0;
  // need token
  if (t[tokenIndex] == varsym) {
    // increase number of vars
    numVars++;
    // tokenIndex++;

    if (t[tokenIndex] != varsym) {
      printf("Error: Not a variable");
      exit(0);
    }

    // put in the token
    if (symbolTableCheck() != -1) {
      printf("%d ERROR: Var already in symbol table", tokenIndex);
      exit(0);
    }
    // add to symbol table (kind 2, ident, 0, 0, var# + 2)
    symbolTable[currsymb].kind = 2;

    char *currStr;
    currStr = malloc(sizeof(char) * 12);
    currStr = getCurrStr();
    int currStrlen = strlen(currStr);
    for (int varlen = 0; varlen < currStrlen; varlen++) {
      symbolTable[currsymb].name[varlen] = currStr[varlen];
    }

    symbolTable[currsymb].kind = 2;
    symbolTable[currsymb].val = 0;
    symbolTable[currsymb].addr = 2 + numVars;
    symbolTable[currsymb].mark = 0;
    symbolTable[currsymb].level = currLevel;
    currsymb++;
    tokenIndex += 2;

    while (t[tokenIndex] == commasym) {
      // increase number of vars
      numVars++;
      tokenIndex++;

      // put in the token
      if (symbolTableCheck() != -1) {
        printf("ERROR: Var already in symbol table");
        exit(0);
      }
      // add to symbol table

      currStr = malloc(sizeof(char) * 12);
      currStr = getCurrStr();
      currStrlen = strlen(currStr);
      for (int varlen = 0; varlen < currStrlen; varlen++) {
        symbolTable[currsymb].name[varlen] = currStr[varlen];
      }
      symbolTable[currsymb].kind = 2;
      symbolTable[currsymb].val = 0;
      symbolTable[currsymb].addr = 2 + numVars;
      symbolTable[currsymb].mark = 0;
      symbolTable[currsymb].level = currLevel;
      currsymb++;
      tokenIndex++;
    }
  }
  if (t[tokenIndex] != semicolonsym) {
    // error
    printf("Error: ; token expected\n");
    exit(0);
  }

  tokenIndex++;
  return numVars;
}

void statement() {

  int jpcIdx = 0;
  int symIdx = -1;
  int loopIdx = 0;

  if (t[tokenIndex] == identsym) {
    symIdx = symbolTableCheck();
    if (symIdx == -1) {
      printf("Error: symbol not found in symbol table");
      exit(0);
      // error
    }

    if (symbolTable[symIdx].kind != 2) {
      printf("Error: identifier expected");
      exit(0);
      // error
    }
    tokenIndex++;
    if (t[tokenIndex] != becomessym) {
      printf("Error: := expected");
      exit(0);
      // error
    }
    tokenIndex++;
    expression();
    emit("STO", symbolTable[symIdx].addr);
    return;
  } else if (t[tokenIndex] == callsym) // callsym called
  {
    tokenIndex++;
    if (t[tokenIndex] != identsym) {
      printf("Error: identifier expected");
      exit(0);
    }
    tokenIndex++;
  }

  if (t[tokenIndex] == beginsym) {
    tokenIndex++;
    statement();
    while (t[tokenIndex] == semicolonsym) {
      tokenIndex++;
      statement();
    }

    if (t[tokenIndex] != endsym) {
      printf("Error: end symbol expected");
      exit(0);
      // error
    }
    tokenIndex++;
    return;
  }
  if (t[tokenIndex] == ifsym) {
    tokenIndex++;
    condition();
    jpcIdx = tokenIndex;
    emit("JPC", jpcIdx);
    if (t[tokenIndex] != thensym) {
      // error
      exit(0);
    }
    tokenIndex++;
    statement();
    // code[jpcIdx].M = tokenIndex;
    return;
  }

  if (t[tokenIndex] == whilesym) {
    tokenIndex++;
    loopIdx = tokenIndex;
    condition();

    if (t[tokenIndex] != dosym) {
      printf("Error: do symbol expected");
      exit(0);
      // error
    }
    tokenIndex++;
    jpcIdx = tokenIndex;
    emit("JPC", jpcIdx);
    statement();
    emit("JPC", jpcIdx);
    // emit JMP(M = loopIdx);
    // code[jpcIdx].M = tokenIndex;
    return;
  }

  if (t[tokenIndex] == readsym) {
    tokenIndex++;
    if (t[tokenIndex] != identsym) {
      printf("Error: identifier symbol expected");
      exit(0);
      // error
    }
    symIdx = symbolTableCheck();
    if (symIdx == -1) {
      printf("Error:identifier not found in symbol table");
      exit(0);
      // error
    }
    if (symbolTable[symIdx].kind != 2) {
      printf("Error: do symbol expected");
      exit(0);
      // error
    }
    tokenIndex++;
    // FIX
    emit("SYS", 2);
    emit("STO", symbolTable[symIdx].addr);
    return;
  }
  if (t[tokenIndex] == writesym) {
    tokenIndex++;
    expression();
    // FIX
    emit("SYS", 1);
    return;
  }
}

void condition() {
  if (t[tokenIndex] == oddsym) {
    tokenIndex++;
    expression();
    emit("OPR", 11);
  } else
    expression();
  if (t[tokenIndex] == eqsym) {
    tokenIndex++;
    expression();
    emit("OPR", 5);
  } else if (t[tokenIndex] == neqsym) {
    tokenIndex++;
    expression();
    emit("OPR", 6);
  } else if (t[tokenIndex] == lessym) {
    tokenIndex++;
    expression();
    emit("OPR", 7);
  } else if (t[tokenIndex] == leqsym) {
    tokenIndex++;
    expression();
    emit("OPR", 8);
  } else if (t[tokenIndex] == gtrsym) {
    tokenIndex++;
    expression();
    emit("OPR", 9);
  } else if (t[tokenIndex] == geqsym) {
    tokenIndex++;
    expression();
    emit("OPR", 10);
  } else {
    printf(" %d", tokenIndex);
    printf("Error: condition symbol expected");
    exit(0);
    // error;
  }
}

void expression() {

  while (t[tokenIndex] == plussym || t[tokenIndex] == minussym) {
    if (t[tokenIndex] == plussym) {
      tokenIndex++;
      term();
      emit("OPR", 1); // ADD
    } else {
      tokenIndex++;
      term();
      emit("OPR", 2); // SUB
    }
  }

  term();
  while (t[tokenIndex] == plussym || t[tokenIndex] == minussym) {
    if (t[tokenIndex] == plussym) {
      tokenIndex++;
      term();
      emit("OPR", 1); // ADD
    } else {
      tokenIndex++;
      term();
      emit("OPR", 2); // SUB
    }
  }
}

void term() {
  factor();
  while (t[tokenIndex] == multsym || t[tokenIndex] == slashsym) {
    if (t[tokenIndex] == multsym) {
      tokenIndex++;
      factor();
      emit("OPR", 3); // mul
    } else if (t[tokenIndex] == slashsym) {
      tokenIndex++;
      factor();
      emit("OPR", 4); // div
    }
  }
}

void factor() {
  int symIdx = 0;
  int val = 0;
  if (t[tokenIndex] == identsym) {
    symIdx = symbolTableCheck();
    if (symIdx == -1) {
      printf("Error: Symbol not found");
      exit(0);
    }

    if (symbolTable[symIdx].kind == 1) {
      emit("LIT", symbolTable[symIdx].val);
    } else {
      emit("LOD", symbolTable[symIdx].addr);
    }
    tokenIndex++;
  } else if (t[tokenIndex] == numbersym) {
    val = atoi(values[getcurrVal()]);
    emit("LIT", val);

    tokenIndex++;
  } else if (t[tokenIndex] == lparentsym) {
    tokenIndex++;
    expression();
    if (t[tokenIndex] != rparentsym) {
      printf("Error: expected ) identifier");
      exit(0);
    }
    tokenIndex++;
  }
}

void emit(char op[3], int m) {

  assemCode[assemIndex].l = currLevel;
  assemCode[assemIndex].m = m;
  assemCode[assemIndex].op[0] = op[0];
  assemCode[assemIndex].op[1] = op[1];
  assemCode[assemIndex].op[2] = op[2];
  assemIndex++;
}

void printSymTable() {
  printf("Assembly Code Table: \n\n");
  printf("Line   OP   L   M\n");
  for (int i = 0; i < assemIndex; i++) {
    printf("%d   %s   %d   %d\n", i, assemCode[i].op, assemCode[i].l,
           assemCode[i].m);
  }
  /*
  printf("Symbol Table: \n\n");
  printf("Kind | Name       | Value | Level | Address | Mark \n");
  printf("-----------------------------------------------------\n");
  for (int i = 0; i < currsymb; i++) {
    if (symbolTable[i].kind == 1) {
      printf(" %d |     %s  |   %d |   - |   %d |   %d \n", symbolTable[i].kind,
             symbolTable[i].name, symbolTable[i].val, symbolTable[i].addr,
             symbolTable[i].mark);
    } else {
      printf(" %d |     %s  |   %d |   %d |   %d |   %d \n",
             symbolTable[i].kind, symbolTable[i].name, symbolTable[i].val,
             symbolTable[i].level, symbolTable[i].addr, symbolTable[i].mark);
    }
  }
  */
}

// turns assembly code into elf file
void outputElf() {
  int i = 0;
  int idx = 0;
  int max = 0;
  FILE *oelf;

  while (idx < assemIndex) {
    if (strcmp(assemCode[idx].op, "LIT") == 0) {
      elf[i] = 1;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "OPR") == 0) {
      elf[i] = 2;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "LOD") == 0) {
      elf[i] = 3;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "STO") == 0) {
      elf[i] = 4;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "CAL") == 0) {
      elf[i] = 5;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "INC") == 0) {
      elf[i] = 6;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "JMP") == 0) {
      elf[i] = 7;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "JPC") == 0) {
      elf[i] = 8;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }

    if (strcmp(assemCode[idx].op, "SYS") == 0) {
      elf[i] = 9;
      elf[i + 1] = assemCode[idx].l;
      elf[i + 2] = assemCode[idx].m;
      idx++;
      i += 3;
      continue;
    }
  }
  max = i;
  i = 0;

  oelf = fopen("elf.txt", "w");

  while (i < max) {
    fprintf(oelf, "%d %d %d\n", elf[i], elf[i + 1], elf[i + 2]);
    i += 3;
  }

  fclose(oelf);
}

void globalMark() {
  int i;
  for (i = 0; i < 500; i++) {
    symbolTable[i].mark = 1;
  }
  return;
}