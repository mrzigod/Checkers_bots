#include<iostream>
#include<vector>
#include<string>
#include"mcts.h"
#include"board.cpp"
#include"mcts.cpp"
#include"minimax.cpp"

using namespace std;

int main(int argc, char*argv[])
{
    board b;
    b.initializeStartingBoard(3);
    srand((unsigned)time(NULL)); 
    //b.initializeTestBoard();
    b.printBoard();
    vector <int> players {1,0}; //0=human player 1=mcts bot 2=minimax bot 3=alphabeta bot
    if(argc>1)
    {
        if(argv[1][0]=='a')
        {
            players[0]=1;
            players[1]=3;
        }
    }

    int currentplayer=0;
    bool using_mcts=true;
    root=treeNode(b,1-currentplayer,-1);
    root.current_color=currentplayer;
    current_node=root;
    nodes.push_back(root);
    readPriors();
    while(b.isOngoing())
    {
        int move=b.action(players[currentplayer],currentplayer);
        currentplayer=1-currentplayer;
        b.printBoard();
        b.checkForMoves(currentplayer);
        if(using_mcts)
        {
            int current_index=current_node.children_indexes[move];
            current_node=current_node.move(move);
            if(current_index==-1)
            {
                current_index=nodes.size();
                current_node.own_index=current_index;
                nodes.push_back(current_node);
            }
            else
                current_node.own_index=current_index;
        }
    }
}