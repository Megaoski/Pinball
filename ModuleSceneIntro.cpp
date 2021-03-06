#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "string.h"
#include "ModuleWindow.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	/*circle = box = rick = NULL;*/
	ray_on = false;
	/*sensed = false;*/
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Textures
	/*sprites = App->textures->Load("sprites/material.png"); */
	ballo = App->textures->Load("sprites/pinball_ball.png");
	background = App->textures->Load("sprites/tablero.png");
	lkicker = App->textures->Load("sprites/leftkicker.png");
	rkicker = App->textures->Load("sprites/rightkicker.png");

	//fx
	bonus_fx = App->audio->LoadFx("fx/bonus.wav");
	loose_ball_fx = App->audio->LoadFx("fx/loose_ball.wav");
	kicker_fx = App->audio->LoadFx("fx/kicker.wav");
	launcher_fx = App->audio->LoadFx("fx/launcher.wav");
	turbo_fx = App->audio->LoadFx("fx/turbo.wav");
	bumper_fx = App->audio->LoadFx("fx/bumper.wav");
	triangle_fx = App->audio->LoadFx("fx/triangle.wav");

	//sensors
	endsensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, 890, 600, 200, b2BodyType::b2_staticBody);//sensor de game over
	launchersensor = App->physics->CreateRectangleSensor(712, 430, 45, 30, b2BodyType::b2_staticBody);//sensor launcher
	turbosensor = App->physics->CreateRectangleSensor(310, 400, 45, 45, b2BodyType::b2_staticBody);//middle one
	holesensor = App->physics->CreateRectangleSensor(460, 50, 45, 30, b2BodyType::b2_staticBody);//up hole

	//Creating table
	backgrounds.add(App->physics->CreateChain(245, 0, tabla, 92, b2BodyType::b2_staticBody, 0));//TABLERO chained
	piece.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, toppiece, 16, b2BodyType::b2_staticBody, 0));
	line1.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, botleftline, 16, b2BodyType::b2_staticBody, 0));
	line2.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, botrightline, 16, b2BodyType::b2_staticBody, 0));
	line3.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, topballsline, 8, b2BodyType::b2_staticBody, 0));
	rampa.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, tobogan, 20, b2BodyType::b2_staticBody, 0));
	line4.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, topline2, 8, b2BodyType::b2_staticBody, 0));
	line5.add(App->physics->CreateChain(SCREEN_WIDTH / 4, 0, topline1, 8, b2BodyType::b2_staticBody, 0));
	
	//bouncers...
	chincheta1 = App->physics->CreateCircle(575, 200, 20, b2BodyType::b2_staticBody, false, 2.0f);
	chincheta1 = App->physics->CreateCircle(545, 140, 20, b2BodyType::b2_staticBody, false, 2.0f);
	chincheta3 = App->physics->CreateCircle(620, 140, 20, b2BodyType::b2_staticBody, false, 2.0f);
	chincheta4 = App->physics->CreateCircle(340, 80, 20, b2BodyType::b2_staticBody, false, 2.0f);

	tentaclebouncer1 = App->physics->CreateChain(SCREEN_WIDTH / 4, 0, rectbouncer1, 8, b2BodyType::b2_staticBody, 1.0f);
	tentaclebouncer2 = App->physics->CreateChain(SCREEN_WIDTH / 4, 0, rectbouncer2, 8, b2BodyType::b2_staticBody, 1.0f);
	tentaclebouncer3 = App->physics->CreateChain(SCREEN_WIDTH / 4, 0, rectbouncer3, 8, b2BodyType::b2_staticBody, 1.0f);
	tentaclebouncer4 = App->physics->CreateChain(SCREEN_WIDTH / 4, 0, rectbouncer4, 8, b2BodyType::b2_staticBody, 1.0f);

	lefttriangle = App->physics->CreateChain(SCREEN_WIDTH / 4, 0, leftbottriangle, 6, b2BodyType::b2_staticBody, 0.7f);
	righttriangle = App->physics->CreateChain(SCREEN_WIDTH / 4, 0, rightbottriangle, 6, b2BodyType::b2_staticBody, 0.7f);


	//anim
	left_kicker.PushBack({ 477, 237, 42, 80 });
	

	//calling functions
	CreateKickers();

	//initializing 
	
	on_launcher = true;
	on_turbo = true;
	ball_up = false;
	loose = false;
	
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//title
	p2SString title(" Pinball - SCORE: %d    - BALLS: %d", App->player->score, App->player->balls_left);
	App->window->SetTitle(title.GetString());

	if (on_launcher && !ball_up && !loose)//initializing the first ball
	{
		ball = App->physics->CreateCircle(712, 430, 12, b2BodyType::b2_dynamicBody, true, 0.2f);
		ball->listener = this;
		ball_up = true;
	}

	if (loose && App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		App->player->balls_left = 3;
		App->player->score = 0;
		loose = false;
	}

	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && on_launcher == true && !loose)
	{
		ball->body->SetType(b2_dynamicBody);
		ball->body->SetLinearVelocity(b2Vec2(0, -20));
		on_launcher = false;
		App->audio->PlayFx(launcher_fx);
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12, b2BodyType::b2_dynamicBody, true, 0.0f);//creating ball
		ball->listener = this;
	
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		rev_joint_left->SetMotorSpeed(100);
		rev_joint_top->SetMotorSpeed(100);
		
		
	}
	else
	{
		rev_joint_left->SetMotorSpeed(-100);
		rev_joint_top->SetMotorSpeed(-100);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		rev_joint_right->SetMotorSpeed(-100);
		

	}
	else
	{
		rev_joint_right->SetMotorSpeed(100);
	}

	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	

	App->renderer->Blit(background, SCREEN_WIDTH / 4, 0, NULL, 0.0f, NULL); //pintamos el background

	if (ball_up)
	{
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ballo, x, y, NULL, 0.0f, NULL);
	}

	DrawKickers();
	
	
	

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}


	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	

	if (bodyB == launchersensor)//fixes launcher bug
	{
		on_launcher = true;
	}

	if (bodyB == turbosensor)
	{
		if (bodyA->body->GetLinearVelocity().y < 0) {
			bodyA->body->SetLinearVelocity(b2Vec2(-60, -60));
			App->player->score += 500;
			App->audio->PlayFx(turbo_fx);
		}
	}

	

	if (bodyB == endsensor)
	{
		App->physics->todelete.add(bodyA);
		on_launcher = true;
		ball_up = false;
		App->player->balls_left--;
		App->audio->PlayFx(loose_ball_fx);

		if (App->player->balls_left == 0)
		{
			loose = true;
		}
		
	}

	if (bodyB == chincheta1 || bodyB == chincheta2 || bodyB == chincheta3 || bodyB == chincheta4)
	{
		App->player->score += 600;
		App->audio->PlayFx(bumper_fx);
	}

	if (bodyB == lefttriangle || bodyB == righttriangle)
	{
		App->player->score += 100;
	}

	if (bodyB == tentaclebouncer1 || bodyB == tentaclebouncer2 || bodyB == tentaclebouncer3)
	{
		App->player->score += 400;
	}

	if (bodyB == leftkicker || bodyB == rightkicker || bodyB == topkicker)
	{
		App->player->score += 100;
	}

	if (bodyB == lefttriangle || bodyB == righttriangle)
	{
		App->player->score += 100;
		App->audio->PlayFx(triangle_fx);
	}

	if (bodyB == tentaclebouncer1 || bodyB == tentaclebouncer2 || bodyB == tentaclebouncer3 || bodyB == tentaclebouncer4)
	{
		App->player->score += 500;
		App->audio->PlayFx(bumper_fx);
	}
	
	if (bodyB == leftkicker || bodyB == rightkicker || bodyB == topkicker)
	{
		App->audio->PlayFx(kicker_fx);
	}

	

}

	



void ModuleSceneIntro::CreateKickers()
{
	int left_kicker_vertices[8] = {
		150, 716,
		140, 735,
		212, 765,
		215, 754
	};

	int right_kicker_vertices[8] = {
		271, 765,
		338, 734,
		329, 716,
		267, 756
		
	};

	int top_kicker_vertices2[8] = {
		41, 254,
		51, 236,
		111, 275,
		108, 283
	};

	

	
	leftkicker = App->physics->CreateRectangle(430, 728, 85, 15, 1.0f, 0, b2BodyType::b2_dynamicBody);
	PhysBody* B = App->physics->CreateCircle(416, 728, 7, b2BodyType::b2_staticBody, false, 0);
	rev_joint_left = App->physics->CreateRevoluteJoint(leftkicker, B, -31, 0, -15, -15, 15);

	rightkicker = App->physics->CreateRectangle(590, 728, 85, 15, 1.0f, 0, b2BodyType::b2_dynamicBody);
	PhysBody* B2 = App->physics->CreateCircle(571, 728, 7, b2BodyType::b2_staticBody, false, 0);
	rev_joint_right = App->physics->CreateRevoluteJoint(rightkicker, B2, 31, 0, -167, 165, 200);

	topkicker = App->physics->CreateRectangle(255, 250, 80, 15, 1.0f, 0, b2BodyType::b2_dynamicBody);
	PhysBody* B3 = App->physics->CreateCircle(310, 250, 7, b2BodyType::b2_staticBody, false, 0);
	rev_joint_top = App->physics->CreateRevoluteJoint(topkicker, B3, -31, 0, -15, -15, 15);

	
}

void ModuleSceneIntro::DrawKickers()
{
	
	int x, y;

	leftkicker->GetPosition(x, y);
	App->renderer->Blit(lkicker, x + 8, y - 18, NULL, 1.0f, leftkicker->GetRotation() - 30);

	
	rightkicker->GetPosition(x, y);
	App->renderer->Blit(rkicker, x - 5, y - 22, NULL, 1.0f, rightkicker->GetRotation() + 30);
	

	topkicker->GetPosition(x, y);
	App->renderer->Blit(lkicker, x + 8, y - 18, NULL, 1.0f, topkicker->GetRotation() - 30);

}