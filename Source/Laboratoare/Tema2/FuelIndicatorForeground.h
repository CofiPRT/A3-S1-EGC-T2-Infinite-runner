#pragma once

#include "BaseFuelIndicator.h"

#define INDICATOR_FOREGROUND_COLOR (COLOR_ORANGE)

class FuelIndicatorForeground : public BaseFuelIndicator {

public:
	glm::vec3 GetMeshColor() override;

	void RevealAt(glm::vec3 position, float percentage);

};