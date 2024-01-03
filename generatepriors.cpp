#include<iostream>
#include<vector>
#include<string>
#include"mcts.h"
#include"board.cpp"
#include"mcts.cpp"
#include"minimax.cpp"
#include<fstream>

using namespace std;

int main()
{
    board b;
    b.initializeStartingBoard(3);
    srand((unsigned)time(NULL)); 
    //b.initializeTestBoard();
    b.printBoard();
    int currentplayer=0;
    root=treeNode(b,1-currentplayer,-1);
    root.current_color=currentplayer;
    current_node=root;
    nodes.push_back(root);
    for(int i=0;i<15;i++)
    {
        mctsChoice(root.current_color);
    }
    fstream results;
    results.open("priors.txt",ios::out);
    results<<nodes.size()<<endl;
    for(int i=0;i<nodes.size();i++)
    {
        treeNode node=nodes[i];
        results<<node.parent<<endl;
        results<<node.white_wins<<" "<<node.black_wins<<" "<<node.simulations<<endl;
        for(int i=0;i<node.children_indexes.size();i++)
            results<<node.children_indexes[i]<<" ";
        results<<endl;
    }
    results.close();
}