
#include "Guard.h"

#include "SFML/Graphics.hpp"
#include "../GameData.h"
#include "../Game.h"
#include "Player.h"
#include "../TileMap.h"
#include "../Utility/GameMath.h"
#include "EntityWorldInterface.h"
#include "../PathFinder.h"
#include <iostream>

using namespace Blackguard;
using namespace Blackguard::BurglaryState;
using namespace Blackguard::Utility;

constexpr float detectionCone     = Blackguard::Utility::PI() / 6.f;
constexpr float detectionDistance = 200.f;

Guard::Guard() : Entity()
{
	graphics = sf::Sprite(Game::instance->assets.textures["Guard"]);
	sf::Vector2u size = graphics.getTexture()->getSize();
	bounds.offset = sf::Vector2f(size.x/4, size.y*(2/3.f));
	bounds.size = sf::Vector2f(size.x/2, size.y/3);
	aiState = Watching;
	viewAngle = 0.f;
	for(float f=0.f-detectionCone; f < 0.f+detectionCone; f+=PI()/20)
		viewcone.push_back(f);
}

Guard::~Guard()
{
}

bool Guard::isInView(Entity* obj)
{
	float angle    =    Angle(this->getCenter(), obj->getCenter());
	float distance = Distance(this->getCenter(), obj->getCenter());
	if(distance > detectionDistance)
		return false;
	if(!InsideAngle(angle, viewAngle-detectionCone, viewAngle+detectionCone))
		return false;
	if(world->raycast(this->getCenter(), obj->getCenter()).obstructed)
		return false;
	return true;
}

void Guard::update(float deltaTime)
{
	Entity::update(deltaTime);
	
	// Update viewcone
	static constexpr float deadEndDistance=100.f;
	viewsDeadEnd = true;
	for(ViewRay& viewray : viewcone)
	{
		float angle = viewAngle + viewray.angle;
		sf::Vector2f start = this->getCenter();
		sf::Vector2f end   = start + AngleToVector(angle)*viewray.range;
		RaycastResult raycast = world->raycast(start, end);
		viewray.isObstructed = raycast.obstructed;
		viewray.obstructedRange = VectorLength(raycast.ray);
		if(viewray.obstructedRange > deadEndDistance)
			viewsDeadEnd = false;
	}
	// fucking ugly, I know
	int elements = viewcone.size();
	farLeftViewDistance  = viewcone[0].obstructedRange + viewcone[1].obstructedRange;
	farRightViewDistance = viewcone[elements-1].obstructedRange + viewcone[elements-2].obstructedRange;
	middleViewDistance   = viewcone[elements/2].obstructedRange + viewcone[elements/2-1].obstructedRange + viewcone[elements/2+1].obstructedRange;
	farLeftViewDistance  += VectorLength(world->raycast(getCenter(), getCenter() + AngleToVector(-detectionCone*1.1f)).ray);
	farRightViewDistance += VectorLength(world->raycast(getCenter(), getCenter() + AngleToVector(+detectionCone*1.1f)).ray);
	
	//
	// Artificial "Intelligence"
	//
	static const float movementSpeed=100.f;
	if(aiState == Watching || aiState == ChasingOutOfView)
	{
		std::vector<Entity*> players=world->getEntitiesByType("Player");
		for(Entity* player : players)
		{
			if(!isInView(player))
				continue;
			aiState = ChasingInView;
			currentTarget = player;
			break;
		}
	}
	
	if(aiState == ChasingInView)
	{
		if(!isInView(currentTarget))
		{
			aiState = ChasingOutOfView;
		}
		else
		{
			currentTargetPosition = currentTarget->getCenter();
			targetMovement = currentTarget->getMovement()/deltaTime;
			projectedTargetPosition = currentTargetPosition + targetMovement*0.2f;
			if(world->raycast(getCenter(), projectedTargetPosition).obstructed)
			{
				viewAngle = Angle(getCenter(), currentTargetPosition);
				this->move(VectorNormalize(currentTargetPosition-this->getCenter())*movementSpeed*deltaTime);
			}
			else
			{
				viewAngle = Angle(getCenter(), projectedTargetPosition);
				this->move(VectorNormalize(projectedTargetPosition-this->getCenter())*movementSpeed*deltaTime);
			}
		}
	}
	if(aiState == ChasingOutOfView)
	{
		if(farLeftViewDistance > middleViewDistance || farRightViewDistance > middleViewDistance)
		{
			if(farLeftViewDistance > farRightViewDistance)
				viewAngle -= PI()*deltaTime;
			else
				viewAngle += PI()*deltaTime;
		}
		if(viewsDeadEnd)
			viewAngle += PI()*4*deltaTime;
		viewAngle = NormalizeAngle(viewAngle);
		move(AngleToVector(viewAngle)*movementSpeed*deltaTime);
	}
}

void Guard::onNoise(sf::Vector2f source)
{
	// TODO
}

void Guard::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
	sf::ConvexShape viewconeShape;
	viewconeShape.setPosition(this->getCenter());
	viewconeShape.setPointCount(viewcone.size()+1);
	viewconeShape.setPoint(0, sf::Vector2f(0.f, 0.f));
	for(unsigned int i=0; i < viewcone.size(); i++)
	{
		sf::Vector2f point = AngleToVector(viewAngle + viewcone[i].angle) * viewcone[i].obstructedRange;
		viewconeShape.setPoint(i+1, point);
	}
	viewconeShape.setFillColor(sf::Color(255, 255, 255, 50));
	viewconeShape.setRotation(this->viewAngle);
	target->draw(viewconeShape);
}

void Guard::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(position);
}
