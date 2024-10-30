#include "pch.h"
#include "PuntDeInteresRestaurantSolucio.h"

std::string PuntDeInteresRestaurantSolucio::getName()
{
	return PuntDeInteresBase::getName();
}

unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
	unsigned int color = PuntDeInteresBase::getColor();

	if (m_tipusCuina == "pizza" && m_mobilitatReduida == "yes")
	{
		color = 0x03FCBA;
	}
	else if (m_tipusCuina == "chinese")
	{
		color = 0x03FCBA;
	}
	else if (m_mobilitatReduida == "yes")
	{
		color = 0x251351;
	}

	return color;
}
