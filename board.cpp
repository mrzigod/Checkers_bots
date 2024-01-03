#include<iostream>
#include<vector>
#include<string>
#include"mcts.h"

using namespace std;

board::board()
{
    for(int i=0;i<10;i++)
    {
        spaces[0][i]=-1;
        spaces[9][i]=-1;
    }
    for(int i=1;i<9;i++)
    {
        spaces[i][0]=-1;
        for(int j=1;j<9;j++)
        {
            spaces[i][j]=0;
        }
        spaces[i][9]=-1;
    }
    rows=" abcdefgh ";
    pieces="_oOxX";
    moves_since_last_jump=0;
    ongoing=true;
}


void board::addWhitePiece(int piece)
{
    white.push_back(piece);
    spaces[(piece%100)/10][piece%10]=1+piece/100;
}

void board::addBlackPiece(int piece)
{
    black.push_back(piece);
    spaces[(piece%100)/10][piece%10]=3+piece/100;
}

void board::eraseWhitePiece(int piece)
{
    int idx=-1;
    for(int i=0;i<white.size();i++)
    {
        if(white[i]==piece)
            idx=i;
    }
    if(idx!=-1)
    {
        white.erase(white.begin()+idx);
        piece=piece%100;
        spaces[piece/10][piece%10]=0;
    }
}

void board::eraseBlackPiece(int piece)
{
    int idx=-1;
    for(int i=0;i<black.size();i++)
    {
        if(black[i]==piece)
            idx=i;
    }
    if(idx!=-1)
    {
        black.erase(black.begin()+idx);
        piece=piece%100;
        spaces[piece/10][piece%10]=0;
    }
}

void board::move(int piece, int destination)
{
    int coordinates=piece%100;
    if(piece>100)
        destination+=100;
    if(spaces[coordinates/10][coordinates%10]>2)
    {
        eraseBlackPiece(piece);
        addBlackPiece(destination);
        if(destination/10==1)
            promote(destination);
    }
    else
    {
        eraseWhitePiece(piece);
        addWhitePiece(destination);
        if(destination/10==8)
        {
            promote(destination);
        }
    }
    moves_since_last_jump++;
}

void board::jump(int piece, int destination)
{
    int coordinates=piece%100;
    int takencoordinates=(coordinates+destination)/2;
    if(piece>100)
        destination+=100;
    if(spaces[takencoordinates/10][takencoordinates%10]%2==0)
        takencoordinates+=100;
    if(spaces[coordinates/10][coordinates%10]>2)
    {
        eraseBlackPiece(piece);
        eraseWhitePiece(takencoordinates);
        addBlackPiece(destination);
        if(destination/10==1)
            promote(destination);
    }
    else
    {
        eraseWhitePiece(piece);
        eraseBlackPiece(takencoordinates);
        addWhitePiece(destination);
        if(destination/10==8)
            promote(destination);
    }
    if(white.size()==0||black.size()==0)
        ongoing=false;
    moves_since_last_jump=0;
}

void board::promote(int piece)
{
    if(spaces[piece/10][piece%10]>2)
    {
        eraseBlackPiece(piece);
        addBlackPiece(piece+100);
    }
    else
    {
        eraseWhitePiece(piece);
        addWhitePiece(piece+100);
    }
}

vector <pair<int,int> > board::listOfWhiteMoves()
{
    vector <pair <int,int> > moves;
    for(int i=0;i<white.size();i++)
    {
        int piece=white[i];
        int coordinates=piece%100;
        if(spaces[coordinates/10+1][coordinates%10+1]==0)
        {
            pair<int,int> a{piece,coordinates+11};
            moves.push_back(a);
        }
        if(spaces[coordinates/10+1][coordinates%10-1]==0)
        {
            pair<int,int> a{piece,coordinates+9};
            moves.push_back(a);
        }
        if(piece>100)
        {
            if(spaces[coordinates/10-1][coordinates%10+1]==0)
            {
                pair<int,int> a{piece,coordinates-9};
                moves.push_back(a);
            }
            if(spaces[coordinates/10-1][coordinates%10-1]==0)
            {
                pair<int,int> a{piece,coordinates-11};
                moves.push_back(a);
            }
        }
    }
    return moves;
}

vector <pair<int,int> > board::listOfBlackMoves()
{
    vector <pair <int,int> > moves;
    for(int i=0;i<black.size();i++)
    {
        int piece=black[i];
        int coordinates=piece%100;
        if(spaces[coordinates/10-1][coordinates%10+1]==0)
        {
            pair<int,int> a{piece,coordinates-9};
            moves.push_back(a);
        }
        if(spaces[coordinates/10-1][coordinates%10-1]==0)
        {
            pair<int,int> a{piece,coordinates-11};
            moves.push_back(a);
        }
        if(piece>100)
        {
            if(spaces[coordinates/10+1][coordinates%10+1]==0)
            {
                pair<int,int> a{piece,coordinates+11};
                moves.push_back(a);
            }
            if(spaces[coordinates/10+1][coordinates%10-1]==0)
            {
                pair<int,int> a{piece,coordinates+9};
                moves.push_back(a);
            }
        }
    }
    return moves;
}

vector <vector <pair<int,int> > > board::listOfJumps(int color)
{
    vector <vector <pair <int,int> > > moves;
    int pieces_to_check=0;
    if(color==0)
    {
        pieces_to_check=white.size();
    }
    if(color==1)
    {
        pieces_to_check=black.size();
    }

    for(int i=0;i<pieces_to_check;i++)
    {
        int piece=0;
        if(color==0)
            piece=white[i];
        if(color==1)
            piece=black[i];
        int coordinates=piece%100;
        vector <vector <pair<int,int> > >sequences;
        if(color==0||piece>100)
        {
            if(spaces[coordinates/10+1][coordinates%10+1]==3-2*color||spaces[coordinates/10+1][coordinates%10+1]==4-2*color)
            {
                if(spaces[coordinates/10+2][coordinates%10+2]==0)
                {
                    vector <pair <int,int> > aa;
                    pair<int,int> a{coordinates,coordinates+22};
                    aa.push_back(a);
                    sequences.push_back(aa);
                }
            }
            if(spaces[coordinates/10+1][coordinates%10-1]==3-2*color||spaces[coordinates/10+1][coordinates%10-1]==4-2*color)
            {
                if(spaces[coordinates/10+2][coordinates%10-2]==0)
                {
                    vector <pair <int,int> > aa;
                    pair<int,int> a{coordinates,coordinates+18};
                    aa.push_back(a);
                    sequences.push_back(aa);
                }
            }
        }
        if(color==1||piece>100)
        {
            if(spaces[coordinates/10-1][coordinates%10+1]==3-2*color||spaces[coordinates/10-1][coordinates%10+1]==4-2*color)
            {
                if(spaces[coordinates/10-2][coordinates%10+2]==0)
                {
                    vector <pair <int,int> > aa;
                    pair<int,int> a{coordinates,coordinates-18};
                    aa.push_back(a);
                    sequences.push_back(aa);
                }
            }
            if(spaces[coordinates/10-1][coordinates%10-1]==3-2*color||spaces[coordinates/10-1][coordinates%10-1]==4-2*color)
            {
                if(spaces[coordinates/10-2][coordinates%10-2]==0)
                {
                    vector <pair <int,int> > aa;
                    pair<int,int> a{coordinates,coordinates-22};
                    aa.push_back(a);
                    sequences.push_back(aa);
                }
            }
        }
        while(sequences.size()>0)
        {
            vector <pair <int,int> > current_sequence=sequences.back();
            sequences.pop_back();
            bool extended=false;
            vector <int> removed;
            for(int i=0;i<current_sequence.size();i++)
            {
                removed.push_back((current_sequence[i].first+current_sequence[i].second)/2);
            }
            int current_coordinates=current_sequence.back().second;
            if(color==0||piece>100)
            {
                if(spaces[current_coordinates/10+1][current_coordinates%10+1]==3-2*color||spaces[current_coordinates/10+1][current_coordinates%10+1]==4-2*color)
                {
                    bool alreadytaken=false;
                    for(int i=0;i<removed.size();i++)
                    {
                        if(current_coordinates+11==removed[i])
                        alreadytaken=true;
                    }
                    if(spaces[current_coordinates/10+2][current_coordinates%10+2]==0&&!alreadytaken)
                    {
                        extended=true;
                        vector <pair <int,int> > new_sequence=current_sequence;
                        pair<int,int> a{current_coordinates,current_coordinates+22};
                        new_sequence.push_back(a);
                        sequences.push_back(new_sequence);
                    }
                }
                if(spaces[current_coordinates/10+1][current_coordinates%10-1]==3-2*color||spaces[current_coordinates/10+1][current_coordinates%10-1]==4-2*color)
                {
                    bool alreadytaken=false;
                    for(int i=0;i<removed.size();i++)
                    {
                        if(current_coordinates+9==removed[i])
                        alreadytaken=true;
                    }
                    if(spaces[current_coordinates/10+2][current_coordinates%10-2]==0&&!alreadytaken)
                    {
                        extended=true;
                        vector <pair <int,int> > new_sequence=current_sequence;
                        pair<int,int> a{current_coordinates,current_coordinates+18};
                        new_sequence.push_back(a);
                        sequences.push_back(new_sequence);
                    }
                }
            }
            if(color==1||piece>100)
            {
                if(spaces[current_coordinates/10-1][current_coordinates%10+1]==3-2*color||spaces[current_coordinates/10-1][current_coordinates%10+1]==4-2*color)
                {
                    bool alreadytaken=false;
                    for(int i=0;i<removed.size();i++)
                    {
                        if(current_coordinates-9==removed[i])
                        alreadytaken=true;
                    }
                    if(spaces[current_coordinates/10-2][current_coordinates%10+2]==0&&!alreadytaken)
                    {
                        extended=true;
                        vector <pair <int,int> > new_sequence=current_sequence;
                        pair<int,int> a{current_coordinates,current_coordinates-18};
                        new_sequence.push_back(a);
                        sequences.push_back(new_sequence);
                    }
                }
                if(spaces[current_coordinates/10-1][current_coordinates%10-1]==3-2*color||spaces[current_coordinates/10-1][current_coordinates%10-1]==4-2*color)
                {
                    bool alreadytaken=false;
                    for(int i=0;i<removed.size();i++)
                    {
                        if(current_coordinates-11==removed[i])
                        alreadytaken=true;
                    }
                    if(spaces[current_coordinates/10-2][current_coordinates%10-2]==0&&!alreadytaken)
                    {
                        extended=true;
                        vector <pair <int,int> > new_sequence=current_sequence;
                        pair<int,int> a{current_coordinates,current_coordinates-22};
                        new_sequence.push_back(a);
                        sequences.push_back(new_sequence);
                    }
                }
            }
            if(!extended)
            {
                moves.push_back(current_sequence);
            }
        }
    }
    return moves;
}

void board::initializeStartingBoard(int rows)
{
    if(rows>=1)
    {
        for(int i=0;i<8;i+=2)
        {
            addWhitePiece(11+i);
            addBlackPiece(82+i);
        }
    }
    if(rows>=2)
    {
        for(int i=0;i<8;i+=2)
        {
            addWhitePiece(22+i);
            addBlackPiece(71+i);
        }
    }
    if(rows>=3)
    {
        for(int i=0;i<8;i+=2)
        {
            addWhitePiece(31+i);
            addBlackPiece(62+i);
        }
    }
}

void board::initializeTestBoard()
{
    addWhitePiece(182);
    addWhitePiece(162);
    addBlackPiece(151);
    addBlackPiece(144);
    addBlackPiece(164);

}

void board::printBoard()
{
    for(int i=8;i>=1;i--)
    {
        cout<<rows[i]<<" ";
        for(int j=1;j<9;j++)
        {
            if((i+j)%2==0)
                cout<<pieces[spaces[i][j]]<<" ";
            else
                cout<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void board::printCoordinates(int coordinates)
{
    cout<<rows[(coordinates%100)/10]<<coordinates%10<<" ";
}

bool board::isOngoing()
{
    return ongoing;
}


int board::action(int playertype, int color)
{
    int move;
    if(playertype==0)
    {
        move=humanAction(color);
    }
    if(playertype==1)
    {
        move=mctsAction(color);
    }
    if(playertype==2)
    {
        move=minimaxAction(color);
    }
    if(playertype==3)
    {
        move=alphaBetaAction(color);
    }
    checkForMoves(color);
    return move;
}

int board::humanAction(int color) //0=white, 1=black
{
    vector <vector <pair<int,int> > > available_jumps=listOfJumps(color);
    if(available_jumps.size()==0)
    {
        vector <pair <int,int> > available_moves;
        if(color==0)
        {
            available_moves=listOfWhiteMoves();
        }
        if(color==1)
        {
            available_moves=listOfBlackMoves();
        }
        cout<<"Available Moves:"<<endl;
        for(int i=0;i<available_moves.size();i++)
        {
            cout<<i+1<<": ";
            printCoordinates(available_moves[i].first);
            printCoordinates(available_moves[i].second);
            cout<<endl;
        }
        bool picking=true;
        int picked;
        while(picking)
        {
            cout<<"Enter Index of Chosen Move"<<endl;
            cin>>picked;
            picked-=1;
            if(picked>=0&&picked<available_moves.size())
                picking=false;
        }
        move(available_moves[picked].first,available_moves[picked].second);
        return picked;
    }
    if(available_jumps.size()>0)
    {
        cout<<"Available Jumps:"<<endl;
        for(int i=0;i<available_jumps.size();i++)
        {
            cout<<i+1<<": ";
            printCoordinates(available_jumps[i][0].first);
            for(int j=0;j<available_jumps[i].size();j++)
            {
                printCoordinates(available_jumps[i][j].second);
            }
            cout<<endl;
        }
        bool picking=true;
        int picked;
        while(picking)
        {
            cout<<"Enter Index of Chosen Jump"<<endl;
            cin>>picked;
            picked-=1;
            if(picked>=0&&picked<available_jumps.size())
                picking=false;
        }
        int isqueen=0;
        if(spaces[available_jumps[picked][0].first/10][available_jumps[picked][0].first%10]%2==0)
            isqueen+=100;
        for(int i=0;i<available_jumps[picked].size();i++)
        {
            jump(available_jumps[picked][i].first+isqueen,available_jumps[picked][i].second);
        }
        return picked;
    }
    return 0;
}

int board::mctsAction(int color)
{
    int picked=mctsChoice(color);
    cout<<"MCTS move:"<<endl;
    vector <vector <pair<int,int> > > available_jumps=listOfJumps(color);
    if(available_jumps.size()==0)
    {
        vector <pair <int,int> > available_moves;
        if(color==0)
        {
            available_moves=listOfWhiteMoves();
        }
        if(color==1)
        {
            available_moves=listOfBlackMoves();
        }
        move(available_moves[picked].first,available_moves[picked].second);
        return picked;
    }
    if(available_jumps.size()>0)
    {
        int isqueen=0;
        if(spaces[available_jumps[picked][0].first/10][available_jumps[picked][0].first%10]%2==0)
            isqueen+=100;
        for(int i=0;i<available_jumps[picked].size();i++)
        {
            jump(available_jumps[picked][i].first+isqueen,available_jumps[picked][i].second);
        }
        return picked;
    }
    return 0;
}

void board::checkForMoves(int color)
{
    if(color==0)
    {
        if(listOfJumps(0).size()==0&&listOfWhiteMoves().size()==0)
            ongoing=false;
    }
    if(color==1)
    {
        if(listOfJumps(1).size()==0&&listOfBlackMoves().size()==0)
            ongoing=false;
    }
    if(moves_since_last_jump>=40)
    {
        ongoing=false;
    }
}

float board::evaluateBoard(int color)
{
    if(moves_since_last_jump>=40)
        return 0.5;
    float whitevalue=0,blackvalue=0;
    for(int i=0;i<white.size();i++)
    {
        whitevalue++;
        if(white[i]>100)
            whitevalue+=2;
    }
    for(int i=0;i<black.size();i++)
    {
        blackvalue++;
        if(black[i]>100)
            blackvalue+=2;
    }
    if(color==0)
    {
        return whitevalue/(whitevalue+blackvalue);
    }
    return blackvalue/(whitevalue+blackvalue);
}


int board::minimaxAction(int color)
{
    int picked=minimaxChoice(*this,color,9);
    cout<<"Minimax move:"<<endl;
    vector <vector <pair<int,int> > > available_jumps=listOfJumps(color);
    if(available_jumps.size()==0)
    {
        vector <pair <int,int> > available_moves;
        if(color==0)
        {
            available_moves=listOfWhiteMoves();
        }
        if(color==1)
        {
            available_moves=listOfBlackMoves();
        }
        move(available_moves[picked].first,available_moves[picked].second);
        return picked;
    }
    if(available_jumps.size()>0)
    {
        int isqueen=0;
        if(spaces[available_jumps[picked][0].first/10][available_jumps[picked][0].first%10]%2==0)
            isqueen+=100;
        for(int i=0;i<available_jumps[picked].size();i++)
        {
            jump(available_jumps[picked][i].first+isqueen,available_jumps[picked][i].second);
        }
        return picked;
    }
    return 0;
}

int board::alphaBetaAction(int color)
{
    int picked=alphaBetaChoice(*this,color,9);  //depth 9 gives 8s per move on test pc
    cout<<"AlphaBeta move:"<<endl;
    vector <vector <pair<int,int> > > available_jumps=listOfJumps(color);
    if(available_jumps.size()==0)
    {
        vector <pair <int,int> > available_moves;
        if(color==0)
        {
            available_moves=listOfWhiteMoves();
        }
        if(color==1)
        {
            available_moves=listOfBlackMoves();
        }
        move(available_moves[picked].first,available_moves[picked].second);
        return picked;
    }
    if(available_jumps.size()>0)
    {
        int isqueen=0;
        if(spaces[available_jumps[picked][0].first/10][available_jumps[picked][0].first%10]%2==0)
            isqueen+=100;
        for(int i=0;i<available_jumps[picked].size();i++)
        {
            jump(available_jumps[picked][i].first+isqueen,available_jumps[picked][i].second);
        }
        return picked;
    }
    return 0;
}