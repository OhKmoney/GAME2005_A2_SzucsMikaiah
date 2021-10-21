#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include "Math.h"

using namespace std;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 150, 0, 150);

	// Draw grid based on ground height and screen width
	for (int i = 0; i <= Config::SCREEN_WIDTH;)
	{
		SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 0 + i, 0, 0 + i, ground);
		i += scale;
	}

	for (int i = 0; i <= ground;)
	{
		SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 0, 0 + i, Config::SCREEN_WIDTH, 0 + i);
		i += scale;
	}

	//Draw ramp
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 1);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 0, ground - (scale * rise), scale * run, ground);
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 0, ground, Config::SCREEN_WIDTH, ground);

	//Reset color
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{

	float dt = Game::Instance().getDeltaTime();
	t += dt;

	initialY = ground - (scale * rise);


	//Detect if box has reached ground level
	if (currentY < ground)
	{
		//Affix Acceleration and Velocity for on Ramp
		acceleration = (gravity)*(mass)*sin(rampAngle);
		velocity.x += acceleration * cos(rampAngle) * dt;
		velocity.y += acceleration * sin(rampAngle) * dt;

		currentX += velocity.x * dt * scale;
		currentY += velocity.y * dt * scale;
		//Default angle for rotation on Ramp
		m_pProjectile->setAngle(36);
	}

	else if (velocity.x > 0) {
		//Affix Acceleration and Velocity for on Ground
		acceleration = -(gravity * mass * frictionCoefficient);
		velocity.x += acceleration * cos(rampAngle) * dt;
		

		currentY = ground;
		currentX += velocity.x * dt * scale;
		//Default angle for rotation on Ground
		m_pProjectile->setAngle(0);

	}

	else {
	}

	//Move Box
	m_pProjectile->getTransform()->position = glm::vec2(currentX, currentY);

	//Label stuff here
	//Doesn't actually matter if this is added but the labels make more sense
	if (velocity.x < 0) velocity.x = 0;

	//Only want this label to update while box is moving
	if (velocity.x > 0) m_pTimeLabel->setText("Time: " + std::to_string(t) + "s");

	//Other Label stuff here
	m_pDistanceLabel->setText("Distance: " + std::to_string((currentX - initialX) / scale) + "m");
	m_pVelocityLabel->setText("Velocity X: " + std::to_string(velocity.x) + "m/s");
	m_pAccelerationLabel->setText("Acceleration: " + std::to_string(acceleration) + "m/s^2");
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController



	// handle player movement if no Game Controllers found
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_F))
	{
		//Reset simulation

		t = 0;
		currentX = initialX;
		currentY = initialY;
		rampAngle = atan(rise / run);
		velocity = glm::vec2(0.0, 0.0);
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	//std::cout << std::setprecision(2) << std::fixed;

	// Physics Box
	m_pProjectile = new PhysBox();
	addChild(m_pProjectile, 1);
	m_pProjectile->getTransform()->position = glm::vec2(currentX, currentY);

	// Background
	m_pBackGround = new Background();
	addChild(m_pBackGround, 0);
	m_pBackGround->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, Config::SCREEN_HEIGHT * 0.2);


	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 525.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
		{
			m_pBackButton->setActive(false);
			TheGame::Instance().changeSceneState(START_SCENE);
		});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pBackButton->setAlpha(128);
		});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pBackButton->setAlpha(255);
		});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 525.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
		{
			m_pNextButton->setActive(false);
			TheGame::Instance().changeSceneState(END_SCENE);
		});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pNextButton->setAlpha(128);
		});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pNextButton->setAlpha(255);
		});

	addChild(m_pNextButton);


	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, Config::SCREEN_HEIGHT * 0.05);
	addChild(m_pInstructionsLabel);

	m_pResetLabel = new Label("Press F to reset physics simulation", "Consolas");
	m_pResetLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, Config::SCREEN_HEIGHT * 0.1);
	addChild(m_pResetLabel);

	//Display Labels
	m_pDistanceLabel = new Label("Distance: 0.00m", "Consolas");
	m_pDistanceLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, Config::SCREEN_HEIGHT * 0.2);
	addChild(m_pDistanceLabel);

	m_pTimeLabel = new Label("Time: 0.00s", "Consolas");
	m_pTimeLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, Config::SCREEN_HEIGHT * 0.25);
	addChild(m_pTimeLabel);

	m_pVelocityLabel = new Label("Velocity X: 0.00m/s", "Consolas");
	m_pVelocityLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, Config::SCREEN_HEIGHT * 0.3);
	addChild(m_pVelocityLabel);

	m_pAccelerationLabel = new Label("Acceleration: 0.00m/s^2", "Consolas");
	m_pAccelerationLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, Config::SCREEN_HEIGHT * 0.35);
	addChild(m_pAccelerationLabel);



	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();


	ImGui::Begin("Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::SliderFloat("PPM", &scale, 1.0f, 100.0f, "%.1f");
	ImGui::SliderFloat("Coeffcient of Kinetic Friction", &frictionCoefficient, 0.0f, 3.0f, "%.1f");
	ImGui::SliderFloat("Mass", &mass, 1.0f, 100.0f, "%.1f");
	ImGui::SliderFloat("Rise", &rise, 00.f, 10.0f, "%.1f");
	ImGui::SliderFloat("Run", &run, 0.0f, 10.0f, "%.1f");

	

	ImGui::End();
}
