#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "board.h"
#include "side.h"
using namespace std;
#define MAX_LEVEL 11

class Player{
public:
	Player(std::string name);
	// a Player with the indicated name.
	string name() const;
	//Return the name of the player
	virtual bool isInteractive() const { return false; }
	//Return false if the player is a computer player.Return true if the player is human.Most kinds of players will be computer players.
	virtual int chooseMove(const Board& b, Side s) const = 0;
	//Every concrete class derived from this class must implement this function so that if the player were to be playing 
	//side s and had to make a move given board b, the function returns the move the player would choose.If no move is possible, return −1.
	virtual ~Player() {}
	//Since this class is designed as a base class, it should have a virtual destructor.
private:
	string m_name;
};

class BadPlayer : public Player{
public:
	BadPlayer(string name);
	virtual bool isInteractive() const { return false; }
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~BadPlayer() {}
};

class HumanPlayer : public Player
{
public:
	HumanPlayer(string name);
	virtual bool isInteractive() const { return true; }
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~HumanPlayer() {}
};


class SmartPlayer:public Player{
public:
	SmartPlayer(string name);
	virtual bool isInteractive() const { return false; }
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~SmartPlayer() {}
private:
	int fakeMove(Board& b, Side s, int move) const;
	void minimax(const Board& b, Side s, int level, int& bestAction, int& utility) const;
	int utilityFunc(const Board& b) const;
};
#endif // PLAYER_H

	
