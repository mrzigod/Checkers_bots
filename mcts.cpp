#include<vector>
#include"mcts.h"
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<fstream>

using namespace std;

class treeNode
{
    public:
    board Board;
    int white_wins;
    int black_wins;
    int simulations;
    int current_color;
    int parent;
    int own_index;
    vector <pair <int,int> > possible_moves;
    vector <vector <pair <int,int> > > possible_jumps;
    vector <int> children_indexes;
    treeNode(){};
    treeNode(board b,int parent_color, int parent_index)
    {
        Board=b;
        white_wins=0;
        black_wins=0;
        simulations=0;
        current_color=1-parent_color;
        parent=parent_index;
        own_index=0;
        possible_jumps=Board.listOfJumps(current_color);
        int amount_of_kids=0;
        if(possible_jumps.size()==0)
        {
            if(current_color==0)
            {
                possible_moves=Board.listOfWhiteMoves();
            }
            else
            {
                possible_moves=Board.listOfBlackMoves();
            }
            amount_of_kids=possible_moves.size();
        }
        else
        {
            amount_of_kids=possible_jumps.size();
        }
        children_indexes=vector<int> (amount_of_kids,-1);
    }
    treeNode move(int index)
    {
        board child_board;
        child_board=Board;
        if(possible_jumps.size()==0)
        {
            child_board.move(possible_moves[index].first,possible_moves[index].second);
        }
        if(possible_jumps.size()>0)
        {
            int isqueen=0;
            if(Board.spaces[possible_jumps[index][0].first/10][possible_jumps[index][0].first%10]%2==0)
                isqueen+=100;
            for(int i=0;i<possible_jumps[index].size();i++)
            {
                child_board.jump(possible_jumps[index][i].first+isqueen,possible_jumps[index][i].second);
            }
        }
        treeNode child=treeNode(child_board,current_color,own_index);
        return child;
    }

    void printAvailableMoves()
    {   
        cout<<"Available Moves:"<<endl;
        for(int i=0;i<possible_moves.size();i++)
        {
            Board.printCoordinates(possible_moves[i].first);
            Board.printCoordinates(possible_moves[i].second);
            cout<<endl;
        }
    }

    void printAvailableJumps()
    {
        cout<<"Available Jumps:"<<endl;
        for(int i=0;i<possible_jumps.size();i++)
        {
            Board.printCoordinates(possible_jumps[i][0].first);
            for(int j=0;j<possible_jumps[i].size();j++)
            {
                Board.printCoordinates(possible_jumps[i][j].second);
            }
            cout<<endl;
        }
    }

    int countValues(bool final);
    float value(treeNode node, bool final);
};

treeNode root;
treeNode current_node;
vector <treeNode> nodes;

int treeNode::countValues(bool final)
{
    int n=children_indexes.size();
    vector<float> results=vector<float>(n);
    for(int i=0;i<n;i++)
    {
        if(children_indexes[i]==-1)
        {
            results[i]=10000;
            if(final)
            results[i]=0;
        }
        else
        {
            results[i]=value(nodes[children_indexes[i]],final);
        }
    }
    int result=0;
    for(int i=0;i<n;i++)
    {
        if(results[i]>results[result])
            result=i;
    }
    return result;
}

float treeNode::value(treeNode node,bool final)
{
    if(node.simulations==0)
        return 1000;
    if(node.children_indexes.size()==0)
    {
        if(final)
        return 10000000;
        return 0;
    }
    if(final)
        return node.simulations; 
    float exploitation,exploration;
    if(current_color==0)
    {
        exploitation=float(node.white_wins)/float(node.simulations);
    }
    if(current_color==1)
    {
        exploitation=float(node.black_wins)/float(node.simulations);
    }
    exploitation+=(float)(node.simulations-node.black_wins-node.white_wins)*0.5/float(node.simulations);
    exploration=1.4*sqrt(log((float)nodes[node.parent].simulations)/(float)node.simulations);
    return exploitation+exploration;
}

int randomInt(int max)
{
    return rand()%max;
}

int randomFinish(board Board, int color)
{
    int current_color=color;
    int iterationcount=0;
    while(Board.ongoing)
    {
        vector <vector <pair <int,int> > > possible_jumps=Board.listOfJumps(current_color);
        if(possible_jumps.size()>0)
        {
            int index=randomInt(possible_jumps.size());
            int isqueen=0;
            if(Board.spaces[possible_jumps[index][0].first/10][possible_jumps[index][0].first%10]%2==0)
                isqueen+=100;
            for(int i=0;i<possible_jumps[index].size();i++)
            {
                Board.jump(possible_jumps[index][i].first+isqueen,possible_jumps[index][i].second);
            }
        }
        if(possible_jumps.size()==0)
        {
            vector <pair <int,int> > possible_moves;
            if(current_color==0)
            {
                possible_moves=Board.listOfWhiteMoves();
            }
            else
            {
                possible_moves=Board.listOfBlackMoves();
            }
            if(possible_moves.size()==0)
            {
                if(current_color==0)
                {
                    return -1;
                }
                if(current_color==1)
                {
                    return 1;
                }
            }
            else
            {
                int index=randomInt(possible_moves.size());
                Board.move(possible_moves[index].first,possible_moves[index].second);
            }
        }
        current_color=1-current_color;
        Board.checkForMoves(current_color);
    }
    if(Board.moves_since_last_jump==40)
    {
        return 0;
    }
    if(current_color==0)
    {
        return -1;
    }
    if(current_color==1)
    {
        return 1;
    }
    return 0;
}


int mctsChoice(int color)
{
    for(int i=0;i<3000;i++) 
    {
        treeNode searchnode=current_node;
        int child=0;
        while(searchnode.children_indexes[child]!=-1&&searchnode.children_indexes.size()>0)
        {
            child=searchnode.countValues(false);
            if(searchnode.children_indexes[child]!=-1)
            searchnode=nodes[searchnode.children_indexes[child]];
        }
        if(searchnode.children_indexes.size()>0)
        {
            treeNode childNode=searchnode.move(child);
            int child_index=nodes.size();
            childNode.own_index=child_index;
            nodes.push_back(childNode);
            nodes[searchnode.own_index].children_indexes[child]=child_index;
            searchnode.children_indexes[child]=child_index;
            int gameResult=randomFinish(childNode.Board,childNode.current_color); //0=remis, 1=white won, -1=black won
            //wroc do gory zwracajac wynik gry i wpisujac go do node'ow
            while(nodes[child_index].parent!=current_node.parent)
            {
                nodes[child_index].simulations++;
                if(gameResult==1)
                {
                    nodes[child_index].white_wins++;
                }
                if(gameResult==-1)
                {
                    nodes[child_index].black_wins++;
                }
                child_index=nodes[child_index].parent;
            }
            nodes[child_index].simulations++;
            current_node.simulations++;
            if(gameResult==1)
            {
                nodes[child_index].white_wins++;
                current_node.white_wins++;
            }
            if(gameResult==-1)
            {
                nodes[child_index].black_wins++;
                current_node.black_wins++;
            }
            current_node=nodes[current_node.own_index];
        }
        if(searchnode.children_indexes.size()==0)
        {
            int child_index=searchnode.own_index;
            while(nodes[child_index].parent!=current_node.parent)
            {
                nodes[child_index].simulations++;
                if(searchnode.current_color==1)
                {
                    nodes[child_index].white_wins++;
                }
                if(searchnode.current_color==0)
                {
                    nodes[child_index].black_wins++;
                }
                child_index=nodes[child_index].parent;
            }
            nodes[child_index].simulations++;
            current_node.simulations++;
            if(searchnode.current_color==1)
            {
                nodes[child_index].white_wins++;
                current_node.white_wins++;
            }
            if(searchnode.current_color==0)
            {
                nodes[child_index].black_wins++;
                current_node.black_wins++;
            }
            current_node=nodes[current_node.own_index];
        }
    }
    //po skonczonej liczbie symulacji policz value ostatni raz i zwroc optymalny node
    
    int final_choice=current_node.countValues(true);
    return final_choice;
}


void readPriors()
{
    fstream input;
    input.open("priors.txt",ios::in);
    int count;
    input>>count;
    int root_parent;
    input>>root_parent;
    input>>current_node.white_wins>>current_node.black_wins>>current_node.simulations;
    for(int i=0;i<current_node.children_indexes.size();i++)
    {
        input>>current_node.children_indexes[i];
    }
    nodes[0]=current_node;
    root=current_node;
    for(int i=1;i<count;i++)
    {
        int parent;
        input>>parent;
        int index=0;
        for(int j=0;j<nodes[parent].children_indexes.size();j++)
        {
            if(nodes[parent].children_indexes[j]==i)
            index=j;
        }
        treeNode node=nodes[parent].move(index);
        input>>node.white_wins;
        input>>node.black_wins;
        input>>node.simulations;
        node.own_index=i;
        for(int j=0;j<node.children_indexes.size();j++)
        {
            input>>node.children_indexes[j];
        }
        nodes.push_back(node);
    }

}