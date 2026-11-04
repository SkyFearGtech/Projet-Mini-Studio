#include "Core/QuadTree.h"


void QuadTree::insert(Entity* _entity, const sf::FloatRect& _bounds)
{
	if (NW != nullptr)
	{
		if (NW->m_bounds.contains(sf::Vector2f(_bounds.left, _bounds.top)) && NW->m_bounds.contains(sf::Vector2f(_bounds.left + _bounds.width, _bounds.top + _bounds.height)))
			NW->insert(_entity, _bounds);
		else if (NE->m_bounds.contains(sf::Vector2f(_bounds.left, _bounds.top)) && NE->m_bounds.contains(sf::Vector2f(_bounds.left + _bounds.width, _bounds.top + _bounds.height)))
			NE->insert(_entity, _bounds);
		else if (SW->m_bounds.contains(sf::Vector2f(_bounds.left, _bounds.top)) && SW->m_bounds.contains(sf::Vector2f(_bounds.left + _bounds.width, _bounds.top + _bounds.height)))
			SW->insert(_entity, _bounds);
		else if (SE->m_bounds.contains(sf::Vector2f(_bounds.left, _bounds.top)) && SE->m_bounds.contains(sf::Vector2f(_bounds.left + _bounds.width, _bounds.top + _bounds.height)))
			SE->insert(_entity, _bounds);
		else
			m_entities.push_back({ _entity, _bounds });
	}
	else
	{
		m_entities.push_back({ _entity, _bounds });
		if (m_entities.size() > MAX_OBJECTS && m_actualDeep < MAX_DEPTH)
		{
			subDivise();
			std::vector<Entry> entities = m_entities;
			m_entities.clear();
			for (Entry entry : entities)
			{
				if (NW->m_bounds.contains(sf::Vector2f(entry.bounds.left, entry.bounds.top)) && NW->m_bounds.contains(sf::Vector2f(entry.bounds.left + entry.bounds.width, entry.bounds.top + entry.bounds.height)))
					NW->insert(entry.entity, entry.bounds);
				else if (NE->m_bounds.contains(sf::Vector2f(entry.bounds.left, entry.bounds.top)) && NE->m_bounds.contains(sf::Vector2f(entry.bounds.left + entry.bounds.width, entry.bounds.top + entry.bounds.height)))
					NE->insert(entry.entity, entry.bounds);
				else if (SW->m_bounds.contains(sf::Vector2f(entry.bounds.left, entry.bounds.top)) && SW->m_bounds.contains(sf::Vector2f(entry.bounds.left + entry.bounds.width, entry.bounds.top + entry.bounds.height)))
					SW->insert(entry.entity, entry.bounds);
				else if (SE->m_bounds.contains(sf::Vector2f(entry.bounds.left, entry.bounds.top)) && SE->m_bounds.contains(sf::Vector2f(entry.bounds.left + entry.bounds.width, entry.bounds.top + entry.bounds.height)))
					SE->insert(entry.entity, entry.bounds);
				else
					m_entities.push_back(entry);
			}
		}
	}
}

std::vector<Entry> QuadTree::query(const sf::FloatRect& _area)
{
	std::vector<Entry> result;
	for (Entry entity : m_entities)
	{
		if (_area.intersects(entity.bounds))
			result.push_back(entity);
	}

	if (NW == nullptr)
		return result;

	if (_area.intersects(NW->m_bounds))
	{
		auto sub = NW->query(_area);
		result.insert(result.end(), sub.begin(), sub.end());
	}
	if (_area.intersects(NE->m_bounds))
	{
		auto sub = NE->query(_area);
		result.insert(result.end(), sub.begin(), sub.end());
	}

	if (_area.intersects(SW->m_bounds))

	{
		auto sub = SW->query(_area);
		result.insert(result.end(), sub.begin(), sub.end());
	}
	if (_area.intersects(SE->m_bounds))
	{
		auto sub = SE->query(_area);
		result.insert(result.end(), sub.begin(), sub.end());
	}

	return result;
}

void QuadTree::clear()
{
	m_entities.clear();
	NE = nullptr;
	NW = nullptr;
	SW = nullptr;
	SE = nullptr;
}

void QuadTree::subDivise()
{
	NW = std::make_unique<QuadTree>(sf::FloatRect(sf::Vector2f(m_bounds.left, m_bounds.top), sf::Vector2f(m_bounds.width / 2, m_bounds.height / 2)), m_actualDeep + 1);

	NE = std::make_unique<QuadTree>(sf::FloatRect(sf::Vector2f(m_bounds.left + m_bounds.width / 2, m_bounds.top), sf::Vector2f(m_bounds.width / 2, m_bounds.height / 2)), m_actualDeep + 1);

	SW = std::make_unique<QuadTree>(sf::FloatRect(sf::Vector2f(m_bounds.left, m_bounds.top + m_bounds.height / 2), sf::Vector2f(m_bounds.width / 2, m_bounds.height / 2)), m_actualDeep + 1);

	SE = std::make_unique<QuadTree>(sf::FloatRect(sf::Vector2f(m_bounds.left + m_bounds.width / 2, m_bounds.top + m_bounds.height / 2), sf::Vector2f(m_bounds.width / 2, m_bounds.height / 2)), m_actualDeep + 1);
}