#include "tictactoe.h"


/*
 * ============================================================================
 * Constructor
 * ============================================================================
 */
TicTacToe::TicTacToe(int boardSize, int agent1, int agent2) {
   srand(time(NULL));

   this->agent1 = agent1;
   this->agent2 = agent2;
   this->N = sqrt(boardSize);
   this->boardSize = N*N;
   this->board = (char*)malloc(sizeof(char) * this->boardSize);
   memset(this->board, ' ', this->boardSize);

   // Set up Zobrist's table for hashing
   this->zTable = (int**)malloc(sizeof(int*) * this->boardSize);
   for(int i = 0; i < this->boardSize; i++) {
      this->zTable[i] = (int*)malloc(sizeof(int)*2);
      this->zTable[i][0] = rand();
      this->zTable[i][1] = rand();
   }
}

/*
 * ============================================================================
 * Destructor
 * ============================================================================
 */
TicTacToe::~TicTacToe() {
   free(board);
   for(int i = 0; i < boardSize; i++) {
      free(zTable[i]);
   }
   free(zTable);
}

/*
 * ============================================================================
 * Print game board
 * ============================================================================
 */
void TicTacToe::printBoard() {
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

/*
 * ============================================================================
 * Print board positions
 * ============================================================================
 */
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

/*
 * ============================================================================
 * Reset board and hash map
 * ============================================================================
 */
void TicTacToe::reset() {
   for(int i = 0; i < boardSize; i++) {
      board[i] = ' ';
   }
   hmap.clear();
}

/*
 * ============================================================================
 * Test if game has end
 *    false - contains valid positions
 *    true  - does not contain valid positions
 * ============================================================================
 */
bool TicTacToe::hasGameEnded() {
   for(int i = 0; i < boardSize; i++) {
      if(board[i] == ' ') {
         return false;
      }
   }
   return true;
}

/*
 * ============================================================================
 * Check if agent has won game
 * ============================================================================
 */
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

/*
 * ============================================================================
 * Get board configuration score
 * ============================================================================
 */
int TicTacToe::getScore(int depth, int agent) {
   if(hasAgentWon('O')) {
      return depth - 1000;
   }
   if(hasAgentWon('X')) {
      return 1000 - depth;
   }
   return 0;
}


/*
 * ============================================================================
 * Check if end game conditions are met
 * ============================================================================
 */
bool TicTacToe::isTerminalState() {
   return hasAgentWon('X') || hasAgentWon('O') || hasGameEnded(); 
}

/*
 * ============================================================================
 * Heuristic Minimax algorithm with alpha-beta pruning and transposition table
 *    - Heuristically stops at depth of 5
 *    - Deterministic alpha-beta pruning of sub-configurations
 *    - Deterministic pruning of same board configurations
 * ============================================================================
 */
int TicTacToe::alphabeta(int depth, int alpha, int beta, int agent){

   // heurestic pruning
   if(depth == 5 || isTerminalState()) {
      return getScore(depth, agent);
   }

   if(agent == MAX_AGENT) {
      int bestVal = INT_MIN;
      for(int i = 0; i < boardSize; i++) {
         if(board[i] == ' ') {
            board[i] = 'X';

            unsigned long hash = getHash(agent == 'X' ? 0 : 1);
            unordered_map<unsigned long, int>::iterator it = hmap.find(hash);

            if(it == hmap.end()) {
               bestVal = max(bestVal, alphabeta(depth+1, alpha, beta, MIN_AGENT));
            }
            else {
               bestVal = max(bestVal, hmap[hash]); // configuration pruning
            }

            alpha   = max(bestVal, alpha);
            board[i] = ' ';
           
            // deterministic pruning
            if(alpha >= beta) {
               break;
            }
         } // end-if
      } // end-for
      return bestVal;
   }
   else {
      int bestVal = INT_MAX;
      for(int i = 0; i < boardSize; i++) {
         if(board[i] == ' ') {
            board[i] = 'O';

            unsigned long hash = getHash(agent == 'X' ? 0 : 1);
            unordered_map<unsigned long, int>::iterator it = hmap.find(hash);

            if(it == hmap.end()) {
               bestVal = min(bestVal, alphabeta(depth+1, alpha, beta, MAX_AGENT));
            }
            else {
               bestVal = min(bestVal, hmap[hash]); // configuration pruning
            }

            beta     = min(bestVal, beta);
            board[i] = ' ';

            // deterministic pruning
            if(alpha >= beta) {
               break;
            }
         } // end-if
      } // end-for
      return bestVal;  
   }
}


/*
 * ============================================================================
 * Apply Zobrist hashing function
 * ============================================================================
 */
unsigned long TicTacToe::getHash(int agent) {
   unsigned long hash = 0;
   for(int i = 0; i < boardSize; i++) {
      if(board[i] != ' ') {
         hash ^= zTable[i][agent];
      }
   }
   return hash;
}

/*
 * ============================================================================
 * Return best value for a given board configuration
 *    For each valid position, compute minimax algorithm
 * ============================================================================
 */
int TicTacToe::computeBestMove(char agent) {
   int bestVal = agent == 'X' ? INT_MIN : INT_MAX;
   int bestPos =  -1;
   int alpha = INT_MIN;
   int beta  = INT_MAX;

   for(int i = 0; i < boardSize; i++) {
      if(board[i] == ' ') {
         board[i] = agent;

         unsigned long hash = getHash(agent == 'X' ? 0 : 1);
         unordered_map<unsigned long, int>::iterator it = hmap.find(hash);

         int moveRes;
         if(it == hmap.end()) {
            moveRes = alphabeta(0, alpha, beta, agent == 'X' ? MIN_AGENT : MAX_AGENT);
         }
         else {
            moveRes = hmap[hash];
         }


         board[i] = ' ';

         if(agent == 'X' && moveRes > bestVal) {
            bestPos = i;
            bestVal = moveRes;
            if(it == hmap.end()) {
               hmap[hash] = bestVal;
            }
         }
         if(agent == 'O' && moveRes < bestVal) {
            bestPos = i;
            bestVal = moveRes;
            if(it == hmap.end()) {
               hmap[hash] = bestVal;
            }
         }
      }
   }
   return bestPos;
} 

/*
 * ============================================================================
 * Return the outcome of the game
 *    1 - Player 1 wins
 *    2 - Player 2 wins
 *    0 - Tied
 * ============================================================================
 */
int TicTacToe::gameResult() {
   if(hasAgentWon('X')) {
      return 1;
   }
   if(hasAgentWon('O')) {
      return 2;
   }
   return 0;
}

/*
 * ============================================================================
 * Computer vs computer game mode
 * ============================================================================
 */
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

/*
 * ============================================================================
 * Human vs human game mode
 * ============================================================================
 */
int TicTacToe::humanVsHuman() {
   char agent = 'X';

   while(!isTerminalState()) {
      printBoard();   
      humanMove(agent);
      agent = agent == 'X' ? 'O' : 'X';
   }

   return gameResult();
}

/*
 * ============================================================================
 * Human vs computer game mode
 * ============================================================================
 */
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

/*
 * ============================================================================
 * Computer Move
 *    Apply minimax algorithm and select the best position on the board   
 * ============================================================================
 */
void TicTacToe::computerMove(char agent) {
   int position = computeBestMove(agent);
   board[position] = agent;
   printf("%c's turn. Selected position: %d\n\n", agent, position);
}

/*
 * ============================================================================
 * Human Move
 *    Selete a valid position on the board and set it to X/O accordingly   
 * ============================================================================
 */
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

/*
 * ============================================================================
 * Game Types
 *   Selete between human vs human, human vs computer, and compute vs computer
 * ============================================================================
 */
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

