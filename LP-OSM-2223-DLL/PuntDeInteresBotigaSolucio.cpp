#include "pch.h"
#include "PuntDeInteresBotigaSolucio.h"

std::string PuntDeInteresBotigaSolucio::getName()
{
	return PuntDeInteresBase::getName();
}

unsigned int PuntDeInteresBotigaSolucio::getColor()
{
	unsigned int color = 0xEFD6AC;

	if(m_shop == "supermarket")
	{
		color = 0xA5BE00;
	}
	else if(m_shop  == "tobacco")
	{
		color = 0xFFAD69;
	}
	else if(m_shop == "bakery")
	{
		color = 0xE85D75;
	}
	else if(m_shop == "bakery" && m_openingHours.find("06:00-22:00") != std::string::npos && m_wheelchair == std::string("yes"))
	{
		color = 0x4CB944;
	}

	return color;
}
