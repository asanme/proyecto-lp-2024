#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase
{
public:
	std::string getName() override;
	unsigned int getColor() override;

private:
	std::string m_tipusCuina;
	std::string m_mobilitatReduida;
};
