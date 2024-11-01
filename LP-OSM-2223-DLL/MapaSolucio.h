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

	typedef struct
	{
		long long id;
		Coordinate coordinates;
	} EmptyNode;

	typedef struct
	{
		std::vector<long long> nodeRef;
	} PathNode;

	// Other nodes
	std::vector<PathNode> m_pathNodes;
	std::vector<EmptyNode> m_emptyNodes;

	std::vector<CamiBase*> m_paths;
	std::vector<PuntDeInteresBase*> m_interestPoints;

	// Checks
	static NodeType getNodeType(const XmlElement& element);
	static bool isElementInterestPoint(const XmlElement& element);
	static bool elementContainsTag(const XmlElement& element);

	// Helper methods
	static std::string getElementChildValue(const XmlElement& element, const std::string& keyName);
	static std::string getElementAttributeValue(const XmlElement& element, const std::string& keyName);
	static std::vector<long long> getElementNodeReferences(const XmlElement& element);
	Coordinate getCoordinateById(const long long& nodeId);

	// Parsers
	static EmptyNode parseEmptyNode(const XmlElement& element);
	static PathNode parsePathNode(const XmlElement& element);
	CamiBase* parsePath(const PathNode& pathNode);

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
