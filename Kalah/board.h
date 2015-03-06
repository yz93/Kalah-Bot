#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "board.h"
#include "side.h"
using namespace std;

class Board{
public:
	Board(int nHoles, int nInitialBeansPerHole);
	int holes() const { return m_holes; }
	//Return the number of holes on a side(not counting the pot).
	int beans(Side s, int hole) const;
	//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
	int beansInPlay(Side s) const;
	//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
	int totalBeans() const { return m_totalBeans; }
	//Return the total number of beans in the game.
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	//If the hole indicated by(s, hole) is empty or invalid or a pot, this function returns false without changing anything.
	//Otherwise, it will return true after sowing the beans : the beans are removed from hole(s, hole) and sown counterclockwise, 
	//including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to 
	//the side and hole where the last bean was placed. (This function does not make captures or multiple turns; 
	//different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.) 
	bool moveToPot(Side s, int hole, Side potOwner);
	//If the indicated hole is invalid or a pot, return false without changing anything.Otherwise, move all the beans in 
	//hole(s, hole) into the pot belonging to potOwner and return true.
	bool setBeans(Side s, int hole, int beans);
private:
	bool invalidSideHole(Side s, int hole) const{
		if ((s != Side::NORTH) && (s != Side::SOUTH))
			return true;
		if ((hole < 0) || (hole > m_holes))
			return true;
		return false;
	}
private:
	int m_holes;
	int m_nInitialBeansPerHole;
	//int m_beansInPlay;
	int m_totalBeans;
	map<Side, vector<int>> m_board;
	//vector<int> m_northSide;// 0 is the pot. other numbers are the holes.
	//vector<int> m_southSide;
};

#endif  //BOARD_H