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
	{
		puntsInteres.push_back(puntInteres);
	}
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

bool MapaSolucio::isElementPath(const XmlElement& element)
{
	bool containsHighway = !getElementChildValue(element, "highway").empty();
	bool containsChildren = !element.fills.empty();

	return element.id_element == "way" && containsHighway && containsChildren;
}

bool MapaSolucio::isElementInterestPoint(const XmlElement& element)
{
	bool containsName = !getElementChildValue(element, "name").empty();
	bool containsHighway = !getElementChildValue(element, "highway").empty();
	bool containsPublicTransport = !getElementChildValue(element, "public_transport").empty();
	bool containsAccess = !getElementChildValue(element, "access").empty();
	bool containsEntrance = !getElementChildValue(element, "entrance").empty();

	return (element.id_element == "node" && containsName) &&
		(!containsHighway && !containsPublicTransport && !containsAccess && !containsEntrance);
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
				Coordinate coordinates;
				coordinates.lat = std::stod(getElementAttributeValue(xmlElement, "lat"));
				coordinates.lon = std::stod(getElementAttributeValue(xmlElement, "lon"));

				std::string name = getElementChildValue(xmlElement, "name");
				std::string cuisine = getElementChildValue(xmlElement, "cuisine");
				std::string wheelchair = getElementChildValue(xmlElement, "wheelchair");

				m_puntsInteres.push_back(
					new PuntDeInteresRestaurantSolucio(coordinates, name, cuisine, wheelchair)
				);
				break;
			}
			else if (isShop)
			{
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

				break;
			}
		}
	}
}

void MapaSolucio::parsePath(std::vector<XmlElement>& xmlElements, const XmlElement& xmlElement)
{
	std::vector<std::string> nodeReferences = getElementNodeReferences(xmlElement);
	std::vector<Coordinate> nodeCoordinates(nodeReferences.size());

	for (const std::string& nodeId : nodeReferences)
	{
		Coordinate cords = getNodeCoordinatesById(xmlElements, nodeId);
		nodeCoordinates.push_back(cords);
	}

	m_camins.push_back(new CamiSolucio(nodeCoordinates));
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
