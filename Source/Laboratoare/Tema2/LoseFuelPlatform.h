#pragma once

#include "BasePlatform.h"

#define LOSE_FUEL_PLATFORM_COLOR	(COLOR_YELLOW)
#define LOSE_FUEL_PLATFORM_AMOUNT	(100.f)

class LoseFuelPlatform : public BasePlatform {

protected:
	virtual glm::vec3 GetSpecialColor() override;
	virtual void ApplySpecialEffect(Player* player) override;

public:
	LoseFuelPlatform(glm::vec3& position);
	~LoseFuelPlatform();

};