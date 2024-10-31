#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase
{
public:
	CamiSolucio(const std::vector<Coordinate>& pathCoordinates) : CamiBase(), m_pathCoordinates(0)
	{
		for (Coordinate coord : pathCoordinates)
			m_pathCoordinates.push_back(coord);
	}

	std::vector<Coordinate> getCamiCoords() override;

private:
	std::vector<Coordinate> m_pathCoordinates;
};
