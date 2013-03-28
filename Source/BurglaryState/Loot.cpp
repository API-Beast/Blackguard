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
	lastPosition = sf::Vector2f(0.f, 0.f);
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

void Loot::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(position);
	if(lastPosition != sf::Vector2f(0.f, 0.f))
		world->unblockTileAt(lastPosition);
	world->blockTileAt(position);
	lastPosition = position;
}

bool Loot::activate(Player& activator)
{
	// TODO: Well this should not give 300 gold everytime...
	activator.addGold(300);
	this->remove();
	return true;
}
