#include "Player.h"
#include "BasePlatform.h"

Player::Player(glm::vec3 position) {
	this->position = position;
	this->scale = glm::vec3(PLAYER_SPHERE_RADIUS * 2);
}

Player::~Player() {
	
}

void Player::UpdateTrajectory(double deltaTimeSeconds) {
	BaseObject::UpdateTrajectory(deltaTimeSeconds);

	if (isSupported && position.y <= PLATFORM_HEIGHT + PLAYER_SPHERE_RADIUS)
		Land();
}

void Player::UpdateAlive(double deltaTimeSeconds) {

	UpdatePowerUpTimer(deltaTimeSeconds);

	if (!isSupported)
		FreeFall();

	switch (powerUp) {
	case PowerUpEffect::OVERDRIVE:
		Accelerate(deltaTimeSeconds);
		break;
	default:
		ApplyBaseDeceleration(deltaTimeSeconds);
	}

	switch (state) {
	case PlayerState::AIR:
		UpdateTrajectory(deltaTimeSeconds);
		TestFall();
		break;
	case PlayerState::GROUND:
		UpdateIdle(deltaTimeSeconds);
		break;
	default:
		break;
	}

}

void Player::UpdateIdle(double deltaTimeSeconds) {

	fuel = fmax(0, fuel - fuelConsumptionIdle * deltaTimeSeconds);

	if (speed < minSpeed)
		Accelerate(deltaTimeSeconds);

	if (speed > ComputeMaxSpeed())
		Decelerate(deltaTimeSeconds);

	if (speed == 0)
		LoseLifePoint();

}

void Player::UpdatePowerUpTimer(double deltaTimeSeconds) {

	// nothing to do
	if (powerUp == PowerUpEffect::NONE)
		return;

	powerUpTimer -= deltaTimeSeconds;

	if (powerUpTimer > 0)
		return;

	// end power-up
	powerUp = PowerUpEffect::NONE;
	powerUpTimer = 0;

}

void Player::Accelerate(double deltaTime) {
	
	if ((state != PlayerState::GROUND && powerUp != PowerUpEffect::OVERDRIVE) || fuel == 0)
		return;

	IncreaseSpeed(PLAYER_ACCELERATION * deltaTime);
	fuel = fmax(0, fuel - fuelConsumptionAcceleration * deltaTime);

}

void Player::Decelerate(double deltaTime) {

	if (state != PlayerState::GROUND || powerUp == PowerUpEffect::OVERDRIVE)
		return;

	DecreaseSpeed(PLAYER_DECELERATION * deltaTime);

}

void Player::MoveLeft(double deltaTime) {
	position += glm::vec3(PLAYER_LATERAL_SPEED * deltaTime, 0, 0);
}

void Player::MoveRight(double deltaTime) {
	position -= glm::vec3(PLAYER_LATERAL_SPEED * deltaTime, 0, 0);
}

void Player::Jump() {

	if (state != PlayerState::GROUND)
		return;

	velocity = glm::vec3(0, PLAYER_JUMP_STRENGTH, 0);
	state = PlayerState::AIR;

}

void Player::Land() {

	if (state != PlayerState::AIR)
		return;

	position.y = PLATFORM_HEIGHT + PLAYER_SPHERE_RADIUS;
	velocity = glm::vec3(0);
	state = PlayerState::GROUND;
}

void Player::IncreaseSpeed(float amount) {

	if (speed > ComputeMaxSpeed())
		return;

	speed = fmin(ComputeMaxSpeed(), speed + amount);
}

void Player::DecreaseSpeed(float amount) {

	if (speed < minSpeed && fuel > 0)
		return;

	speed = fmax(fuel > 0 ? minSpeed : 0, speed - amount);

}

void Player::ApplyBaseDeceleration(double deltaTimeSeconds) {

	if (state != PlayerState::GROUND)
		return;

	DecreaseSpeed(PLAYER_BASE_DECELERATION * deltaTimeSeconds);

}

void Player::Refuel(float amount) {
	powerUpTimer = 0;

	fuel = fmin(PLAYER_MAX_FUEL_AMOUNT, fuel + amount);

	ApplyDeformation(DEFORMATION_GET_FUEL, DEFORMATION_GET_FUEL_DURATION);
}

void Player::LoseFuel(float amount) {

	if (Protect())
		return;

	powerUpTimer = 0;

	fuel = fmax(0, fuel - amount);

	ApplyDeformation(DEFORMATION_LOSE_FUEL, DEFORMATION_LOSE_FUEL_DURATION);
}

void Player::ApplyOverdrivePowerUp(float time) {

	if (Protect())
		return;

	powerUp = PowerUpEffect::OVERDRIVE;
	powerUpTimer = time;

	ApplyDeformation(DEFORMATION_OVERDRIVE, DEFORMATION_OVERDRIVE_DURATION);

}

void Player::ApplyInvulnerablePowerUp(float time) {
	powerUp = PowerUpEffect::INVULNERABLE;
	powerUpTimer = time;

	ApplyDeformation(DEFORMATION_INVULNERABLE, DEFORMATION_INVULNERABLE_DURATION);
}

void Player::LoseLifePoint() {

	if (Protect())
		return;

	powerUpTimer = 0;

	if (--lifePoints == 0)
		StartDecay();
	else
		fuel = PLAYER_MAX_FUEL_AMOUNT;

	ApplyDeformation(DEFORMATION_LOSE_LIFE_POINT, DEFORMATION_LOSE_LIFE_POINT_DURATION);

}

void Player::SetSupported(bool value) {
	isSupported = value;
}

void Player::FreeFall() {
	state = PlayerState::AIR;
}

bool Player::Protect() {
	
	if (powerUp != PowerUpEffect::INVULNERABLE)
		return false;

	// consume
	powerUp = PowerUpEffect::NONE;
	powerUpTimer = 0;
	deformationDuration = 0;

	return true;

}

void Player::TestFall() {
	if (position.y < -PLATFORM_HEIGHT)
		StartDecay();
}

float Player::ComputeMaxSpeed() {
	return maxSpeed * (powerUp == PowerUpEffect::OVERDRIVE ? PLAYER_OVERDRIVE_MULTIPLIER : 1.0);
}

void Player::LoadSettings(std::string difficulty) {

	SettingsStore* instance = SettingsStore::GetInstance();

	minSpeed = instance->GetFloatSetting(SETTING_PLAYER_SPEED_MIN, difficulty);
	maxSpeed = instance->GetFloatSetting(SETTING_PLAYER_SPEED_MAX, difficulty);
	fuelConsumptionIdle = instance->GetFloatSetting(SETTING_PLAYER_FUEL_CONSUMPTION_IDLE, difficulty);
	fuelConsumptionAcceleration = instance->GetFloatSetting(SETTING_PLAYER_FUEL_CONSUMPTION_ACCELERATION, difficulty);

}

std::string Player::GetMeshName() {
	return "sphere";
}

glm::vec3 Player::GetMeshColor() {
	return COLOR_SPRAY_BLUE;
}

void Player::StartDecay() {

	if (objState != ObjectState::ALIVE)
		return;

	ApplyDeformation(DEFORMATION_LOSE_LIFE_POINT, DEFORMATION_LOSE_LIFE_POINT_DURATION);

	BaseObject::StartDecay();

}

bool Player::IsInBounds() {
	return position.y >= PLAYER_BOUND_MIN_Y;
}

float Player::GetFuel() {
	return fuel;
}

float Player::GetSpeed() {
	return speed;
}

float Player::GetMinSpeed() {
	return minSpeed;
}

float Player::GetMaxSpeed() {
	return maxSpeed;
}

int Player::GetLifePoints() {
	return lifePoints;
}

bool Player::IsOnGround() {
	return state == PlayerState::GROUND;
}