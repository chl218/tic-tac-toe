//------------------------------------------------------------------------------
// Test Bench
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tictactoe.h"

#define RUN_REGRESSION 0
//------------------------------------------------------------------------------
// Get user input board size
//------------------------------------------------------------------------------
int getBoardSize(char *str) {
   if(strcmp(str, "3x3") == 0) {
      return 9;
   }
   if(strcmp(str, "4x4") == 0) {
      return 16;
   }
   if(strcmp(str, "5x5") == 0) {
      return 25;
   }
   return -1;
}

//------------------------------------------------------------------------------
// Get Player/Computer agent
//------------------------------------------------------------------------------
int getAgent(char * str) {
   if(strcmp(str, "human") == 0) {
      return HUMAN;
   }
   if(strcmp(str, "computer") == 0) {
      return COMPUTER;
   }
   return -1;
}


int main(int argc, char* argv[]) {

   //---------------------------------------------------------------------------
   // Parse program inputs (Set Up)
   //---------------------------------------------------------------------------
   if(argc < 4) {
      printf("[ERROR]: Usage: ./prog <3x3|4x4|5x5>");
      printf(" <human|computer> <human|computer>\n");
      return 0;
   }

   int boardSize = getBoardSize(argv[1]);
   if(boardSize < 0) {
      printf("[ERROR]: Invalid board size (NxN: 3x3, 4x4, 5x5).\n");
      return 0;
   }

   int agent1 = getAgent(argv[2]);
   int agent2 = getAgent(argv[3]);
   if(agent1 != COMPUTER && agent1 != HUMAN) {
      printf("[ERROR]: Invalid agent (Agent: human or computer).\n");
      return 0;
   }
   if(agent2 != COMPUTER && agent2 != HUMAN) {
      printf("[ERROR]: Invalid agent (Agent: human or computer).\n");
      return 0;  
   }


   //---------------------------------------------------------------------------
   // Regression Tests
   //    Prints out the winner e.g. (1, 2, 0)
   //    Expected: all 0s (tie)
   //---------------------------------------------------------------------------
   if(RUN_REGRESSION) {
      TicTacToe game3x3(9, COMPUTER, COMPUTER);
      printf("\n3x3 Board Simulation:\n");
      for(int i = 0; i < 32; i++) {
         int res = game3x3.play();
         printf("%d", res);
         game3x3.reset();
      }

      TicTacToe game4x4(16, COMPUTER, COMPUTER);
      printf("\n4x4 Board Simulation:\n");
      for(int i = 0; i < 32; i++) {
         int res = game4x4.play();
         game4x4.reset();
         printf("%d", res);
      }

      TicTacToe game5x5(25, COMPUTER, COMPUTER);
      printf("\n5x5 Board Simulation:\n");
      for(int i = 0; i < 32; i++) {
         int res = game5x5.play();
         game5x5.reset();
         printf("%d", res);
      }
      printf("\n");
   }



   //---------------------------------------------------------------------------
   // Test Bench
   //---------------------------------------------------------------------------
   TicTacToe game(boardSize, agent1, agent2);
   printf("Board Positions:\n");
   game.printBoardPositions();
   printf("\n");
   
   int res = game.play();
   game.printBoard();
   switch(res) {
      case 1: printf("Player 1 Wins!\n"); break;
      case 2: printf("Player 2 Wins!\n"); break;
      case 0: printf("Tie!\n"); break;
   }
   
   return 0;
}