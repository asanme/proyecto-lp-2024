#pragma once
#include "CamiSolucio.h"
#include "MapaBase.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

class MapaSolucio : public MapaBase
{
private:
	std::vector<PuntDeInteresBase*> m_puntsInteres;
	std::vector<CamiBase*> m_camins;

	// Checks
	static bool isElementPath(const XmlElement& element);
	static bool isElementInterestPoint(const XmlElement& element);

	// Helper methods
	static std::string getElementChildValue(const XmlElement& xmlElement, const std::string& keyName);
	static std::string getElementAttributeValue(const XmlElement& xmlElement, const std::string& keyName);
	static Coordinate getNodeCoordinatesById(const std::vector<XmlElement>& xml, const std::string& nodeId);
	static std::vector<std::string> getElementNodeReferences(const XmlElement& xmlElement);

	// Parsers
	void parseInterestPoint(const XmlElement& xmlElement);
	void parsePath(std::vector<XmlElement>& xmlElements, const XmlElement& xmlElement);

public:
	MapaSolucio() : MapaBase(), m_puntsInteres(std::vector<PuntDeInteresBase*>(0)), m_camins(std::vector<CamiBase*>(0))
	{
	}

	void getPdis(std::vector<PuntDeInteresBase*>&) override;
	void getCamins(std::vector<CamiBase*>&) override;
	void parsejaXmlElements(std::vector<XmlElement>&) override;
};
