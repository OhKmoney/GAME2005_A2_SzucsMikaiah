#include "PhysBox.h"
#include "TextureManager.h"

PhysBox::PhysBox()
{
	TextureManager::Instance().load("../Assets/textures/box.png", "box");

	auto size = TextureManager::Instance().getTextureSize("box");
	setWidth(size.x);
	setHeight(size.y);
	m_Angle = 36;
}

PhysBox::~PhysBox()
= default;

void PhysBox::draw()
{
	TextureManager::Instance().draw("box", getTransform()->position.x, getTransform()->position.y - 17, m_Angle, 255, true);
}

void PhysBox::update()
{
}

void PhysBox::clean()
{
}

void PhysBox::setAngle(int val)
{
	m_Angle = val;
}

int PhysBox::getAngle()
{
	return m_Angle;
}