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
	static void getElementNodeReferences(const XmlElement& xmlElement, std::vector<std::string>& nodeReferences);

	// Parsers
	void parseInterestPoint(const XmlElement& xmlElement);
	void parsePath(std::vector<XmlElement>& xmlElements, const XmlElement& xmlElement);

public:
	// This constructor is only a stub for the real implementation
	MapaSolucio(): MapaBase()
	{
		auto p1 = new PuntDeInteresBotigaSolucio(
			Coordinate{41.4918606, 2.1465411},
			"La Millor Pastisseria",
			"bakery",
			"",
			""
		);

		auto p2 = new PuntDeInteresRestaurantSolucio(
			Coordinate{41.4902204, 2.1406477},
			"El Millor Restaurant",
			"regional",
			"yes"
		);

		m_camins.push_back(new CamiSolucio());
		m_puntsInteres.push_back(p1);
		m_puntsInteres.push_back(p2);
	}

	void getPdis(std::vector<PuntDeInteresBase*>&) override;
	void getCamins(std::vector<CamiBase*>&) override;
	void parsejaXmlElements(std::vector<XmlElement>&) override;
};
