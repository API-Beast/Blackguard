
#ifndef GRG_LOOT_H
#define GRG_LOOT_H

#include "Entity.h"

namespace Blackguard
{
	class Map;

	class Loot : public Entity
	{
	public:
		Loot(sf::Texture& texture, Map* map);
		virtual void draw(sf::RenderTarget* target);
		virtual bool isCollideEnabled() { return true; }
		virtual void move(sf::Vector2f pos);
		virtual void setPosition(sf::Vector2f pos);
		virtual void activate(EntityPtr activator);
	private:
		sf::Sprite graphics;
		Map* map;
	};
}

#endif