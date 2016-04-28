#pragma once

#include "irrlicht.h"
#include <memory>
#include <string>

class AIStrategy;

enum ResourceType
{
	Minerals,
	Gas
};

struct Component {
	typedef int Type;
	typedef std::shared_ptr<Component> Ptr;
};

struct TransformComponent : public Component {
	static const Type family = 1;

	irr::core::vector3df position;
	irr::core::vector3df direction;

	typedef std::shared_ptr<TransformComponent> Ptr;
};

struct StatsComponent : public Component {
	static const Type family = 2;

	float move_speed;
	int hit_points;

	bool selected;

	typedef std::shared_ptr<StatsComponent> Ptr;
};

struct RenderComponent : public Component {
	static const Type family = 3;

	std::string mesh_name;
	irr::scene::ISceneNode * node;

	typedef std::shared_ptr<RenderComponent> Ptr;
};

struct ProductionComponent : public Component {
	static const Type family = 4;
	
	bool can_produce;
	bool in_progress;
	std::string produce_result;
	float time_remaning;
	irr::core::vector3df production_point;

	typedef std::shared_ptr<ProductionComponent> Ptr;
};

struct ResourceComponent : public Component {
	static const Type family = 5;

	ResourceType resource_type;
	int resource_count;

	typedef std::shared_ptr<ResourceComponent> Ptr;
};

struct AIComponent : public Component {
	static const Type family = 6;

	std::shared_ptr<AIStrategy> strategy;
	
	typedef std::shared_ptr<AIComponent> Ptr;
};

struct WeaponComponent : public Component {
	static const Type family = 7;

	int attack_damage;
	float attack_speed;
	float attack_range;

	typedef std::shared_ptr<WeaponComponent> Ptr;
};

struct CollisionComponent : public Component {
	static const Type family = 8;

	// TODO

	typedef std::shared_ptr<CollisionComponent> Ptr;
};

struct CostComponent : public Component {
	static const Type family = 9;

	int price;
	ResourceType resource_type;

	typedef std::shared_ptr<CostComponent> Ptr;
};

struct MiningComponent : public Component {
	static const Type family = 10;

	float mining_speed;
	int resource_collected;
	ResourceType resource_type;

	typedef std::shared_ptr<MiningComponent> Ptr;
};