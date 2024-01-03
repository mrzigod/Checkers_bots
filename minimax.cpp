#include<vector>
#include"mcts.h"
#include<algorithm>

using namespace std;

float minimaxChildEval(board Board, int evaluating_color, int current_color, int depth, int maxdepth)
{
    if(Board.moves_since_last_jump>=40)
        return 0.5;
    vector <pair <int,int> > possible_moves;
    vector <vector <pair <int,int> > > possible_jumps;
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

    if(amount_of_kids==0)
    {
        if(evaluating_color==current_color)
            return 0;
        else
            return 1;
    }
    if(depth>=maxdepth)
    {
        return Board.evaluateBoard(evaluating_color);
    }
    float val=-1;
    if(evaluating_color==current_color)
        val=0;
    else
        val=1;
    for(int index=0;index<amount_of_kids;index++)
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
        float child_result=minimaxChildEval(child_board, evaluating_color, 1-current_color,depth+1,maxdepth);
        if(evaluating_color==current_color)
            val=max(val,child_result);
        else
            val=min(val,child_result);
    }
    return val;
}


int minimaxChoice(board Board, int evaluating_color, int maxdepth)
{
    vector <pair <int,int> > possible_moves;
    vector <vector <pair <int,int> > > possible_jumps;
    possible_jumps=Board.listOfJumps(evaluating_color);
    int amount_of_kids=0;
    if(possible_jumps.size()==0)
    {
        if(evaluating_color==0)
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
    if(amount_of_kids==0)
    {
        return 0;
    }
    float val=0;
    int chosen_index=0;
    for(int index=0;index<amount_of_kids;index++)
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
        float child_result=minimaxChildEval(child_board, evaluating_color, 1-evaluating_color,2,maxdepth);
        if(child_result>val){
            chosen_index=index;
            val=child_result;
        }
    }
    return chosen_index;
}



float alphaBetaChildEval(board Board, int evaluating_color, int current_color, int depth, int maxdepth,float alphabetavalue)
{
    if(Board.moves_since_last_jump>=40)
        return 0.5;
    vector <pair <int,int> > possible_moves;
    vector <vector <pair <int,int> > > possible_jumps;
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

    if(amount_of_kids==0)
    {
        if(evaluating_color==current_color)
            return 0;
        else
            return 1;
    }
    if(depth>=maxdepth)
    {
        return Board.evaluateBoard(evaluating_color);
    }
    float val=-1;
    if(evaluating_color==current_color)
        val=0;
    else
        val=1;
    for(int index=0;index<amount_of_kids;index++)
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
        float child_result=alphaBetaChildEval(child_board, evaluating_color, 1-current_color,depth+1,maxdepth,val);
        if(evaluating_color==current_color)
        {
            val=max(val,child_result);
            if(val>alphabetavalue)
            return val;
        }
        else
        {
            val=min(val,child_result);
            if(val<alphabetavalue)
            return val;
        }
        
    }
    return val;
}


int alphaBetaChoice(board Board, int evaluating_color, int maxdepth)
{
    vector <pair <int,int> > possible_moves;
    vector <vector <pair <int,int> > > possible_jumps;
    possible_jumps=Board.listOfJumps(evaluating_color);
    int amount_of_kids=0;
    if(possible_jumps.size()==0)
    {
        if(evaluating_color==0)
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
    if(amount_of_kids==0)
    {
        return 0;
    }
    float val=0;
    int chosen_index=0;
    for(int index=0;index<amount_of_kids;index++)
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
        float child_result=alphaBetaChildEval(child_board, evaluating_color, 1-evaluating_color,2,maxdepth,val);
        if(child_result>val){
            chosen_index=index;
            val=child_result;
        }
    }
    return chosen_index;
}