#pragma once

#include <vector>
#include <entity.h>

struct EntitySystem 
{
	template<typename T> 
	std::shared_ptr<T> get_component(Entity::Ptr e) {
		return std::static_pointer_cast<T>(e->m_components[T::Type]);
	}

	template<typename T>
	bool has(Entity::Ptr entity) const {
		return entity->get_component<T>() != nullptr;
	}

	template<typename T, typename K, typename ...Types>
	bool has(Entity::Ptr entity) const {
		return entity->get_component<T>() != nullptr && has<K, Types...>(entity);
	}

	template<typename T>
	void get_entities(std::vector<Entity::Ptr> &result) const {
		auto iterPair = m_component_store.equal_range(T::family);
		for (auto iter = iterPair.first; iter != iterPair.second; ++iter) {
			result.push_back(iter->second);
		}
	}

	template<typename T, typename K, typename ...Types>
	void get_entities(std::vector<Entity::Ptr> &result) const {
		auto iterPair = m_component_store.equal_range(T::family);
		for (auto iter = iterPair.first; iter != iterPair.second; ++iter) {
			if (has<K, Types...>(iter->second)) {
				result.push_back(iter->second);
			}
		}
	}

	template<typename T> 
	std::shared_ptr<T> add_component(Entity::Ptr e, std::shared_ptr<T> component) {
		m_component_store.insert(std::pair<T::Type, Entity::Ptr>(T::family, e));
		e->m_components.insert(std::pair<T::Type, T::Ptr>(T::family, component));
		return component;
	}

protected:
	static std::multimap<Component::Type, Entity::Ptr> m_component_store;
};