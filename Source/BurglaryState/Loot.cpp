#include "../Game.h"
#include "Loot.h"
#include "EntityManager.h"
#include "Player.h"
#include "EntityWorldInterface.h"

using namespace Blackguard::BurglaryState;
using namespace Blackguard;

Loot::Loot()
{
	this->graphics.setTexture(Game::instance->assets.textures["Loot"]);
	this->bounds.size = sf::Vector2f(graphics.getTexture()->getSize());
}

Loot::~Loot()
{
	if(world)
		world->unblockTileAt(position);
}

void Loot::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Loot::move(const sf::Vector2f& pos)
{
	Entity::move(pos);
	graphics.setPosition(position);
}

void Loot::setPosition(const sf::Vector2f& pos)
{
	Entity::setPosition(pos);
	graphics.setPosition(position);
	world->blockTileAt(position);
}

bool Loot::activate(Player& activator)
{
	// TODO: Well this should not give 300 gold everytime...
	activator.addGold(300);
	this->remove();
	return true;
}
