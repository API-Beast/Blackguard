
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

std::map<std::string, std::vector<Guard::ViewPatternPoint> > Guard::possiblePatterns; 

Guard::Guard() : Entity()
{
	graphics = sf::Sprite(Game::instance->assets.textures["Guard"]);
	playerNoticed = sf::Sound(Game::instance->assets.sounds["player_noticed"]);
	sf::Vector2u size = graphics.getTexture()->getSize();
	bounds.offset = sf::Vector2f(size.x/4, size.y*(3/4.f));
	bounds.size = sf::Vector2f(size.x/2, size.y/4);
	aiState = Watching;
	defaultState = Watching;
	viewAngle = 90.f;
	for(float f=0.f-detectionCone; f < 0.f+detectionCone; f+=PI()/80)
		viewcone.push_back(ViewRay(f, detectionDistance));
	
	if(possiblePatterns.empty())
	{
		std::vector<ViewPatternPoint> lightSway;
		lightSway.push_back(ViewPatternPoint(2.f, -PI()/20));
		lightSway.push_back(ViewPatternPoint(4.f, 0.f));
		lightSway.push_back(ViewPatternPoint(2.f, +PI()/20));
		lightSway.push_back(ViewPatternPoint(4.f, 0.f));
		possiblePatterns["LightSway"] = lightSway;
		
		std::vector<ViewPatternPoint> strongSway;
		strongSway.push_back(ViewPatternPoint(4.f, -PI()/7, 0.5f));
		strongSway.push_back(ViewPatternPoint(8.f, 0.f, 0.5f));
		strongSway.push_back(ViewPatternPoint(4.f, +PI()/7, 0.5f));
		strongSway.push_back(ViewPatternPoint(8.f, 0.f, 0.5f));
		possiblePatterns["StrongSway"] = strongSway;
		
		std::vector<ViewPatternPoint> turn;
		turn.push_back(ViewPatternPoint(8.f, 0.f , 1.f));
		turn.push_back(ViewPatternPoint(8.f, PI(), 1.f));
		possiblePatterns["180Turn"] = turn;
		
		std::vector<ViewPatternPoint> ninety;
		ninety.push_back(ViewPatternPoint(8.f, 0.f   , 0.7f));
		ninety.push_back(ViewPatternPoint(8.f, PI()/2, 0.7f));
		possiblePatterns["90Turn"] = ninety;
		
		std::vector<ViewPatternPoint> minusNinety;
		ninety.push_back(ViewPatternPoint(8.f,  0.f   , 0.7f));
		ninety.push_back(ViewPatternPoint(8.f, -PI()/2, 0.7f));
		possiblePatterns["-90Turn"] = minusNinety;
	}
	this->viewPattern = possiblePatterns["LightSway"];

	curViewpatternIndex = 0;
	curViewPatternTime  = 0.f;
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

void Guard::initializeFromTileObject(const TileObject& object)
{
	TileObject obj = object;
	this->home = sf::Vector2f(obj.x, obj.y);
	if(obj.properties["behaviour"] == "Roaming")
	{
		this->defaultState = Roaming;
		this->aiState = Roaming;
	}
	this->viewAngle = obj.rotation / 180.f * PI();
	std::vector<Guard::ViewPatternPoint> pattern = possiblePatterns[obj.properties["viewPattern"]];
	if(!pattern.empty())
		viewPattern = pattern;
	for(ViewPatternPoint& point : viewPattern)
	{
		point.targetAngle += this->viewAngle;
	}
}

void Guard::update(float deltaTime)
{
	updateViewcone();
	aiRoutine(deltaTime);
}

void Guard::updateViewcone()
{
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
	for(int i=elements/2-elements/6; i<(elements/2+elements/6); i++)
		middleViewDistance   = std::min(viewcone[i].obstructedRange, middleViewDistance);
	for(int i=elements-elements/3; i<elements; i++)
		farRightViewDistance += viewcone[i].obstructedRange;
	
	farLeftViewDistance  /= elements/3;
	farRightViewDistance /= elements/3;
}


// AI stands for Artificial "Intelligence"
// Watch those " " they are important.
void Guard::aiRoutine(float deltaTime)
{
	const float runningSpeed=100.f;
	const float walkingSpeed= 50.f;
	
	float movingSpeed=0.f;
	sf::Vector2f movingDirection(0.f, 0.f);
	
	if(aiState != ChasingInView)
	{
		std::vector<Entity*> players=world->getEntitiesByType("Player");
		for(Entity* player : players)
		{
			if(!isInView(player))
				continue;
			playerNoticed.play();
			aiState = ChasingInView;
			currentTarget = player;
			break;
		}
	}
	
	if(aiState == Watching)
	{
		ViewPatternPoint& point=viewPattern[curViewpatternIndex];
		curViewPatternTime+=deltaTime;
		if(curViewPatternTime >= point.time)
		{
			curViewPatternTime = 0.f;
			curViewpatternIndex++;
			if(curViewpatternIndex >= viewPattern.size())
				curViewpatternIndex = 0;
		}
		
		float difference=std::abs(point.targetAngle - viewAngle);
		if(difference > 0.01f)
		{
			float change=std::min(float(PI())*curViewPatternTime*point.movementSpeed*deltaTime*difference*3.f, difference);
			if(point.targetAngle > viewAngle)
				viewAngle += change;
			else if(point.targetAngle < viewAngle)
				viewAngle -= change;
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
			sf::Vector2f targetMovement = currentTarget->getMovement()/deltaTime;
			projectedTargetPosition = currentTargetPosition + targetMovement*0.2f;
			movingSpeed = runningSpeed;
			viewAngle = Angle(getCenter(), currentTargetPosition);
			// Try to intercept if possible
			if(world->raycast(getCenter(), projectedTargetPosition).obstructed == false)
				movingDirection = VectorNormalize(projectedTargetPosition-this->getCenter());
			else
				movingDirection = VectorNormalize(currentTargetPosition-this->getCenter());
		}
	}
	if(aiState == Investigating || aiState == ChasingOutOfView)
	{
		movingSpeed = walkingSpeed;
		if(aiState == ChasingOutOfView)
		{
			movingSpeed = runningSpeed;
		}
		
	}
	if(aiState == Panicked || aiState == Roaming)
	{
		movingDirection = AngleToVector(viewAngle);
		movingSpeed = walkingSpeed;
		if(aiState == Panicked)
			movingSpeed = runningSpeed;
		
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
	}
	
	//
	
	if(movingSpeed > 1.f)
	{
		this->move(movingDirection*movingSpeed*deltaTime);
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