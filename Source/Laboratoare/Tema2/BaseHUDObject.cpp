#include "BaseHUDObject.h"

bool BaseHUDObject::IsHudElement() {
	return true;
}

void BaseHUDObject::TogglePosition() {
	position *= glm::vec3(1, -1, 1);
	rotation = glm::vec3(0, 0, M_PI) - rotation;
}