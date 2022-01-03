#pragma once

#include "BasePlatform.h"

#define OVERDRIVE_PLATFORM_COLOR (COLOR_ORANGE)
#define OVERDRIVE_PLATFORM_TIMER (5.f)

class OverdrivePlatform : public BasePlatform {

protected:
	virtual glm::vec3 GetSpecialColor() override;
	virtual void ApplySpecialEffect(Player* player) override;

public:
	OverdrivePlatform(glm::vec3& position);
	~OverdrivePlatform();

};