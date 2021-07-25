#pragma once

// ECS: Entity Component System

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <windows.h>
#include <string>
#include "SDL.h"
#include <SDL_ttf.h>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getNewComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

// 游戲模塊
class Component
{
public:
	Entity* entity;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
	virtual BOOL clickCheck(int x, int y) { return FALSE; }
};

// 游戲實體單元
class Entity
{
private:
	Manager &manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componenetBitset;
	GroupBitset groupBitset;

public:

	Entity(Manager& mManager) : manager(mManager) {}

	void update()
	{
		for (auto& c : components) c->update();
	}

	void draw() 
	{
		for (auto& c : components) c->draw();
	}

	bool isActive()
	{ 
		return active; 
	};

	void distroy() 
	{ 
		active = false; 
	};

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	};

	void addGroup(Group mGroup);

	void delGroup(Group mGroup);

	template <typename T> bool hasComponent() const
	{
		return componenetBitset[getNewComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>T& 
	addComponenet(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getNewComponentTypeID<T>()] = c;
		componenetBitset[getNewComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const
	{
		auto ptr(componentArray[getNewComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};


// 實體單元管理器
class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupEntities;

public:
	void update()
	{
		for (auto& e : entities) e->update();
	}

	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}), 
			std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};