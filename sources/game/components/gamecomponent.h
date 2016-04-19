#pragma once

#include "irrlicht.h"
#include <memory>
#include <string>

struct Component {
	typedef int Type;
	typedef std::shared_ptr<Component> Ptr;
};

struct TransformComponent : public Component {
	static const Type family = 1;

	bool placed;
	irr::core::vector3df position;
	irr::core::vector3df direction;

	typedef std::shared_ptr<TransformComponent> Ptr;
};

struct MovementComponent : public Component {
	static const Type family = 2;

	float speed;
	irr::core::vector3df destination;

	enum State {
		STAND,
		ON_MOVE
	};

	State state;

	typedef std::shared_ptr<MovementComponent> Ptr;
};

struct RenderComponent : public Component {
	static const Type family = 3;

	std::string mesh_name;
	irr::scene::ISceneNode * node;

	typedef std::shared_ptr<RenderComponent> Ptr;
};

struct ProductionComponent : public Component {
	static const Type family = 4;
	
	bool in_progress;
	std::string produce_result;
	float time_remaning;
	irr::core::vector3df production_point;

	typedef std::shared_ptr<ProductionComponent> Ptr;
};

struct ResourceComponent : public Component {
	static const Type family = 5;

	enum ResourceType
	{
		Minerals,
		Gas
	};

	ResourceType resource_type;
	int resource_count;

	typedef std::shared_ptr<ResourceComponent> Ptr;
};

struct HealthComponent : public Component {
	static const Type family = 6;

	int hit_points;

	typedef std::shared_ptr<HealthComponent> Ptr;
};

struct WeaponComponent : public Component {
	static const Type family = 7;

	int dmg;
	float cooldown;

	typedef std::shared_ptr<WeaponComponent> Ptr;
};

struct CollisionComponent : public Component {
	static const Type family = 8;

	// TODO

	typedef std::shared_ptr<CollisionComponent> Ptr;
};