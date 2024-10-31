#pragma once

#include <string>
#include "Common.h"

class PuntDeInteresBase
{
private:
	Coordinate m_coord;
	std::string m_name;

public:
	PuntDeInteresBase();
	PuntDeInteresBase(Coordinate coord, std::string name);

	Coordinate getCoord();
	virtual std::string getName();
	virtual unsigned int getColor();
};
