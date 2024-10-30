#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase
{
public:
	PuntDeInteresRestaurantSolucio(
		Coordinate coord,
		const std::string& name,
		const std::string& cuisine,
		const std::string& wheelchair
	):
		PuntDeInteresBase(coord, name),
		m_tipusCuina(cuisine),
		m_mobilitatReduida(wheelchair)
	{
	}

	std::string getName() override;
	unsigned int getColor() override;

private:
	std::string m_tipusCuina;
	std::string m_mobilitatReduida;
};
