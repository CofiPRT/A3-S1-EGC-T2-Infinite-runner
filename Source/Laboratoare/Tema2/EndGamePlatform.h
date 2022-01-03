#pragma once

#include "BasePlatform.h"

#define END_GAME_PLATFORM_COLOR (COLOR_RED)

class EndGamePlatform : public BasePlatform {

protected:
	virtual glm::vec3 GetSpecialColor() override;
	virtual void ApplySpecialEffect(Player* player) override;

public:
	EndGamePlatform(glm::vec3& position);
	~EndGamePlatform();

};