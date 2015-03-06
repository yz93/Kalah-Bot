#include "game.h"
#include "player.h"
#include "board.h"
#include "side.h"
#include <iostream>
#include <cassert>
using namespace std;

void doBoardTests()
{
	Board b(3, 2);
	assert(b.holes() == 3);
	assert(b.totalBeans() == 12);
	assert(b.beans(SOUTH, 0) == 0);
	assert(b.beansInPlay(SOUTH) == 6);
	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11 && b.beans(SOUTH, 1) == 1 &&
		b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, 0) == 2 &&
		b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH);
	assert(eh == 3);
	assert(b.beans(SOUTH, 3) == 0);
	assert(b.beans(NORTH, 3) == 3);
	assert(b.beans(SOUTH, 0) == 3);
	assert(b.beansInPlay(SOUTH) == 1);
	assert(b.beansInPlay(NORTH) == 7);
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge"  &&  hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer"  &&  !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	cout << "=========" << endl;
	int n = hp.chooseMove(b, SOUTH);
	cout << "=========" << endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
	n = sp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
}

void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	//    Homer
	//   0  1  2
	// 0         0
	//   2  0  0
	//    Bart
	g.status(over, hasWinner, winner);
	assert(!over);
	assert(g.beans(NORTH, 0) == 0);
	assert(g.beans(SOUTH, 0) == 0);
	assert(g.beans(NORTH, 1) == 0);
	assert(g.beans(NORTH, 2) == 1);
	assert(g.beans(NORTH, 3) == 2);
	assert(g.beans(SOUTH, 1) == 2);
	assert(g.beans(SOUTH, 2) == 0);
	assert(g.beans(SOUTH, 3) == 0);

	g.move();
	//   0  1  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over);
	assert(g.beans(NORTH, 0) == 0);
	assert(g.beans(SOUTH, 0) == 3);
	assert(g.beans(NORTH, 1) == 0);
	assert(g.beans(NORTH, 2) == 1); 
	assert(g.beans(NORTH, 3) == 0);
	assert(g.beans(SOUTH, 1) == 0);
	assert(g.beans(SOUTH, 2) == 1);
	assert(g.beans(SOUTH, 3) == 0);

	g.move();
	//   1  0  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, 0) == 0 && g.beans(SOUTH, 0) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   1  0  0
	// 0         3
	//   0  0  1
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, 0) == 0 && g.beans(SOUTH, 0) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

	g.move();
	//   0  0  0
	// 1         4
	//   0  0  0
	g.status(over, hasWinner, winner);
	assert(over && g.beans(NORTH, 0) == 1 && g.beans(SOUTH, 0) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}


int main()
{
	//doBoardTests();
	//doPlayerTests();
	//doGameTests();
	//cout << "Passed all tests" << endl;
	cout << "Press ENTER to start the Game..." << endl;
	cin.ignore(10, '\n');
	SmartPlayer bp1("Zhao");
	SmartPlayer bp2("Jiaqi");
	Board b(3, 0);
	Board b2(6,0);
	Board b3(6, 0);
	b3.setBeans(NORTH, 2, 1);
	b3.setBeans(NORTH, 5, 2);
	b3.setBeans(NORTH, 0, 22);
	b3.setBeans(SOUTH, 3, 2);
	b3.setBeans(SOUTH, 5, 1);
	b3.setBeans(SOUTH, 0, 20);

	b2.setBeans(NORTH, 1, 2);
	b2.setBeans(NORTH, 2, 4);

	for (int i = 3; i <= 6;++i)
		b2.setBeans(SOUTH, i, 1);
	b2.setBeans(NORTH, 0, 22);
	b2.setBeans(SOUTH, 0, 16);

	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	Game g2(b2, &bp1, &bp2);
	Game g3(b3, &bp1, &bp2);

	g.play();
	//g2.play();
	//g3.play();
	
}
