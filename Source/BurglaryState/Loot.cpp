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
	collect = sf::Sound(Game::instance->assets.sounds["collect_loot"]);
	collect.setPitch(1.f);
	this->bounds.size = sf::Vector2f(32,32);
	lastPosition = sf::Vector2f(0.f, 0.f);
	collected = false;
}

void Loot::finalize()
{
	world->addGoal();
}

Loot::~Loot()
{
	if(world)
	{
		world->unblockTileAt(position);
		//if(!collected)
		//	world->markGoalAsReached();
	}
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
	if(!collected)
	{
		collected = true;
		collect.play();
		graphics.setTexture(Game::instance->assets.textures["Loot/Open"]);
		world->markGoalAsReached();
	}
	return true;
}

