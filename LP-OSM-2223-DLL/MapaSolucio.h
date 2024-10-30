#pragma once
#include "MapaBase.h"

class MapaSolucio : public MapaBase
{
private:
	std::vector<PuntDeInteresBase*> m_puntsInteres;
	std::vector<CamiBase*> m_camins;


	// Helper methods
	static bool isElementPath(const XmlElement& element);
	static bool isElementInterestPoint(const XmlElement& element);

public:
	void getPdis(std::vector<PuntDeInteresBase*>&) override;
	void getCamins(std::vector<CamiBase*>&) override;
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override;
};
