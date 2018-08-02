#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stdio.h>      // printf()
#include <stdlib.h>     // rand()
#include <time.h>       // time()
#include <math.h>       // sqrt()
#include <string.h>     // strcmp()
#include <limits.h>     // INT_MAX, INT_MIN
#include <unordered_map>    

using namespace std;

#define HUMAN    111
#define COMPUTER 222

#define MAX_AGENT 11
#define MIN_AGENT 22

class TicTacToe {

private:
   int **zTable;
   char *board;
   int boardSize;
   int N;
   int agent1, agent2;
   
   unordered_map<unsigned long, int> hmap;

   unsigned long getHash(int agent);
   int getScore(int depth, int agent);
   int gameResult();

   int computerVsComputer();
   int humanVsHuman();
   int humanVsComputer();
   
   void humanMove(char agent);
   void computerMove(char agent);
   
   int alphabeta(int depth, int a, int b, int agent);
   int computeBestMove(char agent);

   bool isTerminalState();
   bool hasGameEnded();
   bool hasAgentWon(char agent);

   
public:

   TicTacToe(int boardSize, int agent1, int agent2);
   ~TicTacToe();

   void printBoard();
   void printBoardPositions();
   void reset();
  
   int play();

};



#endif
