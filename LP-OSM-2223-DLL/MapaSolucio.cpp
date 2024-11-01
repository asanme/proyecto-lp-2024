#include "pch.h"
#include "MapaSolucio.h"

#include <algorithm>

#include "Util.h"
#include "CamiSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

#include <iostream>

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& interestPoints)
{
	for (PuntDeInteresBase* interestPoint : m_interestPoints)
		interestPoints.push_back(interestPoint);
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& paths)
{
	for (CamiBase* path : m_paths)
		paths.push_back(path);
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	for (const XmlElement& element : xmlElements)
	{
		NodeType nodeType = getNodeType(element);

		switch (nodeType)
		{
		case INTEREST_POINT:
			m_interestPoints.push_back(parseInterestPoint(element));
			break;

		case PATH_NO_TAG:
			break;

		case PATH_NO_NAME:
			break;

		case PATH_HIGHWAY:
			break;

		default:
			break;
		}
	}
}

MapaSolucio::NodeType MapaSolucio::getNodeType(const XmlElement& element)
{
	NodeType nodeType = NodeType::NOT_APPLICABLE;
	bool isValidElement = element.id_element == "node" || element.id_element == "way";
	if (!isValidElement)
		return nodeType;

	if (element.id_element == "way" && getElementChildValue(element, "highway").empty())
		return nodeType;

	nodeType = NodeType::INTEREST_POINT;
	if (isElementInterestPoint(element))
		return nodeType;

	nodeType = NodeType::PATH_NO_TAG;
	if (!elementContainsTag(element))
		return nodeType;

	nodeType = NodeType::PATH_NO_NAME;
	if (getElementChildValue(element, "name").empty())
		return nodeType;

	nodeType = NodeType::PATH_HIGHWAY;
	return nodeType;
}

PuntDeInteresBase* MapaSolucio::parseInterestPoint(const XmlElement& element)
{
	bool isShop = !getElementChildValue(element, "shop").empty();
	PuntDeInteresBase* interestPoint;

	if (isShop)
		interestPoint = parseShop(element);
	else
		interestPoint = parseRestaurant(element);

	return interestPoint;
}

PuntDeInteresBase* MapaSolucio::parseShop(const XmlElement& element)
{
	Coordinate coordinates;
	coordinates.lat = std::stod(getElementAttributeValue(element, "lat"));
	coordinates.lon = std::stod(getElementAttributeValue(element, "lon"));

	std::string name = getElementChildValue(element, "name");
	std::string shopType = getElementChildValue(element, "shop");
	std::string openingHours = getElementChildValue(element, "opening_hours");
	std::string wheelchair = getElementChildValue(element, "wheelchair");

	if (wheelchair.empty())
		wheelchair = std::string("no");

	return new PuntDeInteresBotigaSolucio(coordinates, name, shopType, openingHours, wheelchair);
}

PuntDeInteresBase* MapaSolucio::parseRestaurant(const XmlElement& element)
{
	Coordinate coordinates;
	coordinates.lat = std::stod(getElementAttributeValue(element, "lat"));
	coordinates.lon = std::stod(getElementAttributeValue(element, "lon"));

	std::string name = getElementChildValue(element, "name");
	std::string cuisine = getElementChildValue(element, "cuisine");
	std::string wheelchair = getElementChildValue(element, "wheelchair");

	if (wheelchair.empty())
		wheelchair = std::string("no");

	return new PuntDeInteresRestaurantSolucio(coordinates, name, cuisine, wheelchair);
}

void MapaSolucio::parsePath(std::vector<XmlElement>& xmlElements, const XmlElement& element)
{
	std::vector<std::string> nodeReferences = getElementNodeReferences(element);
	std::vector<Coordinate> nodeCoordinates(nodeReferences.size());

	for (const std::string& nodeId : nodeReferences)
	{
		Coordinate cords = getNodeCoordinatesById(xmlElements, nodeId);
		nodeCoordinates.push_back(cords);
	}

	m_paths.push_back(new CamiSolucio(nodeCoordinates));
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

bool MapaSolucio::elementContainsTag(const XmlElement& element)
{
	for (const CHILD_NODE& child : element.fills)
	{
		if (child.first == "tag")
			return true;
	}

	return false;
}

std::vector<std::string> MapaSolucio::getElementNodeReferences(const XmlElement& element)
{
	std::vector<std::string> references(0);
	for (const CHILD_NODE& child : element.fills)
	{
		if (child.first == "nd")
			references.push_back(child.second[0].second);
	}

	return references;
}

std::string MapaSolucio::getElementChildValue(const XmlElement& element, const std::string& keyName)
{
	for (CHILD_NODE child : element.fills)
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

std::string MapaSolucio::getElementAttributeValue(const XmlElement& element, const std::string& attributeName)
{
	for (const PAIR_ATTR_VALUE& child : element.atributs)
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
