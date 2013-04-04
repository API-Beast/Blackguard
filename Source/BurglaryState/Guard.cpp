
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

const float detectionCone     = Blackguard::Utility::PI() / 4.f;
const float detectionDistance = 250.f;

Guard::Guard() : Entity()
{
	graphics = sf::Sprite(Game::instance->assets.textures["Guard"]);
	sf::Vector2u size = graphics.getTexture()->getSize();
	bounds.offset = sf::Vector2f(size.x/4, size.y*(3/4.f));
	bounds.size = sf::Vector2f(size.x/2, size.y/4);
	aiState = Watching;
	viewAngle = 0.f;
	for(float f=0.f-detectionCone; f < 0.f+detectionCone; f+=PI()/80)
		viewcone.push_back(ViewRay(f, detectionDistance));
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
	static const float deadEndDistance=100.f;
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
	farLeftViewDistance = farRightViewDistance = middleViewDistance = 0;
	int elements = viewcone.size();
	for(int i=0; i<elements/3; i++)
		farLeftViewDistance  += viewcone[i].obstructedRange;
	for(int i=elements/2-elements/4; i<(elements/2+elements/4); i++)
		middleViewDistance   += viewcone[i].obstructedRange;
	for(int i=elements-elements/3; i<elements; i++)
		farRightViewDistance += viewcone[i].obstructedRange;
	
	farLeftViewDistance  /= elements/3;
	middleViewDistance   /= elements/4;
	farRightViewDistance /= elements/3;
	
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
			if(Distance(this->getCenter(), currentTarget->getCenter()) < 10.f)
			{
				if(currentTarget->getType() == "Player")
				{
					world->onPlayerWasCatched();
				}
			}
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

void Guard::draw(sf::RenderTarget* target) const
{
	target->draw(graphics);
}

void Guard::drawBackground(sf::RenderTarget* target) const
{
	target->pushGLStates();
	sf::VertexArray viewconeShape(sf::TrianglesStrip, viewcone.size()*2);
	sf::Color color(0, 0, 0, 25);
	for(unsigned int i=0; i < viewcone.size(); i++)
	{
		sf::Vector2f pointA = AngleToVector(viewAngle + viewcone[i].angle) * 10.f;
		sf::Vector2f pointB = AngleToVector(viewAngle + viewcone[i].angle) * viewcone[i].obstructedRange;
		//sf::Vector2f pointC = AngleToVector(viewAngle + viewcone[i+1].angle) * 10.f;
		//sf::Vector2f pointD = AngleToVector(viewAngle + viewcone[i+1].angle) * viewcone[i].obstructedRange;
		viewconeShape[i*2  ] = sf::Vertex(getCenter() + pointA, color);
		viewconeShape[i*2+1] = sf::Vertex(getCenter() + pointB, color);
		//viewconeShape[i*4+3] = sf::Vertex(getCenter() + pointC, color);
		//viewconeShape[i*4+2] = sf::Vertex(getCenter() + pointD, color);
	}
	target->draw(viewconeShape);
	target->popGLStates();
}

void Guard::updatePosition()
{
	Blackguard::BurglaryState::Entity::updatePosition();
	graphics.setPosition(position);
}

bool Guard::activate(Player& activator)
{
	this->remove();
	// TODO Leave unconscious guard
	return true;
}