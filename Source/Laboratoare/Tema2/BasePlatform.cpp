#include "BasePlatform.h"

BasePlatform::BasePlatform(glm::vec3& position) {
	this->position = position;
	this->scale = glm::vec3(PLATFORM_WIDTH, PLATFORM_HEIGHT, PLATFORM_LENGTH);
}

BasePlatform::~BasePlatform() {}

void BasePlatform::PostIsBelow(Player* player) {

	if (!player->IsOnGround() || status == PlatformStatus::CLAIMED)
		return;

	status = PlatformStatus::CLAIMED;
	ApplySpecialEffect(player);

}

void BasePlatform::ApplySpecialEffect(Player* player) {

}

void BasePlatform::SetSpeed(float speed) {
	this->velocity = -AXIS_OZ * speed;
}

std::string BasePlatform::GetMeshName() {
	return "box";
}

glm::vec3 BasePlatform::GetSpecialColor() {
	return COLOR_LIGHT_GRAY;
}

glm::vec3 BasePlatform::GetMeshColor() {
	return status == PlatformStatus::CLAIMED ? PLATFORM_CLAIMED_COLOR : GetSpecialColor();
}

bool BasePlatform::IsBelow(Player* player) {

	// the player is a sphere
	glm::vec3 minimums = position - scale / 2.f;
	glm::vec3 maximums = position + scale / 2.f;

	glm::vec3 playerPos = player->GetPosition();
	bool inBoundsX = playerPos.x >= minimums.x && playerPos.x <= maximums.x;
	bool inBoundsY = playerPos.y - PLAYER_SPHERE_RADIUS >= maximums.y;
	bool inBoundsZ = playerPos.z >= minimums.z && playerPos.z <= maximums.z;

	bool result = inBoundsX && inBoundsY && inBoundsZ;
	if (result)
		PostIsBelow(player);

	return result;

}

void BasePlatform::UpdateAlive(double deltaTimeSeconds) {
	UpdateTrajectory(deltaTimeSeconds);
}

float BasePlatform::GetGravitationalAcceleration() {
	return 0;
}

bool BasePlatform::IsInBounds() {
	return position.z > -PLATFORM_LENGTH;
}
