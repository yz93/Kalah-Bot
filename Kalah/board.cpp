#include "board.h"
#include <iostream>
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole)
	:m_holes(nHoles), m_nInitialBeansPerHole(nInitialBeansPerHole), m_totalBeans(2*nHoles*nInitialBeansPerHole)
{
	vector<int> temp;
	temp.push_back(0);
	for (int i = 1; i <= m_holes; ++i)
		temp.push_back(m_nInitialBeansPerHole);
	m_board[NORTH] = temp;
	m_board[SOUTH] = temp;
}

int Board::beans(Side s, int hole) const
{
	if (invalidSideHole(s, hole))
		return -1;
	return m_board.at(s)[hole];
}

int Board::beansInPlay(Side s) const
{
	if (invalidSideHole(s, 0))
		return -1;
	int count = 0;
	for (int i = 1; i <= m_holes; ++i)
		count+=m_board.at(s)[i];
	return count;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	if (invalidSideHole(s, hole))
		return false;
	if (hole == 0)  // can't sow a pot
		return false;
	if (m_board[s][hole] == 0)  // hole is empty
		return false;

	int seeds = m_board[s][hole];
	m_board[s][hole] = 0;
	Side currentSide = s;
	int currentHole;
	if (hole == m_holes && currentSide == SOUTH)
		currentHole = 0;
	else if (hole == 1 && currentSide == NORTH)
		currentHole = 0;
	else
		if (currentSide == SOUTH)
			currentHole = hole + 1;
		else
			currentHole = hole - 1;
	while (seeds > 0)
	{
		if (currentHole > m_holes)
			currentHole = 0;
		if (currentHole == 0){
			if (currentSide == s)
			{
				m_board[currentSide][currentHole]++;
				seeds--;
				if (seeds == 0)
					break;
			}
			// update side and hole
			currentSide = opponent(currentSide);
			if (currentSide == NORTH)
				currentHole = m_holes;
			else if (currentSide == SOUTH)
				currentHole = 1;
		}
		else
		{
			m_board[currentSide][currentHole]++;
			seeds--;
			if (seeds == 0)
				break;
			if (currentSide == SOUTH)
				currentHole++;
			else if (currentSide == NORTH)
				currentHole--;
		}
	}
	endSide = currentSide;
	endHole = currentHole;
	
	return true;
}
//If the hole indicated by(s, hole) is empty or invalid or a pot, this function returns false without changing anything.
//Otherwise, it will return true after sowing the beans : the beans are removed from hole(s, hole) and sown counterclockwise, 
//including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to 
//the side and hole where the last bean was placed. (This function does not make captures or multiple turns; 
//different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.) 

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if (invalidSideHole(s, hole))
		return false;
	if (potOwner != NORTH && potOwner != SOUTH)
		return false;
	m_board[potOwner][0] += m_board[s][hole];  // put beans in pot
	//m_beansInPlay -= m_board[s][hole];  // take out beans in play
	m_board[s][hole] = 0;  // set the hole to be empty
	return true;
}

// needs to change beans in play and total beans
bool Board::setBeans(Side s, int hole, int beans){
	if (invalidSideHole(s, hole) || beans < 0)
		return false;
	
	m_totalBeans += (beans-m_board[s][hole]);
	m_board[s][hole] = beans;
	return true;
}