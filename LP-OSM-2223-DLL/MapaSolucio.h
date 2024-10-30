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
	std::string getXmlElementValue(const XmlElement& xmlElement, const std::string& tagName);
	std::string getXmlElementAtribute(const XmlElement& xmlElement, const std::string& atributeName);

	/*
		Para tenerlo de referencia

		typedef struct
		{
			std::string id_element;
			std::vector<PAIR_ATTR_VALUE> atributs;
			std::vector<CHILD_NODE> fills;
		} XmlElement;
	*/

	std::string getAttributeValue(const std::vector<PAIR_ATTR_VALUE>& attributes);

public:
	void getPdis(std::vector<PuntDeInteresBase*>&) override;
	void getCamins(std::vector<CamiBase*>&) override;
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override;
};
