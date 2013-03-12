#include "Loot.h"
#include "Map.h"
#include "Player.h"

using namespace Blackguard;

Loot::Loot(sf::Texture& texture, Map* map)
{
	this->graphics = sf::Sprite(texture);
	this->map = map;
	this->aabb.size = graphics.getTexture()->getSize();
}

void Loot::draw(sf::RenderTarget* target)
{
	target->draw(graphics);
}

void Loot::move(sf::Vector2f pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Loot::setPosition(sf::Vector2f pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
}

void Loot::activate(EntityPtr activator)
{
	if(activator->getType() == EntityType::Player) {
		PlayerPtr player = std::dynamic_pointer_cast<Player>(activator);
		// TODO: Well this should not give 300 gold everytime...
		player->addGold(300);
		map->remove(this->ownID);
	}
}