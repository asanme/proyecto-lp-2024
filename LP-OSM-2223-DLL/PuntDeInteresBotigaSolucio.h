#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresBotigaSolucio : public PuntDeInteresBase
{
public:
	std::string getName() override;
	unsigned int getColor() override;

private:
	std::string m_shop;
	std::string m_openingHours;
	std::string m_wheelchair;
};
