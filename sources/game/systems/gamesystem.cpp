#include "gamesystem.h"

void MovementProcess::update(std::vector<Entity::Ptr>& entities, float dt) {
	for (Entity::Ptr & entity : entities) {
		TransformComponent::Ptr transformation = entity->get_component<TransformComponent>();
		MovementComponent::Ptr movement = entity->get_component<MovementComponent>();
		RenderComponent::Ptr render = entity->get_component<RenderComponent>();

		if (movement->state == MovementComponent::ON_MOVE) {
			irr::core::vector3df direction = (movement->destination - transformation->position).normalize();
			transformation->position += direction * movement->speed * dt;
			transformation->direction = direction;

			if (transformation->position.getDistanceFromSQ(movement->destination) < 1.f) {
				movement->state = MovementComponent::STAND;
			}
		}

		if (render && render->node) {
			render->node->setPosition(transformation->position);
			render->node->setRotation(transformation->direction.getHorizontalAngle());
		}
	}
}

ProductionProcess::ProductionProcess(std::vector<Entity::Ptr>& storage, EntityFactory & factory) : m_factory(factory), m_storage(storage)
{
}

void ProductionProcess::update(std::vector<Entity::Ptr>& entities, float dt) {
	for (Entity::Ptr & entity : entities) {
		ProductionComponent::Ptr production = entity->get_component<ProductionComponent>();
		if (production && production->in_progress) {
			production->time_remaning -= dt;
			if (production->time_remaning <= 0) {
				production->time_remaning = 0.f;
				production->in_progress = false;
				// make new object
				Entity::Ptr result_entity = m_factory.create_object(production->produce_result);
				MovementComponent::Ptr movement = result_entity->get_component<MovementComponent>();
				if (movement) {
					movement->state = MovementComponent::ON_MOVE;
					movement->destination = production->production_point;
					TransformComponent::Ptr transform = entity->get_component<TransformComponent>();
					EntityHelpers::set_position(result_entity, transform->position);
				} else {
					EntityHelpers::set_position(result_entity, production->production_point);
				}

				m_storage.push_back(result_entity);
			}
		}
	}
}
