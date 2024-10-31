#include "pch.h"
#include "MapaSolucio.h"
#include "Util.h"

#include <iostream>

#include "CamiSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& puntsInteres)
{
	for (PuntDeInteresBase* puntInteres : m_puntsInteres)
		puntsInteres.push_back(puntInteres);
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& camins)
{
	for (CamiBase* cami : m_camins)
		camins.push_back(cami);
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	for (const XmlElement& xmlElement : xmlElements)
	{
		if (isElementInterestPoint(xmlElement))
			parseInterestPoint(xmlElement);
		else if (isElementPath(xmlElement))
			parsePath(xmlElements, xmlElement);
	}
}

// Comprueba que el XmlElement actual contenga "highway" como clave
bool MapaSolucio::isElementPath(const XmlElement& element)
{
	return element.id_element == "way" && !getElementChildValue(element, "highway").empty();
}

// TODO Comprobar que este check no es bypasseado por los nodos vacios
// Comprueba que el elemento tenga hijos y que sea de tipo nodo
bool MapaSolucio::isElementInterestPoint(const XmlElement& element)
{
	return element.id_element == "node" && !element.fills.empty();
}

void MapaSolucio::parseInterestPoint(const XmlElement& xmlElement)
{
	for (const CHILD_NODE& child : xmlElement.fills)
	{
		// TODO Improve the performance of this method by removing unnecessary checks
		// Check that the current child is the tag
		if (child.first == "tag")
		{
			const std::vector<std::pair<std::string, std::string>>& currentTag = child.second;

			const std::pair<std::string, std::string>& keyTag = currentTag[0];
			const std::pair<std::string, std::string>& valueTag = currentTag[1];

			bool isRestaurant = keyTag.second == "amenity" && valueTag.second == "restaurant";
			bool isShop = keyTag.second == "shop";

			if (isRestaurant)
			{
				// std::cout << "------ RESTAURANTE ------\n";

				Coordinate coordinates;
				coordinates.lat = std::stod(getElementAttributeValue(xmlElement, "lat"));
				coordinates.lon = std::stod(getElementAttributeValue(xmlElement, "lon"));

				std::string name = getElementChildValue(xmlElement, "name");
				std::string cuisine = getElementChildValue(xmlElement, "cuisine");
				std::string wheelchair = getElementChildValue(xmlElement, "wheelchair");

				m_puntsInteres.push_back(new PuntDeInteresRestaurantSolucio(coordinates, name, cuisine, wheelchair));

				// std::cout << "Tipo: " << cuisine << "\nSilla de ruedas: " << wheelchair;
				// std::cout << "\n---------------------------\n";
				break;
			}
			else if (isShop)
			{
				// std::cout << "------ TIENDA ------\n";

				Coordinate coordinates;
				coordinates.lat = std::stod(getElementAttributeValue(xmlElement, "lat"));
				coordinates.lon = std::stod(getElementAttributeValue(xmlElement, "lon"));

				std::string name = getElementChildValue(xmlElement, "name");
				std::string shopType = getElementChildValue(xmlElement, "shop");
				std::string openingHours = getElementChildValue(xmlElement, "opening_hours");
				std::string wheelchair = getElementChildValue(xmlElement, "wheelchair");

				if (wheelchair.empty())
					wheelchair = std::string("no");

				m_puntsInteres.push_back(
					new PuntDeInteresBotigaSolucio(coordinates, name, shopType, openingHours, wheelchair)
				);


				// std::cout <<
				// 	"Tipo: " << shopType <<
				// 	"\nHorario: " << openingHours <<
				// 	"\nSilla de ruedas: " << wheelchair;
				//
				// std::cout << "\n----------------------\n";

				break;
			}
		}
	}
}

void MapaSolucio::parsePath(std::vector<XmlElement>& xmlElements, const XmlElement& xmlElement)
{
	// std::cout << "------ CAMINO ------\n";

	std::vector<std::string> nodeReferences = getElementNodeReferences(xmlElement);
	std::vector<Coordinate> nodeCoordinates(nodeReferences.size());

	for (const std::string& nodeId : nodeReferences)
		nodeCoordinates.push_back(getNodeCoordinatesById(xmlElements, nodeId));

	m_camins.push_back(new CamiSolucio(nodeCoordinates));

	// std::cout << "----------------------\n";
}

std::vector<std::string> MapaSolucio::getElementNodeReferences(const XmlElement& xmlElement)
{
	std::vector<std::string> references(0);
	for (const CHILD_NODE& child : xmlElement.fills)
	{
		if (child.first == "nd")
			references.push_back(child.second[0].second);
	}

	return references;
}

std::string MapaSolucio::getElementChildValue(const XmlElement& xmlElement, const std::string& keyName)
{
	for (CHILD_NODE child : xmlElement.fills)
	{
		if (child.first == "tag")
		{
			const std::pair<std::string, std::string>& keyValue = Util::kvDeTag(child.second);
			if (keyValue.first == keyName)
				return keyValue.second;
		}
	}

	return "";
}

std::string MapaSolucio::getElementAttributeValue(const XmlElement& xmlElement, const std::string& attributeName)
{
	for (const PAIR_ATTR_VALUE& child : xmlElement.atributs)
	{
		if (child.first == attributeName)
			return child.second;
	}

	return "";
}

Coordinate MapaSolucio::getNodeCoordinatesById(const std::vector<XmlElement>& xml, const std::string& nodeId)
{
	Coordinate nodeCoordinates = Coordinate{0, 0};
	for (const XmlElement& xmlElement : xml)
	{
		std::string currentNodeId = getElementAttributeValue(xmlElement, "id");
		if (currentNodeId == nodeId)
		{
			nodeCoordinates.lat = std::stod(getElementAttributeValue(xmlElement, "lat"));
			nodeCoordinates.lon = std::stod(getElementAttributeValue(xmlElement, "lon"));
			return nodeCoordinates;
		}
	}

	return nodeCoordinates;
}
