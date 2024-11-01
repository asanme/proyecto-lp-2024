#pragma once
#include "CamiSolucio.h"
#include "MapaBase.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

class MapaSolucio : public MapaBase
{
private:
	// Filter node types
	enum NodeType
	{
		NOT_APPLICABLE,
		INTEREST_POINT,
		PATH_NO_TAG,
		PATH_NO_NAME,
		PATH_HIGHWAY
	};

	std::vector<PuntDeInteresBase*> m_interestPoints;
	std::vector<CamiBase*> m_paths;

	// Checks
	static NodeType getNodeType(const XmlElement& element);
	static bool isElementInterestPoint(const XmlElement& element);
	static bool elementContainsTag(const XmlElement& element);

	// Helper methods
	static std::string getElementChildValue(const XmlElement& element, const std::string& keyName);
	static std::string getElementAttributeValue(const XmlElement& element, const std::string& keyName);
	static Coordinate getNodeCoordinatesById(const std::vector<XmlElement>& xml, const std::string& nodeId);
	static std::vector<std::string> getElementNodeReferences(const XmlElement& element);

	// Parsers
	void parsePath(std::vector<XmlElement>& xmlElements, const XmlElement& element);
	static PuntDeInteresBase* parseInterestPoint(const XmlElement& element);
	static PuntDeInteresBase* parseRestaurant(const XmlElement& element);
	static PuntDeInteresBase* parseShop(const XmlElement& element);

public:
	MapaSolucio() : MapaBase()
	{
	}

	void getPdis(std::vector<PuntDeInteresBase*>&) override;
	void getCamins(std::vector<CamiBase*>&) override;
	void parsejaXmlElements(std::vector<XmlElement>&) override;
};
