#pragma once

#include "BasePlatform.h"

#define GET_FUEL_PLATFORM_COLOR		(COLOR_GREEN)
#define GET_FUEL_PLATFORM_AMOUNT	(250.f)

class GetFuelPlatform : public BasePlatform {

protected:
	virtual glm::vec3 GetSpecialColor() override;
	virtual void ApplySpecialEffect(Player* player) override;

public:
	GetFuelPlatform(glm::vec3& position);
	~GetFuelPlatform();

};