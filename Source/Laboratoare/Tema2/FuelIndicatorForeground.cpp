#include "FuelIndicatorForeground.h"

glm::vec3 FuelIndicatorForeground::GetMeshColor() {
	return INDICATOR_FOREGROUND_COLOR;
}

void FuelIndicatorForeground::RevealAt(glm::vec3 position, float percentage) {
	this->position = position + glm::vec3((percentage - 1) * INDICATOR_WIDTH / 2, 0, 0);
	this->scale = glm::vec3(percentage * INDICATOR_WIDTH, INDICATOR_HEIGHT, INDICATOR_LENGTH);
}