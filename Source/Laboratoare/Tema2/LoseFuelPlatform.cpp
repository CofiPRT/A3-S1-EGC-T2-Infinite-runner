#include "LoseFuelPlatform.h"

LoseFuelPlatform::LoseFuelPlatform(glm::vec3& position)
	: BasePlatform(position) {

}

LoseFuelPlatform::~LoseFuelPlatform() {

}

void LoseFuelPlatform::ApplySpecialEffect(Player* player) {
	player->LoseFuel(LOSE_FUEL_PLATFORM_AMOUNT);
}

glm::vec3 LoseFuelPlatform::GetSpecialColor() {
	return LOSE_FUEL_PLATFORM_COLOR;
}