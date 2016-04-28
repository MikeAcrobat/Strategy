#include "entity.h"
#include "entityfactory.h"
#include <irrlicht.h>
#include <string>
#include <fstream>
#include "json/json.h"

template<class T>
bool parse_component(std::shared_ptr<T> &, const Json::Value & value);

template<> bool parse_component(TransformComponent::Ptr & component, const Json::Value & value) {
	return true;
}

template<> bool parse_component(StatsComponent::Ptr & component, const Json::Value & value) {
	component->hit_points = value.get("hit_points", 100).asInt();
	component->move_speed = value.get("move_speed", 0).asFloat();
	return false;
}

template<> bool parse_component(RenderComponent::Ptr & component, const Json::Value & value) {
	component->mesh_name = value.get("mesh", "").asString();
	component->node = nullptr;
	return false;
}

template<> bool parse_component(ProductionComponent::Ptr & component, const Json::Value & value) {
	component->produce_result = "";
	component->time_remaning = 0;
	component->in_progress = 0;
	component->can_produce = value.get("can_produce", true).asBool();
	return false;
}

template<> bool parse_component(ResourceComponent::Ptr & component, const Json::Value & value) {
	return false;
}

template<> bool parse_component(AIComponent::Ptr & component, const Json::Value & value) {
	return false;
}

template<> bool parse_component(WeaponComponent::Ptr & component, const Json::Value & value) {
	component->attack_speed = value.get("speed", 1).asFloat();
	component->attack_damage = value.get("damage", 1).asInt();
	component->attack_range = value.get("range", 1).asFloat();
	return false;
}

template<> bool parse_component(CollisionComponent::Ptr & component, const Json::Value & value) {
	return false;
}

template<> bool parse_component(CostComponent::Ptr & component, const Json::Value & value) {
	return false;
}

template<> bool parse_component(MiningComponent::Ptr & component, const Json::Value & value) {
	return false;
}

typedef void(*create_component_func)(EntitySystem &, Entity::Ptr &);
typedef bool(*load_component_func)(Component::Ptr &, const Json::Value &);
typedef Entity::Ptr(*create_entity_func)();

template<typename T>
void create_component(EntitySystem & entity_system, Entity::Ptr & entity) {
	std::shared_ptr<T> component = std::make_shared<T>();
	entity_system.add_component(entity, component);
}

template<typename T>
bool parser(Component::Ptr & component, const Json::Value & value) {
	std::shared_ptr<T> cmp = std::static_pointer_cast<T>(component);
	return parse_component<T>(cmp, value);
}

struct Helper {
	create_component_func create_func;
	load_component_func parse_func;
	Component::Type family;
};

template<typename T>
struct ComponentHelper : Helper {
	ComponentHelper() {
		this->create_func = &create_component<T>;
		this->parse_func = &parser<T>;
		this->family = T::family;
	}
};

static std::map<std::string, Helper> component_helpers;
static std::map<std::string, Json::Value> entity_templates;

Entity::Ptr create_entity(EntitySystem & entity_system, const Json::Value & data) {
	Entity::Ptr entity = std::make_shared<Entity>();

	for (const Json::Value & value : data["components"]) {
		const std::string & component_name = value.asString();

		if (component_helpers.find(component_name) == component_helpers.end()) {
			continue;
		}
		
		Helper & helper = component_helpers[component_name];
		helper.create_func(entity_system, entity);
	}

	if (data.isMember("components_defaults")) {
		const Json::Value & defaults = data["components_defaults"];
		for (const std::string & component_name : defaults.getMemberNames()) {
			if (component_helpers.find(component_name) == component_helpers.end()) {
				continue;
			}

			Helper & helper = component_helpers[component_name];
			Component::Ptr component = entity->get_component(helper.family);
			if (component) {
				helper.parse_func(component, defaults[component_name]);
			}
		}
	}

	return entity;
}

EntityFactory::EntityFactory(EntitySystem & entity_system, irr::scene::ISceneManager * scene_manager) : m_entity_system(entity_system), m_scene_manager(scene_manager) {
}

void EntityFactory::register_components() {
	component_helpers["transform"] = ComponentHelper<TransformComponent>();
	component_helpers["stats"] = ComponentHelper<StatsComponent>();
	component_helpers["render"] = ComponentHelper<RenderComponent>();
	component_helpers["production"] = ComponentHelper<ProductionComponent>();
	component_helpers["resouce"] = ComponentHelper<ResourceComponent>();
	component_helpers["ai"] = ComponentHelper<AIComponent>();
	component_helpers["weapon"] = ComponentHelper<WeaponComponent>();
	component_helpers["collision"] = ComponentHelper<CollisionComponent>();
	component_helpers["cost"] = ComponentHelper<CostComponent>();
	component_helpers["miner"] = ComponentHelper<MiningComponent>();
}

bool EntityFactory::register_templates(const std::string & filename)
{
	std::ifstream file(filename, std::ios::in);
	if (file.is_open()) {
		Json::Reader reader;
		Json::Value content;
		if (!reader.parse(file, content)) {
			return false;
		}

		if (!content.isMember("templates")) {
			return false;
		}

		Json::Value & templates = content["templates"];
		for (Json::Value & entity_template : templates) {
			if (entity_template.isMember("name")) {
				const std::string & name = entity_template["name"].asString();
				entity_templates[name] = entity_template;				
			}
		}

		return true;

	} else {
		return false;
	}
}

Entity::Ptr EntityFactory::create_object(const std::string & name)
{
	if (entity_templates.find(name) != entity_templates.end()) {
		Entity::Ptr entity = create_entity(m_entity_system, entity_templates[name]);
		EntityHelpers::initialize_components(entity, m_scene_manager);
		return entity;
	}

	return nullptr;
}