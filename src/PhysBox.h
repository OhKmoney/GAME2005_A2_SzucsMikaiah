#pragma once
#ifndef __PHYSBOX__
#define __PHYSBOX__

#include "Sprite.h"

class PhysBox final : public Sprite
{
public:
	PhysBox();
	~PhysBox();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	int getAngle();
	void setAngle(int val);

private:
	int m_Angle;
};

#endif /* defined (__PHYSBOX__) */
