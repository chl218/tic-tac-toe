#include "tictactoe.h"



TicTacToe::TicTacToe(int boardSize, int agent1, int agent2) {
   srand(time(NULL));

   this->agent1 = agent1;
   this->agent2 = agent2;
   this->N = sqrt(boardSize);
   this->boardSize = N*N;
   this->board = (char*)malloc(sizeof(char) * this->boardSize);
   memset(this->board, ' ', this->boardSize);
}

TicTacToe::~TicTacToe() {

   free(board);
}

void TicTacToe::printBoard() {
   // printf("Board\n");
   for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
         if(board[i*N+j] == ' ') {
            printf("   ");
         }
         else {
            printf(" %c ", board[i*N+j]);
         }
         if(j < N-1) {
            printf("|");
         }
      } 

      printf("\n");
      if(i < N-1) {  
         for(int j = 0; j < N; j++) {
            printf("---");
            if(j < N-1) {
               printf("+");
            }
         }
         printf("\n");  
      }
   }
}

void TicTacToe::printBoardPositions() {

   int width = 0;
   int digits = boardSize;
   while(digits > 0) {
      digits /= 10;
      width++;
   }

   int pos = 1;

   for(int i = 0; i < N; i++) {
      for(int j = 0; j < N; j++) {
         printf(" %*d ", width, pos++);
         if(j < N-1) {
            printf("|");
         }
      } 

      printf("\n");
      if(i < N-1) {  
         for(int j = 0; j < N; j++) {
            for(int k = 0; k < width+2; k++) {
               printf("-");
            }
            if(j < N-1) {
               printf("+");
            }
         }
         printf("\n");  
      }
   }
}

void TicTacToe::reset() {
   for(int i = 0; i < boardSize; i++) {
      board[i] = ' ';
   }
}

bool TicTacToe::hasGameEnded() {
   for(int i = 0; i < boardSize; i++) {
      if(board[i] == ' ') {
         return false;
      }
   }
   return true;
}

bool TicTacToe::hasAgentWon(char agent) {

   bool done = true;

   // check rows
   for(int i = 0; i < N; i++) {
      done = true;
      for(int j = 0; j < N; j++) {
         done &= board[i*N+j] == agent;
      }
      if(done) {
         return true;
      }
   }

   // check columns
   for(int j = 0; j < N; j++) {
      done = true;
      for(int i = 0; i < N; i++) {
         done &= board[i*N+j] == agent;
      }
      if(done) {
         return true;
      }
   }

   // check diagonals
   done = true;
   for(int diag = 0; diag < boardSize; diag += N+1) {
      done &= board[diag] == agent;
   }
   if(done) {
      return true;
   }

   done = true;
   for(int diag = N-1; diag < boardSize-1; diag += N-1) {
      done &= board[diag] == agent;
   }
   if(done) {
      return true;
   }

   return false;
}

int TicTacToe::getScore(int depth, int agent) {
   if(hasAgentWon('O')) {
      return depth - 1000;
   }
   if(hasAgentWon('X')) {
      return 1000 - depth;
   }
   return 0;
}

/**
int TicTacToe::minimax(int depth, int agent) {
   printf("%d \n", depth);
   int score = getScore(depth, agent);

   if(score != 0) {
      return score;
   }  

   if(!containsMoreMoves()) {
      return 0;
   }

   int maxAgentVal = INT_MIN;
   int minAgentVal = INT_MAX;
   for(int i = 0; i < boardSize; i++) {
      if(board[i] == ' ') {
         if(agent == MAX_AGENT) {
            board[i] = 'X';
            // getchar();
            maxAgentVal = max(maxAgentVal, minimax(depth+1, MIN_AGENT));
            board[i] = ' ';
         }
         else {
            board[i] = 'O';
            // getchar();
            minAgentVal = min(minAgentVal, minimax(depth+1, MAX_AGENT));
            board[i] = ' ';
         }
      }
   }

   return agent == MAX_AGENT ? maxAgentVal : minAgentVal;
}
**/

bool TicTacToe::isTerminalState() {
   return hasAgentWon('X') || hasAgentWon('O') || hasGameEnded(); 
}

int TicTacToe::alphabeta(int depth, int alpha, int beta, int agent){
   // printBoard();
   // getchar();

   if(depth == 5 || isTerminalState()) {
      return getScore(depth, agent);
   }


   if(agent == MAX_AGENT) {
      int bestVal = INT_MIN;
      for(int i = 0; i < boardSize; i++) {
         if(board[i] == ' ') {
            board[i] = 'X';
            bestVal = max(bestVal, alphabeta(depth+1, alpha, beta, MIN_AGENT));
            alpha   = max(bestVal, alpha);
            board[i] = ' ';
           
            if(alpha >= beta) {
               break;
            }
         }
      }
      return bestVal;
   }
   else {
      int bestVal = INT_MAX;
      for(int i = 0; i < boardSize; i++) {
         if(board[i] == ' ') {
            
            board[i] = 'O';
            bestVal = min(bestVal, alphabeta(depth+1, alpha, beta, MAX_AGENT));
            beta    = min(bestVal, beta);
            board[i] = ' ';


            if(alpha >= beta) {
               break;
            }

         }
      }
      return bestVal;  
   }
}

int TicTacToe::computeBestMove(char agent) {
   int bestVal = agent == 'X' ? INT_MIN : INT_MAX;
   int bestPos =  -1;
   int alpha = INT_MIN;
   int beta  = INT_MAX;

   for(int i = 0; i < boardSize; i++) {
      if(board[i] == ' ') {
         board[i] = agent;
         int moveRes = alphabeta(0, alpha, beta, agent == 'X' ? MIN_AGENT : MAX_AGENT);
         board[i] = ' ';

         if(agent == 'X' && moveRes > bestVal) {
            bestPos = i;
            bestVal = moveRes;
         }
         if(agent == 'O' && moveRes < bestVal) {
            bestPos = i;
            bestVal = moveRes;
         }
      }
   }
   return bestPos;
} 

int TicTacToe::gameResult() {
   if(hasAgentWon('X')) {
      return 1;
   }
   if(hasAgentWon('O')) {
      return 2;
   }
   return 0;
}

int TicTacToe::computerVsComputer() {
   char agent = 'X';

   int firstMove = rand() % boardSize + 1;
   board[firstMove] = agent;

   while(!isTerminalState()) {
      agent = agent == 'X' ? 'O' : 'X';
      int position = computeBestMove(agent);
      board[position] = agent;
   }

   return gameResult();
}

int TicTacToe::humanVsHuman() {
   char agent = 'X';

   while(!isTerminalState()) {
      printBoard();   
      humanMove(agent);
      agent = agent == 'X' ? 'O' : 'X';
   }

   return gameResult();
}

int TicTacToe::humanVsComputer() {
   char agent = 'X';

   while(!isTerminalState()) {
      printBoard();
      if(agent1 == HUMAN) {
         switch(agent) {
            case 'X': humanMove(agent);    break;
            case 'O': computerMove(agent); break;
         }
      }
      else {
         switch(agent) {
            case 'X': computerMove(agent); break;
            case 'O': humanMove(agent);    break;
         }
      }
      agent = agent == 'X' ? 'O' : 'X';
   }

   return gameResult();
}

void TicTacToe::computerMove(char agent) {
   int position = computeBestMove(agent);
   board[position] = agent;
   printf("%c's turn. Selected position: %d\n\n", agent, position);
}

void TicTacToe::humanMove(char agent) {
   int pos = -1;
   do {
      char input[80];
      printf("%c's turn. Select a position (1-%d): ", agent, boardSize);
      scanf("%36s", input);

      pos = atoi(input);

   } while(!(pos >= 1 && pos <= boardSize && board[pos-1] == ' '));
   printf("\n");

   board[pos-1] = agent;
}

int TicTacToe::play() {

   if(agent1 == COMPUTER && agent2 == COMPUTER) {
      return computerVsComputer();
   }
   else if(agent1 == HUMAN && agent2 == HUMAN) {
      return humanVsHuman();
   }
   else {
      return humanVsComputer();
   }
}

