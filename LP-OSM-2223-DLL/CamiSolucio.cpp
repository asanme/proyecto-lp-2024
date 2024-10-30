#include "pch.h"
#include "CamiSolucio.h"

std::vector<Coordinate> CamiSolucio::getCamiCoords()
{
	return stubGetCamiCoords();
}

// TODO Remove this method, this is a stub to see if it renders properly
std::vector<Coordinate> CamiSolucio::stubGetCamiCoords()
{
	/*
	- (41.4928803, 2.1452381)
	- (41.4929072, 2.1452474)
	- (41.4933070, 2.1453852)
	- (41.4939882, 2.1456419)
	 */

	return std::vector<Coordinate>{
		Coordinate{41.4928803, 2.1452381},
		Coordinate{41.4929072, 2.1452474},
		Coordinate{41.4933070, 2.1453852},
		Coordinate{41.4939882, 2.1456419}
	};
}
