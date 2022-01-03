#include "EndGamePlatform.h"

EndGamePlatform::EndGamePlatform(glm::vec3& position)
	: BasePlatform(position) {

}

EndGamePlatform::~EndGamePlatform() {

}

void EndGamePlatform::ApplySpecialEffect(Player* player) {
	player->LoseLifePoint();
}

glm::vec3 EndGamePlatform::GetSpecialColor() {
	return END_GAME_PLATFORM_COLOR;
}