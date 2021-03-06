/* Copyright 2018-2019 the Age of Empires Free Software Remake authors. See LEGAL for legal info */

/**
 * Virtual world logic model
 *
 * Licensed under Affero General Public License v3.0
 * Copyright by Folkert van Verseveld.
 */

#pragma once

#include "image.hpp"

#include <vector>

struct Point final {
	int x, y;

	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	friend bool operator==(const Point &lhs, const Point &rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	void to_screen(Point &dst) const;

	void move(int dx, int dy) { x += dx; y += dy; }
};

struct AABB final {
	Point pos, hbounds;

	AABB(Point pos = {}, Point hbounds = Point(1, 1))
		: pos(pos), hbounds(hbounds) {}

	AABB(int size) : AABB(size, size) {}
	AABB(int w, int h) : AABB({0, 0}, {w, h}) {}

	bool contains(const Point &p) const {
		return p.x < pos.x + hbounds.x && p.x >= pos.x - hbounds.x
			&& p.y < pos.y + hbounds.y && p.y >= pos.y - hbounds.y;
	}

	bool intersects(const AABB &other) const {
		return (pos.x + hbounds.x >= other.pos.x - other.hbounds.x || pos.x - hbounds.x < other.pos.x + other.hbounds.x)
			&& (pos.y + hbounds.y >= other.pos.y - other.hbounds.y || pos.y - hbounds.y < other.pos.y + other.hbounds.y);
	}

	friend bool operator==(const AABB &lhs, const AABB &rhs) {
		return lhs.pos == rhs.pos && lhs.hbounds == rhs.hbounds;
	}
};

enum MapSize {
	MICRO, // for debug purposes...
	TINY,
	SMALL,
	MEDIUM,
	LARGE,
	HUGE_, // HUGE is already being used somewhere...
};

class Map final {
public:
	std::unique_ptr<uint8_t[]> map, heightmap;
	unsigned w, h;
	int left, bottom, right, top;

	Map() : map() {}
	Map(unsigned w, unsigned h);

	/** Update viewport limits. */
	void reshape(int x, int y, int w, int h);
	void resize(MapSize size);
private:
	void resize(unsigned w, unsigned h);
	void resize(unsigned size) { resize(size, size); }
};

/**
 * World particle (e.g. tree chunks, debris)
 */
class Particle {
public:
	Point pos;

	AABB scr;
	// tile displacement
	int dx, dy, w, h;
	unsigned size;
	unsigned id;
	static unsigned count; // FIXME debug stuff, remove when done
	static unsigned id_counter;

	unsigned color;

	const AnimationTexture &animation;
	unsigned image_index;

	Particle(int x, int y, unsigned size, int w, int h, unsigned sprite_index, unsigned color, int dx=0, int dy=0);

	virtual void draw(Map &map) const;

	unsigned drs_id() const {
		return animation.id;
	}

	friend bool operator==(const Particle &lhs, const Particle &rhs) {
		return lhs.id == rhs.id;
	}
};

/**
 * Elementary world unit. This includes gaia stuff (e.g. trees, gold, berry
 * bushes...)
 */
class Unit : public Particle {
public:
	unsigned hp, hp_max;

	Unit(
		unsigned hp, int x, int y,
		unsigned size, int w, int h,
		unsigned sprite_index,
		unsigned color,
		int dx=0, int dy=0
	) : Particle(x, y, size, w, h, sprite_index, color, dx, dy), hp(hp), hp_max(hp) {}
	virtual ~Unit() { --count; }

	void draw_selection(Map &map) const;
	virtual void to_screen(Map &map);
};

class DynamicUnit : public Unit {
public:
	Point target;
	float tdx, tdy;
	float movespeed;

	DynamicUnit(
		unsigned hp, int x, int y,
		unsigned size, int w, int h,
		unsigned sprite_index,
		unsigned color,
		float movespeed,
		int dx=0, int dy=0
	) : Unit(hp, x, y, size, w, h, sprite_index, color, dx, dy)
	, target(x, y), tdx(0), tdy(0), movespeed(movespeed) {}
	virtual ~DynamicUnit() {}

	void tick();
	void move(int tx, int ty, int dx, int dy);
};

class Villager : public DynamicUnit {
public:
	Villager(unsigned hp, int x, int y, unsigned color, int dx=0, int dy=0)
		: DynamicUnit(hp, x, y, 14, 0, 0, DRS_VILLAGER_STAND, color, 2, dx, dy) {}
};

#define SR_FOOD 0
#define SR_WOOD 1
#define SR_GOLD 2
#define SR_STONE 3

// e.g. berry bush, tree, etc.
class StaticResource final : public Unit {
public:
	unsigned type, amount;

	StaticResource(
		int x, int y, unsigned size, int w, int h, unsigned id,
		unsigned type, unsigned amount
	) : Unit(0, x, y, size, w, h, id, 8, 0, 0), type(type), amount(amount) {}
};

#if 0
// deer, lion, elephant, aligator
class DynamicResource final : public StaticResource {
public:
	unsigned decay, timer;
};
#endif

class Building final : public Unit {
public:
	const AnimationTexture &overlay;
	unsigned overlay_index;

	Building(
		unsigned hp, unsigned id, unsigned p_id,
		int x, int y, unsigned size, int w, int h, unsigned color
	);
	Building(
		unsigned hp, unsigned id, unsigned p_id,
		int x, int y, unsigned size, unsigned color
	) : Building(hp, id, p_id, x, y, size, 0, 0, color) {}
	void draw(Map &map) const override;
	void to_screen(Map &map) override;
};

class Quadtree final {
	AABB bounds;
public:
	// this one manages all non-units only
	std::vector<std::shared_ptr<Particle>> particles;
	// this one manages the memory, hence it uses shared_ptr
	std::vector<std::shared_ptr<Unit>> objects;
	// this one only holds the dynamic units from objects, hence no shared_ptr
	std::vector<DynamicUnit*> dynamic_objects;
	bool split;
	Quadtree(AABB bounds = {}) : bounds(bounds), particles(), objects(), split(false) {}

	bool put(Unit *obj);
	bool erase(Unit *obj);
	void update(Unit *obj);
	void clear();

	void query(std::vector<std::shared_ptr<Unit>> &lst, AABB bounds);
	void query(std::vector<std::shared_ptr<Particle>> &lst, AABB bounds);
private:
	void reshape(AABB bounds) { this->bounds = bounds; }
};
