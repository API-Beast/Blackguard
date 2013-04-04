
#ifndef GRG_GUARD_H
#define GRG_GUARD_H

#include "Entity.h"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include <stack>
#include "../Utility/GameMath.h"

namespace sf
{
	class Texture;
}

namespace Blackguard
{
	struct GameData;
	class TileMap;

	namespace BurglaryState
	{
		class Guard : public Entity
		{
		public:
			enum AIState
			{
				Watching,         // The guard is just idling.
				ChasingInView,    // The guard is chasing it's prey, a tasty player.
				ChasingOutOfView, // The guard doesn't see the player anymore but has a idea where he could be.
				Investigating,    // The guard is investigating a noise.
				Roaming,          // 
				Panicked,         // The guard has seen & chased the player but doesn't have a clue where he is anymore.
				MoveHome          // 
			};
			struct ViewRay
			{
				float angle;
				float range;
				float obstructedRange;
				bool isObstructed;
				ViewRay(float angle, float range)
				{
					this->angle = angle;
					this->range = range;
					this->obstructedRange = range;
					this->isObstructed = false;
				};
			};
			struct ViewPatternPoint
			{
				float time;
				float targetAngle;
				float movementSpeed;
				ViewPatternPoint(float time, float targetAngle, float movementSpeed=0.333f)
				{
					this->time = time;
					this->targetAngle = targetAngle;
					this->movementSpeed = movementSpeed;
				};
			};
		public:
			Guard();
			~Guard();
			virtual void update(float deltaTime);
			virtual void draw(sf::RenderTarget* target) const;
			virtual void drawBackground(sf::RenderTarget* target) const;
			virtual std::string getType(){ return "Guard"; };
			virtual bool activate(Player& activator);
		protected:
			virtual void updatePosition();
			virtual void initializeFromTileObject(const TileObject&);
		private:
			void aiRoutine(float deltaTime);
			void changeAiState(AIState newState);
			void updateViewcone();
			bool isInView(Blackguard::BurglaryState::Entity* obj);
		private:
			sf::Sprite graphics;
			sf::Vector2f currentTargetPosition;
			sf::Vector2f projectedTargetPosition;
			std::vector<ViewRay> viewcone;
			bool viewsDeadEnd;
			float middleViewDistance;
			float farLeftViewDistance;
			float farRightViewDistance;
			Entity* currentTarget;
			float viewAngle;
			
			AIState aiState;
			float currentStateTime;
			
			AIState defaultState;
			sf::Vector2f home;
			
			std::vector<ViewPatternPoint> viewPattern;
			int curViewpatternIndex;
			float curViewPatternTime;
			
			static std::map<std::string, std::vector<ViewPatternPoint> > possiblePatterns;
		};
	}
}

#endif // GRG_GUARD_H