#pragma once

#include "BaseFuelIndicator.h"

#define INDICATOR_BACKGROUND_COLOR (COLOR_WHITE)

class FuelIndicatorBackground : public BaseFuelIndicator {

public:
	glm::vec3 GetMeshColor() override;

};