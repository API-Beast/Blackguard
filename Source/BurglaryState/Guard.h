
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
				Panicked          // The guard has seen & chased the player but doesn't have a clue where he is anymore.
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
		private:
			bool isInView(Blackguard::BurglaryState::Entity* obj);
		private:
			sf::Sprite graphics;
			//std::stack<sf::Vector2f> waypoints;
			sf::Vector2f currentTargetPosition;
			sf::Vector2f projectedTargetPosition;
			sf::Vector2f targetMovement;
			std::vector<ViewRay> viewcone;
			bool viewsDeadEnd;
			float middleViewDistance;
			float farLeftViewDistance;
			float farRightViewDistance;
			Entity* currentTarget;
			float viewAngle;
			AIState aiState;
		};
	}
}

#endif // GRG_GUARD_H