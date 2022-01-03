#include "OverdrivePlatform.h"

OverdrivePlatform::OverdrivePlatform(glm::vec3& position)
	: BasePlatform(position) {

}

OverdrivePlatform::~OverdrivePlatform() {

}

void OverdrivePlatform::ApplySpecialEffect(Player* player) {
	player->ApplyOverdrivePowerUp(DEFORMATION_OVERDRIVE_DURATION);
}

glm::vec3 OverdrivePlatform::GetSpecialColor() {
	return OVERDRIVE_PLATFORM_COLOR;
}