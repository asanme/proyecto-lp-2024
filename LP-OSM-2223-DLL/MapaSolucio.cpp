#include "pch.h"
#include "MapaSolucio.h"

#include "Util.h"


void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& puntsInteres)
{
	puntsInteres = m_puntsInteres;
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& camins)
{
	camins = m_camins;
}


/*
	Para tenerlo de referencia

	typedef struct
	{
		std::string id_element;
		std::vector<PAIR_ATTR_VALUE> atributs;
		std::vector<CHILD_NODE> fills;
	} XmlElement;
 */
void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	for (const XmlElement& element : xmlElements)
	{
		// En caso de tratarse de un punto de interes, tenemos que determinar si se trata de una tienda o de un restaurante
		if (isElementInterestPoint(element))
		{
			
			// Tiendas -> contienen el tag "shop"
			// Restaurante -> contiene el par k="amenity" v="restaurant"
		}
		else if (isElementPath(element))
		{

		}
	}
}

bool MapaSolucio::isElementPath(const XmlElement& element)
{
	return element.id_element == "way";
}

bool MapaSolucio::isElementInterestPoint(const XmlElement& element)
{
	return element.id_element == "node";
}

std::string MapaSolucio::getXmlElementValue(const XmlElement& xmlElement, const std::string& tagName)
{
	for (CHILD_NODE child : xmlElement.fills)
	{
		if (child.first == "tag")
		{
			const std::pair<std::string, std::string> keyValue = Util::kvDeTag(child.second);
			if (keyValue.first == tagName)
				return keyValue.second;
		}
	}

	 return std::string("");
}

std::string MapaSolucio::getXmlElementAtribute(const XmlElement& xmlElement, const std::string& atributeName)
{
	for (const PAIR_ATTR_VALUE& child : xmlElement.atributs)
	{
		if (child.first == atributeName)
			return child.second;
	}

	return std::string("");
}
