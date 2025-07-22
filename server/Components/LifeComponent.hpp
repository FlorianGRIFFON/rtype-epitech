#pragma once

struct LifeComponent
{
	int remainingLife;

	LifeComponent(int life) {
		remainingLife = life;
	}

	LifeComponent() {
		remainingLife = 0;
	}
};
