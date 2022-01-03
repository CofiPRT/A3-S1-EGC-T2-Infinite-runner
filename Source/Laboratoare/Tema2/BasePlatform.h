#pragma once

#include "Player.h"

#define PLATFORM_HEIGHT		(1.f)
#define PLATFORM_WIDTH		(2.5f)
#define PLATFORM_LENGTH		(50.f)

#define PLATFORM_CLAIMED_COLOR (COLOR_PURPLE)

class BasePlatform : public BaseObject {

protected:
	enum class PlatformStatus { UNCLAIMED, CLAIMED };

	PlatformStatus status = PlatformStatus::UNCLAIMED;

	virtual glm::vec3 GetSpecialColor();
	virtual void ApplySpecialEffect(Player* player);

	void UpdateAlive(double deltaTimeSeconds) override;
	float GetGravitationalAcceleration() override;
	bool IsInBounds() override;

	virtual void PostIsBelow(Player* player);

public:
	BasePlatform(glm::vec3& position);
	~BasePlatform();

	std::string GetMeshName() override;
	glm::vec3 GetMeshColor() override;

	void SetSpeed(float speed);
	bool IsBelow(Player* player);

};