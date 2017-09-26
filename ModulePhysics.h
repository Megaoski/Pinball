#pragma once
#include "Module.h"
#include "Globals.h"

#define PIXELS_PER_METER 50.0f
#define METERS_PER_PIXEL -7.0f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXELS_TO_METER(p) ((float) METERS_PER_PIXEL * p)

class b2World; //needed to declare a b2world pointer in the class
class b2Body;

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:

	b2World* myWorld;
	b2Body* ground;
	bool debug;
};