#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase
{
public:
	CamiSolucio(const std::vector<Coordinate>& pathCoordinates):
		m_pathCoordinates(pathCoordinates)
	{
	}

	std::vector<Coordinate> getCamiCoords() override;
	std::vector<Coordinate> stubGetCamiCoords();

private:
	std::vector<Coordinate> m_pathCoordinates;
};
