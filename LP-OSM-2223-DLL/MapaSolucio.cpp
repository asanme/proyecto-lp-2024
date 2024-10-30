#include "pch.h"
#include "MapaSolucio.h"
#include "Util.h"

#include <iostream>

#include "CamiSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

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
	for (const XmlElement& xmlElement : xmlElements)
	{
		if (isElementInterestPoint(xmlElement))
		{
			for (auto child : xmlElement.fills)
			{
				std::vector<std::pair<std::string, std::string>> currentTag = child.second;
				std::pair<std::string, std::string> keyTag = currentTag[0];
				std::pair<std::string, std::string> valueTag = currentTag[1];

				if (keyTag.second == "amenity" && valueTag.second == "restaurant")
				{
					std::cout << "------ RESTAURANTE ------\n";

					Coordinate coordinates;
					coordinates.lat = std::stod(getXmlElementAttribute(xmlElement, "lat"));
					coordinates.lon = std::stod(getXmlElementAttribute(xmlElement, "lon"));

					std::string name = getXmlElementValue(xmlElement, "name");
					std::string cuisine = getXmlElementValue(xmlElement, "cuisine");
					std::string wheelchair = getXmlElementValue(xmlElement, "wheelchair");

					m_puntsInteres.
						push_back(new PuntDeInteresRestaurantSolucio(coordinates, name, cuisine, wheelchair));

					std::cout << "Tipo: " << cuisine << "\nSilla de ruedas: " << wheelchair;
					std::cout << "\n---------------------------\n";
					break;
				}
				else if (keyTag.second == "shop")
				{
					std::cout << "------ TIENDA ------\n";

					Coordinate coordinates;
					coordinates.lat = std::stod(getXmlElementAttribute(xmlElement, "lat"));
					coordinates.lon = std::stod(getXmlElementAttribute(xmlElement, "lon"));

					std::string name = getXmlElementValue(xmlElement, "name");
					std::string shopType = getXmlElementValue(xmlElement, "shop");
					std::string openingHours = getXmlElementValue(xmlElement, "opening_hours");
					std::string wheelchair = getXmlElementValue(xmlElement, "wheelchair");

					m_puntsInteres.push_back(
						new PuntDeInteresBotigaSolucio(
							coordinates,
							name,
							shopType,
							openingHours,
							wheelchair)
					);

					if (wheelchair.empty())
					{
						wheelchair = std::string("no");
					}

					std::cout <<
						"Tipo: " << shopType <<
						"\nHorario: " << openingHours <<
						"\nSilla de ruedas: " << wheelchair;

					std::cout << "\n----------------------\n";

					break;
				}
			}
		}
		else if (isElementPath(xmlElement))
		{
			std::cout << "------ CAMINO ------\n";

			std::vector<std::string> nodeReferences;
			getXmlElementNodeReferences(xmlElement, nodeReferences);

			std::vector<Coordinate> nodeCoordinates(nodeReferences.size());

			for (const std::string& nodeId : nodeReferences)
			{
				nodeCoordinates.push_back(getNodeCoordinatesById(xmlElements, nodeId));
			}

			m_camins.push_back(new CamiSolucio(nodeCoordinates));

			std::cout << "----------------------\n";
		}
	}
}

bool MapaSolucio::isElementPath(const XmlElement& element)
{
	// Comprueba que el XmlElement actual contenga "highway" como clave
	return element.id_element == "way" && !getXmlElementValue(element, "highway").empty();
}

bool MapaSolucio::isElementInterestPoint(const XmlElement& element)
{
	// This checks weather or not it's an actual "interest point" or just a node with nothing else inside
	return element.id_element == "node" && !element.fills.empty();
}

void MapaSolucio::getXmlElementNodeReferences(const XmlElement& xmlElement, std::vector<std::string>& nodeReferences)
{
	for (CHILD_NODE child : xmlElement.fills)
	{
		if (child.first == "nd")
		{
			nodeReferences.push_back(child.second[0].second);
		}
	}
}

std::string MapaSolucio::getXmlElementValue(const XmlElement& xmlElement, const std::string& tagName)
{
	for (CHILD_NODE child : xmlElement.fills)
	{
		if (child.first == "tag")
		{
			const std::pair<std::string, std::string> keyValue = Util::kvDeTag(child.second);
			if (keyValue.first == tagName)
			{
				return keyValue.second;
			}
		}
	}

	return "";
}

std::string MapaSolucio::getXmlElementAttribute(const XmlElement& xmlElement, const std::string& attributeName)
{
	for (const PAIR_ATTR_VALUE child : xmlElement.atributs)
	{
		if (child.first == attributeName)
		{
			return child.second;
		}
	}

	return "";
}

Coordinate MapaSolucio::getNodeCoordinatesById(const std::vector<XmlElement>& xml, const std::string& nodeId)
{
	Coordinate nodeCoordinates = Coordinate{0, 0};
	for (auto& xmlElement : xml)
	{
		std::string currentNodeId = getXmlElementAttribute(xmlElement, "id");
		if (currentNodeId == nodeId)
		{
			nodeCoordinates.lat = std::stod(getXmlElementAttribute(xmlElement, "lat"));
			nodeCoordinates.lon = std::stod(getXmlElementAttribute(xmlElement, "lon"));
			return nodeCoordinates;
		}
	}

	return nodeCoordinates;
}
