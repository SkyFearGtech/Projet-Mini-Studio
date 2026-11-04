#pragma once
#include <vector>
#include <memory>

#include <SFML/Graphics/Rect.hpp>

#include "Core/GameManager.h"
#include "Core/Entity.h"

struct Entry
{
	Entity* entity;
	sf::FloatRect bounds;
};

class QuadTree
{

private:
	static constexpr int MAX_OBJECTS = 8;
	static constexpr int MAX_DEPTH = 5;

	sf::FloatRect m_bounds;
	int m_actualDeep;
	std::vector<Entry> m_entities;

	std::unique_ptr<QuadTree> NW;
	std::unique_ptr<QuadTree> NE;
	std::unique_ptr<QuadTree> SW;
	std::unique_ptr<QuadTree> SE;

public:
	QuadTree(sf::FloatRect _bounds, int _deep) : m_bounds(_bounds), m_actualDeep(_deep) {}

	void insert(Entity* _entity, const sf::FloatRect& _bounds);
	std::vector<Entry> query(const sf::FloatRect& _area);
	void clear();

private:
	void subDivise();
};
