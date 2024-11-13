//
// Author: Abhusha Ghimire
//

#include "lab8part1.h"

#include "math.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

int playerWin(char board[][26], int n, char colour, char otherColour) {
  int counterColour = 0, counterOtherColour = 0, counter = 0;
  bool isBoardFull = true;
  while (isBoardFull && counter < (n * n)) {
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        if (board[row][col] == colour) {
          counterColour++;
        } else if (board[row][col] == otherColour) {
          counterOtherColour++;
        } else {
          return 0;
          isBoardFull = false;
        }
        counter++;
      }
    }
  }
  if (isBoardFull == true) {
    if (counterColour > counterOtherColour) {
      return 2;
    } else if (counterColour == counterOtherColour) {
      return 3;
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}

int checkScore(char board[][26], int row, int col, char colour, int deltaRow,
               int deltaCol) {
  char otherColour;
  int score = 1;
  if (colour == 'W') {
    otherColour = 'B';
  } else {
    otherColour = 'W';
  }
  while (board[row + score * deltaRow][col + score * deltaCol] == otherColour) {
    score++;
  }
  return score;
}

void changeBoard(char board[][26], char colour, char otherColour, int intRow,
                 int intCol, int deltaRow, int deltaCol) {
  board[intRow][intCol] = colour;
  int counter = 1;
  while (board[intRow + counter * deltaRow][intCol + counter * deltaCol] ==
         otherColour) {
    board[intRow + counter * deltaRow][intCol + counter * deltaCol] = colour;
    counter++;
  }
}

bool isValidMoves(char board[][26], int n, char colour, char otherColour,
                  int delta[][8]) {
  int deltaRow = 0, deltaCol = 0;
  bool hasValid = false;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        for (int i = 0; i < 8; i++) {
          deltaRow = delta[0][i];
          deltaCol = delta[1][i];
          if (positionInBounds(n, row + 2 * deltaRow, col + 2 * deltaCol)) {
            if (checkLegalInDirection(board, n, row, col, colour, deltaRow,
                                      deltaCol)) {
              return true;
              hasValid = true;
            }
          }
        }
      }
    }
  }
  if (!hasValid) {
    printf("\n%c player has no valid move.", colour);
  }
  return hasValid;
}

void printBoard(char board[][26], int n) {
  const char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";
  printf("  ");
  for (int i = 0; i < n; i++) {
    printf("%c", alphabet[i]);
  }
  for (int row = 0; row < n; row++) {
    printf("\n%c ", alphabet[row]);
    for (int col = 0; col < n; col++) {
      printf("%c", board[row][col]);
    }
  }
}

bool positionInBounds(int n, int row, int col) {
  if (row < n && col < n && row >= 0 && col >= 0) {
    return true;
  } else {
    return false;
  }
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  char otherCol;
  if (colour == 'W') {
    otherCol = 'B';
  } else {
    otherCol = 'W';
  }
  if (board[row + deltaRow][col + deltaCol] == otherCol) {
    int counter = 1;
    bool keepGoing = true;
    while (keepGoing) {
      if (positionInBounds(n, (row + counter * deltaRow),
                           (col + counter * deltaCol)) == false ||
          board[row + counter * deltaRow][col + counter * deltaCol] == 'U') {
        return false;
        keepGoing = false;
      }
      if (board[row + counter * deltaRow][col + counter * deltaCol] == colour) {
        return true;
        keepGoing = false;
      }
      counter++;
    }
    return false;
  }
  return false;
}

int computerPlay(char board[][26], int n, char colour, char otherColour,
                 int delta[][8]) {
  if (!isValidMoves(board, n, colour, otherColour, delta)) {
    return 0;
  } else {
    int maxScore = 0;
    int tempScore, maxRow, maxCol, winNum;
    char charRow, charCol;
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        if (board[row][col] == 'U') {
          tempScore = 0;
          for (int i = 0; i < 8; i++) {
            int deltaCol = delta[1][i];
            int deltaRow = delta[0][i];
            if (positionInBounds(n, row + 2 * deltaRow, col + 2 * deltaCol)) {
              if (checkLegalInDirection(board, n, row, col, colour, deltaRow,
                                        deltaCol)) {
                tempScore +=
                    checkScore(board, row, col, colour, deltaRow, deltaCol);
                if (tempScore > maxScore) {
                  maxScore = tempScore;
                  maxRow = row;
                  maxCol = col;
                }
              }
            }
          }
        }
      }
    }
    charRow = (char)(maxRow + 97);
    charCol = (char)(maxCol + 97);

    printf("\nComputer places %c at %c%c.\n", colour, charRow, charCol);
    for (int i = 0; i < 8; i++) {
      int deltaCol = delta[0][i];
      int deltaRow = delta[1][i];
      if (positionInBounds(n, maxRow + 2 * deltaRow, maxCol + 2 * deltaCol) &&
          checkLegalInDirection(board, n, maxRow, maxCol, colour, deltaRow,
                                deltaCol)) {
        changeBoard(board, colour, otherColour, maxRow, maxCol, deltaRow,
                    deltaCol);
      }
    }
    printBoard(board, n);
    winNum = playerWin(board, n, otherColour, colour);
    return winNum;
  }
}

int humanPlay(char board[][26], int n, char colour, char otherColour,
              int delta[][8]) {
  char charRow, charCol;
  int intRow, intCol, winNum;
  if (!isValidMoves(board, n, otherColour, colour, delta)) {
    return 0;
  } else {
    printf("\nEnter move for colour %c (RowCol): ", otherColour);
    scanf(" %c %c", &charRow, &charCol);
    intRow = (int)charRow - 97;
    intCol = (int)charCol - 97;
    bool isValid = false;
    if (positionInBounds(n, intRow, intCol) && board[intRow][intCol] == 'U') {
      for (int i = 0; i < 8; i++) {
        int deltaCol = delta[0][i];
        int deltaRow = delta[1][i];
        if (positionInBounds(n, intRow + 2 * deltaRow, intCol + 2 * deltaCol) &&
            checkLegalInDirection(board, n, intRow, intCol, otherColour,
                                  deltaRow, deltaCol)) {
          changeBoard(board, otherColour, colour, intRow, intCol, deltaRow,
                      deltaCol);
          isValid = true;
        }
      }
    }
    if (!isValid) {
      printf("Invalid move.");
      return 1;
    } else {
      printBoard(board, n);
      winNum = playerWin(board, n, otherColour, colour);
      return winNum;
    }
  }
}

int main(void) {
  int n;
  char colour;
  char otherColour = 'W';
  int delta[2][8] = {{-1, 0, 1, -1, 1, -1, 0, 1}, {-1, -1, -1, 0, 0, 1, 1, 1}};
  printf("Enter the board dimension: ");
  scanf("%d", &n);
  printf("Computer plays (B/W): ");
  scanf(" %c", &colour);
  if (colour == 'W') {
    otherColour = 'B';
  }
  char board[26][26];
  for (int row = 0; row <= n; row++) {
    for (int col = 0; col <= n; col++) {
      board[row][col] = 'U';
    }
  }
  board[n / 2 - 1][n / 2 - 1] = 'W';
  board[n / 2 - 1][n / 2] = 'B';
  board[n / 2][n / 2 - 1] = 'B';
  board[n / 2][n / 2] = 'W';

  printBoard(board, n);

  int computerWins = 0;
  int humanWins = 0;

  while (humanWins == 0 && computerWins == 0) {
    if (colour == 'B') {
      computerWins = computerPlay(board, n, colour, otherColour, delta);
      if (computerWins == 1 || humanWins == 2) {
        printf("\n%c player wins.", otherColour);
        return 0;
      }
      if (computerWins == 2 || humanWins == 1) {
        printf("\n%c player wins.", colour);
        return 0;
      }
      if (computerWins == 3 || humanWins == 2) {
        printf("\nDraw!");
        return 0;
      }
      humanWins = humanPlay(board, n, colour, otherColour, delta);
      if (computerWins == 1 || humanWins == 2) {
        printf("\n%c player wins.", otherColour);
        return 0;
      }
      if (computerWins == 2 || humanWins == 1) {
        printf("\n%c player wins.", colour);
        return 0;
      }
      if (computerWins == 3 || humanWins == 2) {
        printf("\nDraw!");
        return 0;
      }
    } else {
      humanWins = humanPlay(board, n, colour, otherColour, delta);
      if (computerWins == 1 || humanWins == 2) {
        printf("\n%c player wins.", otherColour);
        return 0;
      }
      if (computerWins == 2 || humanWins == 1) {
        printf("\n%c player wins.", colour);
        return 0;
      }
      if (computerWins == 3 || humanWins == 2) {
        printf("\nDraw!");
        return 0;
      }
      computerWins = computerPlay(board, n, colour, otherColour, delta);
      if (computerWins == 1 || humanWins == 2) {
        printf("\n%c player wins.", otherColour);
        return 0;
      }
      if (computerWins == 2 || humanWins == 1) {
        printf("\n%c player wins.", colour);
        return 0;
      }
      if (computerWins == 3 || humanWins == 2) {
        printf("\nDraw!");
        return 0;
      }
    }
  }
}