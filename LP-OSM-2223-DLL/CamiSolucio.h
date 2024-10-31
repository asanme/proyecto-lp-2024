#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase
{
public:
	// TODO Remove this later on
	CamiSolucio(): CamiBase(), m_pathCoordinates(stubGetCamiCoords())
	{
	}

	CamiSolucio(const std::vector<Coordinate>& pathCoordinates) : CamiBase(), m_pathCoordinates(pathCoordinates)
	{
	}

	std::vector<Coordinate> getCamiCoords() override;

private:
	std::vector<Coordinate> m_pathCoordinates;

	static std::vector<Coordinate> stubGetCamiCoords();
};
