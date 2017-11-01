#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType bodytype, bool bullet, float restitution);
	PhysBody* CreateRectangle(int x, int y, int width, int height, float density, float restitution, b2BodyType bodytype);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType bodytype);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType bodytype, float restitution);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, float density);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* A, PhysBody* B, float anchor_x, float anchor_y, int reference_angle, int lower_angle, int upper_angle);
	bool DeleteBody(b2Body* body);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

public:

	p2List<PhysBody*> todelete;

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	bool is_clicked = false;
	b2Vec2 mouse_position;
	b2Body* clicked_body;
};