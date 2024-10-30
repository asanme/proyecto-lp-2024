#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase
{
public:
	std::vector<Coordinate> getCamiCoords() override;
	std::vector<Coordinate> stubGetCamiCoords();

private:
	std::vector<Coordinate> m_pathCoordinates;
};
