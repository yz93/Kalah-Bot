#include "game.h"
//#include <algorithm>
Game::Game(const Board& b, Player* south, Player* north)
	:m_b(b), m_south(south), m_north(north), m_turn(SOUTH)
{

}

void Game::display() const
{
	cout <<"  "<< m_north->name() << endl;
	printSide(NORTH);
	cout << m_b.beans(NORTH, 0)<<"	";
	for (int i = 0; i <=m_b.holes(); ++i)
		cout << " ";
	cout <<m_b.beans(SOUTH, 0)<<endl;
	printSide(SOUTH);
	cout << "  " << m_south->name() << endl;
}

void Game::printSide(Side s) const{
	cout << "  ";
	for (int i = 1; i <= m_b.holes(); ++i)
		cout << m_b.beans(s,i) << " ";
	cout << endl;
}

/*status checks if both sides of the board are empty, if so, game is over, and it counts 
the number of seeds in the pots then announces a winner.*/
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	int northBeans = m_b.beansInPlay(NORTH);
	int southBeans = m_b.beansInPlay(SOUTH);

	/*at this stage, both rows should have been cleaned and all seeds are in pots*/
	if (northBeans == 0 && southBeans == 0){
		over = true;
		int nPot = m_b.beans(NORTH, 0);
		int sPot = m_b.beans(SOUTH, 0);
		hasWinner = (nPot != sPot);
		if (hasWinner)
		{
			if (nPot > sPot)
				winner = NORTH;
			else if (nPot < sPot)
				winner = SOUTH;
		}
		else
			return;

	}
	else{
		over = false; 
		return;
	}
}

bool Game::move()
{
	bool over=false, hasWinner;
	Side s;
	status(over, hasWinner, s);
	if (over)
		return false;
	Side oppo = opponent(m_turn);
	bool notFinished = true;
	while (notFinished)
	{
		int move = playerMove(m_turn);
		//cout << "MOVE IS: " << move << endl;
		Side endSide;
		int endHole;
		
		if (!m_b.sow(m_turn, move, endSide, endHole))
			cerr << "Error: Can't sow!" << endl;
		string name;
		if (m_turn == NORTH)
			name = m_north->name();
		else if (m_turn == SOUTH)
			name = m_south->name();
		display();
		
		/*check if the game is over after this move. If so, move all remaining seeds to pots*/
		if (m_b.beansInPlay(NORTH) == 0)
		{
			for (int i = 1; i <= m_b.holes(); ++i)
				if (m_b.beans(SOUTH, i) > 0)
					m_b.moveToPot(SOUTH, i, SOUTH);
			display();
			cout << endl << name << " finished his step. Game is over!" << endl;
			break;
		}
		else if (m_b.beansInPlay(SOUTH) == 0)
		{
			for (int i = 1; i <= m_b.holes(); ++i)
				if (m_b.beans(NORTH, i) > 0)
					m_b.moveToPot(NORTH, i, NORTH);
			display();
			cout << endl << name << " finished his step. Game is over!" << endl;
			break;
		}
		/*taking an additional turn because ended in my pot*/
		if (endHole == 0 && endSide == m_turn)
		{
			cout << name <<" takes another turn..." << endl;
			cout << endl << "Press ENTER to proceed..." << endl;
			cin.ignore(10, '\n');
			continue;
		}
		
		/*capture: If the end side is the player's side, and the hole was 0 before, and my opponent's same hole is not 0*/
		/*Then move my opponent's seeds and the 1 seed in the end hole to the player's pot*/
		else if ((m_b.beans(m_turn, endHole) == 1) && (endSide == m_turn) && (m_b.beans(oppo, endHole) > 0))
		{
			m_b.moveToPot(oppo, endHole, m_turn);
			m_b.moveToPot(m_turn, endHole, m_turn);
			display();
			cout << name << " has a capture..." << endl;
			if (m_b.beansInPlay(NORTH) == 0)
			{
				for (int i = 1; i <= m_b.holes(); ++i)
					if (m_b.beans(SOUTH, i) > 0)
						m_b.moveToPot(SOUTH, i, SOUTH);
				display();
				cout << endl << name << "Game is over! ";
				break;
			}
			else if (m_b.beansInPlay(SOUTH) == 0)
			{
				for (int i = 1; i <= m_b.holes(); ++i)
					if (m_b.beans(NORTH, i) > 0)
						m_b.moveToPot(NORTH, i, NORTH);
				display();
				cout << endl << "Game is over! ";
				break;
			}
			else
			{
				cout << endl << name << " finished his step: Press ENTER to proceed..." << endl;
				cin.ignore(10, '\n');
				notFinished = false;
			}
		}
		else
		{
			cout << endl << name << " finished his step: Press ENTER to proceed..." << endl;
			cin.ignore(10, '\n');
			notFinished = false;
		}
	}
	m_turn = oppo;
	return true;
}
int Game::playerMove(Side turn)
{
	int move;
	if (turn == SOUTH)
		move = m_south->chooseMove(m_b, SOUTH);
	else if (turn == NORTH)
		move = m_north->chooseMove(m_b, NORTH);

	return move;
}

void Game::play()
{
	bool over;
	bool hasWinner;
	Side winner;
	string winnerName;
	cout << "Initial board:" << endl;
	display();
	cout << "Press ENTER to continue..." << endl;
	cin.ignore(10 , '\n');
	while (move()) {}
	
	status(over, hasWinner, winner);
	if (hasWinner)
	{
		if (winner == NORTH)
			winnerName = m_north->name();
		else
			winnerName = m_south->name();
		cout << "The winner is: " << winnerName << endl;
	}
	else
		cout << "It is a tie!" << endl;
}

int Game::beans(Side s, int hole) const{
	return m_b.beans(s, hole);
}