#include "player.h"
#include <iostream>
using namespace std;

//inline
Player::Player(std::string name)
: m_name(name) {
}

//inline  // use inline must be in header file, not cpp!
string Player::name() const{
	return m_name;
}

BadPlayer::BadPlayer(string name)
	:Player(name)
{}

int BadPlayer::chooseMove(const Board& b, Side s) const{
	for (int i = 1; i <= b.holes(); ++i)
	{
		if (b.beans(s, i) > 0)
			return i;  // return the number of this hole
	}
	
	return -1;
}

HumanPlayer::HumanPlayer(string name)
	:Player(name)
{}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
	cout << "Please enter a position: ";
	int position;
	cin >> position;
	while (position > b.holes() || position <= 0 || (b.beans(s,position)==0)){
		cout <<"Please enter a valid position: ";
		cin>>position;
	}

	return position;
}

SmartPlayer::SmartPlayer(string name)
	:Player(name)
{}
/*fakeMove is to execute a move on the board, called by minimax*/
/*@parameter Board& b will be passed in a copy of minimax's board.*/
/*After fakeMove, minimax is called again on the opponent's turn*/
/*fakeMove is recursive only because I might need to take another turn, in this case, I have to try every possible move,
and each move might results in another move... Until I finish my turn, I need to keep simulating the game. Then I return the
utility of the current board. The called collects all the utilities resulted from all possible moves and make the best move.
The reason this function is separated from minimax while having similar structure is that I want MAX and MIN to alternate between levels.*/
int SmartPlayer::fakeMove(Board& b, Side s, int move) const{
	Side oppo = opponent(s);
	
	Side endSide;
	int endHole;

	b.sow(s, move, endSide, endHole);
		
	/*check if the game is over after this move. If so, move all remaining seeds to pots*/
	if (b.beansInPlay(NORTH) == 0){
		for (int i = 1; i <= b.holes(); ++i)
			if (b.beans(SOUTH, i) > 0)
				b.moveToPot(SOUTH, i, SOUTH);
		return utilityFunc(b);
	}
	else if (b.beansInPlay(SOUTH) == 0){
		for (int i = 1; i <= b.holes(); ++i)
			if (b.beans(NORTH, i) > 0)
				b.moveToPot(NORTH, i, NORTH);
		return utilityFunc(b);
	}
	/*game not over*/
	/*taking an additional turn because ended in my pot*/
	/*now I need to try all moves until my turn is over, then compare all the utilities, then return an utility*/
	if (endHole == 0 && endSide == s){
		if (s == SOUTH){
			int currBestUtil = INT_MIN;
			Board currBestBoard = b;
			for (int i = 1; i <= b.holes(); ++i){
				if (b.beans(s, i) > 0)
				{
					Board b_copy = b;
					int util = fakeMove(b_copy, s, i);
					if (i == 1)
						currBestBoard = b_copy;
					if (util > currBestUtil)
					{
						currBestBoard = b_copy;
						currBestUtil = util;
					}
				}
			}
			b = currBestBoard;
			return currBestUtil;
		}
		else if (s == NORTH){
			int currBestUtil = INT_MAX;
			Board currBestBoard = b;
			for (int i = 1; i <= b.holes(); ++i){
				if (b.beans(s, i) > 0)
				{
					Board b_copy = b;
					int util = fakeMove(b_copy, s, i);
					if (util < currBestUtil)
					{
						currBestBoard = b_copy;
						currBestUtil = util;
					}
				}
			}
			b = currBestBoard;
			return currBestUtil;
		}
	}
	/*capture: If the end side is the player's side, and the hole was 0 before, and my opponent's same hole is not 0*/
	/*Then move my opponent's seeds and the 1 seed in the end hole to the player's pot*/
	else if ((b.beans(s, endHole) == 1) && (endSide == s) && (b.beans(oppo, endHole) > 0)){
		b.moveToPot(oppo, endHole, s);
		b.moveToPot(s, endHole, s);
		return utilityFunc(b);
	}
	else
		return utilityFunc(b);
}
/*high utility is to SOUTH player, because SOUTH is MAX*/
int SmartPlayer::utilityFunc(const Board& b) const{
	int northBeans = b.beansInPlay(NORTH);
	int southBeans = b.beansInPlay(SOUTH);
	int nPot = b.beans(NORTH, 0);
	int sPot = b.beans(SOUTH, 0);
	if (northBeans == 0 && southBeans == 0){
		if (nPot > sPot)
			return INT_MIN;
		else if (nPot < sPot)
			return INT_MAX;
		else  // tie;
			return 0;
	}

	return sPot - nPot;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const{
	int act, util;
	minimax(b,s,0,act,util);
	//cout << "util is " << util;
	/*cout <<endl<<"SmartPlayer finished his step: Press ENTER to proceed..." << endl;
	cin.ignore(10, '\n');*/
	return act;
}

void SmartPlayer::minimax(const Board& b, Side s, int level, int& bestAction, int& utility) const{
	if (s != NORTH && s != SOUTH)
		return;

	if (b.beansInPlay(s) == 0 || level == MAX_LEVEL){  // game over or leaf node{
		//bestAction = -1;  // no action
		utility = utilityFunc(b);  // terminal state, leaf node, static evaluation function
		//cout << "minimax utilitiy is:  " << utility;
		return;
	}
	int currBestMove = 0;
	int randomMoveWhenLosing;
	if (s == SOUTH){  // south player is MAX
		int currMax = INT_MIN;
		
		for (int i = 1; i <= b.holes(); ++i){
			if (b.beans(s, i) > 0){
				randomMoveWhenLosing = i;
				Board copyB = b;
				fakeMove(copyB, s, i);
				minimax(copyB, opponent(s), level+1, bestAction, utility);
				if (utility > currMax){
					currMax = utility;
					//bestAction = i;
					currBestMove = i;
				}
			}
		}
		if (currBestMove != 0)
			bestAction = currBestMove;
		if (currMax == INT_MIN)
			bestAction = randomMoveWhenLosing;
	}
	else{  // side is north, MIN
		int currMin = INT_MAX;
		for (int i = 1; i <= b.holes(); ++i){
			if (b.beans(s, i) > 0){
				randomMoveWhenLosing = i;
				Board copyB = b;
				fakeMove(copyB, s, i);
				minimax(copyB, opponent(s), level + 1, bestAction, utility);
				if (utility < currMin){
					currMin = utility;
					//bestAction = i;
					currBestMove = i;
				}
			}
		}
		if (currBestMove != 0)
			bestAction = currBestMove;
		if (currMin == INT_MAX)
			bestAction = randomMoveWhenLosing;
	}
}