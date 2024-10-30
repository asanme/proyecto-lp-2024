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

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	for (const XmlElement& element : xmlElements)
	{
		// En caso de tratarse de un punto de interes, tenemos que determinar si se trata de una tienda o de un restaurante
		if (isElementInterestPoint(element))
		{
			// Tiendas -> contienen la clave "shop"
			// Restaurante -> contienen el par k="amenity" v="restaurant"
			// Esto en principio deberia de decirme si se trata de un restaurante o de una tienda
			for (auto child : element.fills)
			{
				std::vector<std::pair<std::string, std::string>> currentTag = child.second;
				std::pair<std::string, std::string> keyTag = currentTag[0];
				std::pair<std::string, std::string> valueTag = currentTag[1];

				// Si esto es cierto, es un restaurante
				if (keyTag.second == "amenity" && valueTag.second == "restaurant")
				{
					break;
				}
				// Se trata de una tienda
				else if(keyTag.second == "shop")
				{
					break;
				}
			}
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
