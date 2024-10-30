#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresBotigaSolucio : public PuntDeInteresBase
{
public:
	PuntDeInteresBotigaSolucio(
		const Coordinate& coord,
		const std::string& name,
		const std::string& shop,
		const std::string& openingHours,
		const std::string& wheelchair
	):
		PuntDeInteresBase(coord, name),
		m_shop(shop),
		m_openingHours(openingHours),
		m_wheelchair(wheelchair)
	{
	}

	std::string getName() override;
	unsigned int getColor() override;

private:
	std::string m_shop;
	std::string m_openingHours;
	std::string m_wheelchair;
};
