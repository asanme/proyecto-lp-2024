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
	static void getXmlElementNodeReferences(const XmlElement& xmlElement, std::vector<std::string>& nodeReferences);
	static std::string getXmlElementValue(const XmlElement& xmlElement, const std::string& tagName);
	static std::string getXmlElementAttribute(const XmlElement& xmlElement, const std::string& attributeName);
	static Coordinate getNodeCoordinatesById(const std::vector<XmlElement>& xml, const std::string& nodeId);

public:
	void getPdis(std::vector<PuntDeInteresBase*>&) override;
	void getCamins(std::vector<CamiBase*>&) override;
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override;
};
