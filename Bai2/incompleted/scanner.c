/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];
int checkRSEL = 0;
int lineNum = 0, colNum = 0;
/***************************************************************/

void skipBlank() {
   while (currentChar != EOF && charCodes[currentChar] == CHAR_SPACE) {
    readChar();
   }
}

void skipComment() {
  while(charCodes[currentChar] != CHAR_RPAR){
    readChar();
    if(currentChar == EOF){
      error(ERR_ENDOFCOMMENT, lineNo, colNo);
    }
  }
  readChar();
}

Token* readIdentKeyword(void) {  

  Token *token;
  token = makeToken(TK_IDENT, lineNo, colNo);
  int count = 0;

  while(charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT){
    if(count > MAX_IDENT_LEN){
      error(ERR_IDENTTOOLONG,lineNo,colNo);
      return NULL;
    }
    token->string[count] = (char)currentChar;
    count++;
    readChar();
  }
  token->string[count] = '\0';

  TokenType tokenType = checkKeyword(token->string);

  if(KW_PROGRAM <= tokenType && tokenType <= KW_STRING){
      token->tokenType = tokenType; 
  }
  if(token->tokenType != TK_NONE && token->tokenType != TK_IDENT && token->tokenType != TK_NUMBER && token->tokenType != TK_CHAR && token->tokenType != TK_EOF){
    token->string[0] = '\0';  
  }
  return token;
}

Token* readNumber(void) {
  Token *token = makeToken(TK_NUMBER, lineNo, colNo);

  int count = 0;
  while(charCodes[currentChar] == CHAR_DIGIT){
    token->string[count] = (char)currentChar;
    count++;
    readChar();
  }
  if (charCodes[currentChar] == CHAR_PERIOD){
    lineNum = lineNo; 
    colNum = colNo;
    token->string[count] = (char)currentChar;
    token->tokenType = TK_FLOAT;
    count++;
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_RPAR){
      checkRSEL = 1;
      token->tokenType = TK_NUMBER;
      token->string[count-1] = '\0';
      token->value = atoi(token->string);
      return token;
    }
    while (charCodes[currentChar] == CHAR_DIGIT) {
      token->string[count] = (char)currentChar;
      count++;
      readChar();
    }
    if (charCodes[currentChar] == CHAR_PERIOD) {
      error(ERR_INVALIDSYMBOL,lineNo,colNo);
      return NULL;
    } 
  }

  token->string[count] = '\0';
  token->value = atoi(token->string);
  return token;
}

// Token *readFloat (int count){
//   Token *token = makeToken(TK_FLOAT, lineNo, colNo);
//   while(charCodes[currentChar] == CHAR_DIGIT){
//     token->string[count] = (char)currentChar;
//     count++;
//     readChar();
//   }
//   if (charCodes[currentChar] == CHAR_PERIOD){
//     token->string[count] = (char)currentChar;
//     count++;
//     readChar();
//     while (charCodes[currentChar] == CHAR_DIGIT) {
//       token->string[count] = (char)currentChar;
//       count++;
//       readChar();
//     } 
//   }
//   token->string[count] = '\0';
//   return token;
// }

Token* readConstChar(void) {
  // TODO
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  token->string[0] = currentChar;
  token->string[1] = '\0';
  readChar();

  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE){
    readChar();
    return token;
  }

  else {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
}

Token* getToken(void) {
  Token *token;
  int line, col;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  if(checkRSEL == 1){
    checkRSEL = 0;
    token = makeToken(SB_RSEL, lineNum, colNum);
    readChar();
    return token;
  }

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_DIGIT: return readNumber();
  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); 
    return token;
  case CHAR_MINUS:
    token = makeToken(SB_MINUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_TIMES:
    token = makeToken(SB_TIMES, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SLASH:
    token = makeToken(SB_SLASH, lineNo, colNo);
    readChar();
    return token;
  case CHAR_LT:
    line = lineNo;
    col = colNo;
    readChar();
    if(currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
      readChar();
      return makeToken(SB_LE, line, col);
    }
    return makeToken(SB_LT, line, col);
  case CHAR_GT:
    line = lineNo;
    col = colNo;
    readChar();
    if(currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
      readChar();
      return makeToken(SB_GE, line, col);
    }
    return makeToken(SB_GT, line, col);
  case CHAR_EXCLAIMATION:
    line = lineNo;
    col = colNo;
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
      readChar();
      return makeToken(SB_NEQ, line, col);
    }
    error(ERR_INVALIDSYMBOL, line, col);
    return makeToken(TK_NONE, lineNo, colNo);
  case CHAR_EQ:
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar();
    return token;
  case CHAR_COMMA:
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar();
    return token;
  case CHAR_PERIOD:
    line = lineNo;
    col = colNo;
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_RPAR){
      readChar();
      return makeToken(SB_RSEL, line, col);
    }
    if (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT) {
      token = makeToken(TK_FLOAT, line, col);
      token->string[0] = '0';
      token->string[1] = '.';
      int count = 2;
      while (charCodes[currentChar] == CHAR_DIGIT) {
        token->string[count] = (char)currentChar;
        count++;
        readChar();
      }
      if (charCodes[currentChar] == CHAR_PERIOD) {
        error(ERR_INVALIDSYMBOL,lineNo,colNo);
        return NULL;
      } 
      token->string[count] = '\0';
      return token;
    }
    return makeToken(SB_PERIOD, line, col);
  case CHAR_COLON:
    line = lineNo;
    col = colNo;
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
      readChar();
      return makeToken(SB_ASSIGN, line, col);
    }
    return makeToken(SB_COLON, line, col);
  case CHAR_SEMICOLON:
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SINGLEQUOTE:
    return readConstChar();
  case CHAR_LPAR:
    line = lineNo;
    col = colNo;
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_PERIOD){
      readChar();
      return makeToken(SB_LSEL, line, col);
    }
    if (currentChar != EOF && charCodes[currentChar] == CHAR_TIMES){
      skipComment();
      return getToken();
    }
    return makeToken(SB_LPAR, line, col);
  case CHAR_RPAR:
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar();
    return token;
  case CHAR_DOUBLEQUOTES:
    token = makeToken(TK_STRING, lineNo, colNo);
    token->string[0] = '\'';
    int count = 1;
    readChar();
    while(charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT){
      if(count > MAX_IDENT_LEN){
        error(ERR_IDENTTOOLONG, lineNo, colNo);
        return NULL;
      }
      if(currentChar == EOF){
        error(ERR_INVALIDSYMBOL, lineNo, colNo);
        return NULL;
      }
      token->string[count] = (char) currentChar;
      count++;
      readChar();
    }
    if (charCodes[currentChar] != CHAR_DOUBLEQUOTES){
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
      return NULL;
    }
    else {
      token->string[count] = '\'';
      count++;
    }
    token->string[count] = '\0';
    readChar();
    return token;
  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;
  case TK_FLOAT: printf("TK_FLOAT(%s)\n", token->string); break;
  case TK_STRING: printf("TK_STRING(%s)\n", token->string); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;
  case KW_FLOAT: printf("KW_FLOAT\n"); break;
  case KW_STRING: printf("KW_STRING\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}



