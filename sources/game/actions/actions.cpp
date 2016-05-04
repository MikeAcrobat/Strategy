#include "actions.h"
#include <algorithm>

Walk::Walk(irr::core::vector3df target) : target(target) {
}

bool Walk::update(Entity::Ptr entity, float dt)
{
	TransformComponent::Ptr transform = entity->get_component<TransformComponent>();
	StatsComponent::Ptr stats = entity->get_component<StatsComponent>();

	if (!transform || !stats || std::abs(stats->move_speed) < 0.1f)
		return true;

	const float distance_threshold = 0.05f;

	float distance = transform->position.getDistanceFrom(target);

	if (distance >= distance_threshold) {
		transform->direction = (target - transform->position).normalize();
		transform->position += transform->direction * std::min(stats->move_speed * dt, distance);
		EntityHelpers::update_transformation(entity);
	}

	return distance < distance_threshold;
}

Collect::Collect(Entity::Ptr resource) : resource(resource), cooldown(0.f)
{
}

bool Collect::update(Entity::Ptr entity, float dt) {
	Entity::Ptr res = resource.lock();

	if (res == nullptr) return true;

	TransformComponent::Ptr res_transform = res->get_component<TransformComponent>();
	ResourceComponent::Ptr res_resource = res->get_component<ResourceComponent>();

	if (!res_transform || !res_resource || res_resource->resource_amount == 0) {
		return true;
	}

	TransformComponent::Ptr ent_transform = entity->get_component<TransformComponent>();
	ResourceComponent::Ptr ent_resource = entity->get_component<ResourceComponent>();
	StatsComponent::Ptr stats = entity->get_component<StatsComponent>();	

	irr::core::vector3df distance = res_transform->position - ent_transform->position;

	if (distance.getLength() > 10) {
		return true;
	}

	if (ent_resource && stats && stats->mine_speed > 0) {
		if (cooldown <= 0) {
			cooldown += 0.2f;
			int resource_to_mine = std::min(ent_resource->resource_cap - ent_resource->resource_amount, std::min(stats->mine_speed, res_resource->resource_amount));
			if (resource_to_mine > 0) {
				res_resource->resource_amount -= resource_to_mine;
				ent_resource->resource_amount += resource_to_mine;
			}
			return resource_to_mine == 0;
		} else {
			cooldown -= dt;
			return false;
		}
	} else {
		return true;
	}
}

bool Deposit::update(Entity::Ptr entity, float dt)
{
	return true;
}
