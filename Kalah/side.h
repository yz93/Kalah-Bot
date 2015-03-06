#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH };

inline
Side opponent(Side s)
{
	return s == NORTH ? SOUTH : NORTH;
}
#endif  //SIDE_H