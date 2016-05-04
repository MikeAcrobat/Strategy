#pragma once

#include "components/gamecomponent.h"
#include <map>
#include <memory>

struct Entity {
	template<class T>
	std::shared_ptr<T> get_component() const;

	Component::Ptr get_component(Component::Type) const;

	std::map<Component::Type, Component::Ptr> m_components;

	typedef std::shared_ptr<Entity> Ptr;
	typedef std::weak_ptr<Entity> Weak;
};

template<class T>
inline std::shared_ptr<T> Entity::get_component() const {
	return m_components.find(T::family) != m_components.end() ? std::static_pointer_cast<T>(m_components.at(T::family)) : nullptr;
}

class EntityHelpers {
public:
	static void set_position(Entity::Ptr, const irr::core::vector3df &);
	static void set_direction(Entity::Ptr, const irr::core::vector3df &);
	static void update_transformation(Entity::Ptr &);
	static void initialize_components(Entity::Ptr, irr::scene::ISceneManager *);
	static void update_selection(Entity::Ptr, bool);
};
