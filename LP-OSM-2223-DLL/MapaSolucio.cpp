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
	interestPoints.clear();
	interestPoints.reserve(m_interestPoints.size());
	auto it = m_interestPoints.begin();
	for (; it != m_interestPoints.end(); it++)
		interestPoints.push_back(*it);
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& paths)
{
	paths.clear();
	paths.reserve(m_paths.size());
	auto it = m_paths.begin();
	for (; it != m_paths.end(); it++)
		paths.push_back(*it);
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	m_interestPoints.clear();
	m_emptyNodes.clear();
	m_pathNodes.clear();
	m_paths.clear();

	for (const XmlElement& element : xmlElements)
	{
		NodeType nodeType = getNodeType(element);

		switch (nodeType)
		{
		case NOT_APPLICABLE:
			break;

		case INTEREST_POINT:
			m_interestPoints.push_back(parseInterestPoint(element));
			break;

		case PATH_NO_TAG:
		case PATH_NO_NAME:
			m_emptyNodes.push_back(parseEmptyNode(element));
			break;

		case PATH_HIGHWAY:
			m_pathNodes.push_back(parsePathNode(element));
			break;
		}
	}

	for (const auto& path : m_pathNodes)
		m_paths.push_back(parsePath(path));
}

MapaSolucio::NodeType MapaSolucio::getNodeType(const XmlElement& element)
{
	NodeType nodeType = NodeType::NOT_APPLICABLE;
	const bool isValidElement = element.id_element == "node" || element.id_element == "way";
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

MapaSolucio::EmptyNode MapaSolucio::parseEmptyNode(const XmlElement& element)
{
	EmptyNode node{0, Coordinate{0, 0}};

	node.id = std::atoll(getElementAttributeValue(element, "id").c_str());
	node.coordinates.lat = std::stod(getElementAttributeValue(element, "lat"));
	node.coordinates.lon = std::stod(getElementAttributeValue(element, "lon"));

	return node;
}

MapaSolucio::PathNode MapaSolucio::parsePathNode(const XmlElement& element)
{
	PathNode node{{}};
	node.nodeRef = getElementNodeReferences(element);
	return node;
}

CamiBase* MapaSolucio::parsePath(const PathNode& pathNode)
{
	std::vector<Coordinate> pathCoordinates(0);
	for (const long long& nodeId : pathNode.nodeRef)
		pathCoordinates.push_back(getCoordinateById(nodeId));

	return new CamiSolucio(pathCoordinates);
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

std::vector<long long> MapaSolucio::getElementNodeReferences(const XmlElement& element)
{
	std::vector<long long> references(0);
	for (const CHILD_NODE& child : element.fills)
	{
		if (child.first == "nd")
			references.push_back(std::atoll(child.second[0].second.c_str()));
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

Coordinate MapaSolucio::getCoordinateById(const long long& nodeId)
{
	Coordinate nodeCoordinates = Coordinate{0, 0};

	for (const EmptyNode& node : m_emptyNodes)
	{
		if (node.id == nodeId)
			nodeCoordinates = node.coordinates;
	}

	return nodeCoordinates;
}
