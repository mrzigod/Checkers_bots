#ifndef BOARD
#define BOARD

#include<vector>
#include<string>

using namespace std;

class board
{
    public:
    int spaces[10][10];
    vector <int> white;
    vector <int> black;
    string rows;
    string pieces;
    int moves_since_last_jump;
    bool ongoing;
    board();
    void addWhitePiece(int piece);
    void addBlackPiece(int piece);
    void eraseWhitePiece(int piece);
    void eraseBlackPiece(int piece);
    void move(int piece, int destination);
    void jump(int piece, int destination);
    void promote(int piece);
    vector <pair<int,int> > listOfWhiteMoves();
    vector <pair<int,int> > listOfBlackMoves();
    vector <vector <pair<int,int> > > listOfJumps(int color);
    void initializeStartingBoard(int rows);
    void initializeTestBoard();
    void printBoard();
    void printCoordinates(int coordinates);
    bool isOngoing();
    int action(int playertype, int color);
    int humanAction(int color);
    int mctsAction(int color);
    int minimaxAction(int color);
    int alphaBetaAction(int color);
    void checkForMoves(int color);
    float evaluateBoard(int color);

};


int mctsChoice(int color);

int minimaxChoice(board Board, int evaluating_color, int maxdepth);
int alphaBetaChoice(board Board, int evaluating_color, int maxdepth);

#endif