#include "Background.h"


#include "SoundManager.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance().load("../Assets/textures/beach.png", "lootworld");

	auto size = TextureManager::Instance().getTextureSize("lootworld");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

}

Background::~Background()
= default;

void Background::draw()
{
	TextureManager::Instance().draw("lootworld",
		getTransform()->position.x, getTransform()->position.y, 0, 0, true);
}

void Background::update()
{
}

void Background::clean()
{
}
