#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Background.h"
#include "PhysBox.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	Background* m_pBackGround;

	PhysBox* m_pProjectile;



	//Basic Values
	float t = 0;
	float gravity = 9.8f;
	float ground = 500;

	//Adjustable Values
	float rise = 3;
	float run = 4;
	float rampAngle = atan(rise / run);

	float mass = 12.8;
	float scale = 25;
	float frictionCoefficient = 0.42;


	//Physics calculation and display vriables
	glm::vec2 velocity = glm::vec2(0.0, 0.0);
	float acceleration = (gravity)*sin(rampAngle);
	

	float initialX = 0;
	float initialY = ground - (scale * 3);
	float currentX = initialX;
	float currentY = initialY;
	

	//Additional variables
	bool initialized = false;
	bool initializing = false;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Label* m_pResetLabel;

	//Display Labels
	Label* m_pDistanceLabel;
	Label* m_pTimeLabel;

	Label* m_pVelocityLabel;
	Label* m_pAccelerationLabel;
};

#endif /* defined (__PLAY_SCENE__) */