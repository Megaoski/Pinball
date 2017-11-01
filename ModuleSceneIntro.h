#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void CreateKickers();
	void DrawKickers();

public:
	//bodys

	PhysBody* ball;
	PhysBody* leftkicker;
	PhysBody* rightkicker;
	PhysBody* topkicker;

	//joints

	b2RevoluteJoint* rev_joint_right = nullptr;
	b2RevoluteJoint* rev_joint_left = nullptr;
	b2RevoluteJoint* rev_joint_top = nullptr;

	//board

	p2List<PhysBody*> backgrounds;
	p2List<PhysBody*> piece;
	p2List<PhysBody*> line1;
	p2List<PhysBody*> line2;
	p2List<PhysBody*> line3;
	p2List<PhysBody*> rampa;
	p2List<PhysBody*> line4;
	p2List<PhysBody*> line5;
	p2List<PhysBody*> tablero;

	//bouncers

	PhysBody* chincheta1;
	PhysBody* chincheta2;
	PhysBody* chincheta3;
	PhysBody* chincheta4;
	PhysBody* tentaclebouncer1;
	PhysBody* tentaclebouncer2;
	PhysBody* tentaclebouncer3;
	PhysBody* tentaclebouncer4;
	PhysBody* lefttriangle;
	PhysBody* righttriangle;

	//sensors

	PhysBody* launchersensor;
	PhysBody* endsensor;
	PhysBody* turbosensor;
	PhysBody* holesensor;

	//textures
	SDL_Texture* sprites;

	//animation
	Animation left_kicker;
	Animation right_kicker;

	int tabla[92]
	{
		
		445, 441,
		445, 206,
		409, 247,
		415, 254,
		430, 249,
		440, 254,
		442, 269,
		412, 308,
		410, 396,
		439, 449,
		440, 487,
		488, 523,
		485, 669,
		318, 803,
		184, 801,
		13, 672,
		12, 523,
		71, 479,
		70, 463,
		46, 439,
		13, 364,
		12, 63,
		29, 32,
		61, 11,
		86, 4,
		114, 4,
		143, 17,
		162, 30,
		179, 63,
		197, 64,
		199, 28,
		215, 15,
		233, 27,
		233, 57,
		259, 83,
		267, 82,
		268, 35,
		286, 18,
		320, 6,
		418, 6,
		438, 10,
		455, 19,
		470, 31,
		481, 45,
		489, 64,
		492, 440
	};

	int leftbottriangle[6]
	{
		89, 605,
		87, 518,
		154, 659

	};

	int rightbottriangle[6]
	{
		321, 654,
		389, 521,
		391, 606
	};

	int toppiece[16]
	{
		45, 222,
		50, 230,
		38, 240,
		42, 264,
		99, 289,
		66, 364,
		37, 354,
		28, 238
	};

	int botleftline[16]
	{
		45, 542,
		50, 540,
		50, 601,
		60, 632,
		152, 707,
		147, 712,
		57, 641,
		43, 605
	};

	int botrightline[16]
	{
		429, 540,
		435, 540,
		436, 604,
		424, 640,
		330, 713,
		324, 706,
		417, 635,
		429, 602
	};

	int topballsline[8]
	{
		269, 226,
		309, 266,
		316, 261,
		276, 220
	};

	int tobogan[20]
	{
		374, 84,
		374, 62,
		395, 48,
		429, 70,
		428, 205,
		435, 204,
		435, 67,
		392, 39,
		364, 59,
		365, 84
	};

	int topline1[8]
	{
		286, 58,
		294, 59,
		294, 86,
		286, 85
	};

	int topline2[8]
	{
		327, 60,
		327, 87,
		332, 87,
		333, 60
	};

	int rectbouncer1[8] = {
		56, 357,
		74, 363,
		106, 310,
		91, 295
	};

	int rectbouncer2[8] = {
		398, 407,
		407, 393,
		427, 435,
		422, 445
	};

	int rectbouncer3[8] = {
		266, 233,
		272, 224,
		309, 259,
		303, 268
	};

	int rectbouncer4[8] = {
		171, 62,
		169, 74,
		182, 78,
		181, 62
	};

	

/*	
	bool sensed*/;
	bool on_launcher;
	bool on_turbo;
	bool ball_up;
	bool loose;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	
	int current_balls;
	int current_score;

	uint bonus_fx;

	p2Point<int> ray;
	bool ray_on;
};
