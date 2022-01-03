#pragma once

#include "BaseObject.h"
#include "SettingsStore.h"
#include "FuelIndicatorBackground.h"
#include "FuelIndicatorForeground.h"

#define PLAYER_ACCELERATION (20.f)
#define PLAYER_DECELERATION (40.f)
#define PLAYER_BASE_DECELERATION (5.f)

#define PLAYER_JUMP_STRENGTH (25.f)
#define PLAYER_LATERAL_SPEED (10.f)

#define PLAYER_OVERDRIVE_MULTIPLIER (1.5f)
#define PLAYER_MAX_FUEL_AMOUNT		(1000.f)
#define PLAYER_SPHERE_RADIUS		(0.75f)
#define PLAYER_BOUND_MIN_Y			(-100.f)

#define PLAYER_INITIAL_LIFE_POINTS	(3)

class Player : public BaseObject {

private:
	enum class PowerUpEffect { NONE, OVERDRIVE, INVULNERABLE };
	enum class PlayerState { STATIONARY, GROUND, AIR };

	PowerUpEffect powerUp = PowerUpEffect::NONE;
	PlayerState state = PlayerState::GROUND;

	// this field is used for game logic; it moves the platforms, not the player
	float speed = 0;
	float powerUpTimer = 0;
	float fuel = PLAYER_MAX_FUEL_AMOUNT;

	bool isSupported = false;

	int lifePoints = PLAYER_INITIAL_LIFE_POINTS;

	float minSpeed = 0;
	float maxSpeed = 0;
	float fuelConsumptionIdle = 0;
	float fuelConsumptionAcceleration = 0;

	void UpdateIdle(double deltaTimeSeconds);
	void UpdatePowerUpTimer(double deltaTimeSeconds);

	void IncreaseSpeed(float amount);
	void DecreaseSpeed(float amount);
	void ApplyBaseDeceleration(double deltaTimeSeconds);

	bool Protect();
	void TestFall();
	float ComputeMaxSpeed();

protected:
	void UpdateTrajectory(double deltaTimeSeconds) override;
	void UpdateAlive(double deltaTimeSeconds) override;


public:
	Player(glm::vec3 position);
	~Player();

	std::string GetMeshName() override;
	glm::vec3 GetMeshColor() override;
	void StartDecay() override;
	bool IsInBounds() override;

	void Accelerate(double deltaTime);
	void Decelerate(double deltaTime);
	void MoveLeft(double deltaTime);
	void MoveRight(double deltaTime);
	void Jump();
	void Land();

	void Refuel(float amount);
	void LoseFuel(float amount);
	void ApplyOverdrivePowerUp(float time);
	void ApplyInvulnerablePowerUp(float time);
	void LoseLifePoint();

	void SetSupported(bool value);
	void FreeFall();

	void LoadSettings(std::string difficulty);

	float GetFuel();
	float GetSpeed();
	float GetMinSpeed();
	float GetMaxSpeed();
	int GetLifePoints();

	bool IsOnGround();

};