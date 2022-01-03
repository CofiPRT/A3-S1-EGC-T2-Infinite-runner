#include "Game.h"

Game::Game() {
	NewGame();
}

Game::~Game() {

}

void Game::Update(double deltaTimeSeconds) {

	deltaTimeSeconds *= currGameTick;

	if (startDelta > -1)
		startDelta -= deltaTimeSeconds;

	if (IsPaused())
		return;

	UpdateStartAnimations(deltaTimeSeconds);

	if (IsStarting())
		return;

	UpdateDistanceTraveled(deltaTimeSeconds);
	UpdateDifficulty();
	UpdateObjects(deltaTimeSeconds);

	camera->Set(player->GetPosition() + GetCameraShake(), AXIS_OZ, AXIS_OY);

	if (!IsPlaying())
		return;

	AttemptSpawn();

}

void Game::UpdateObjects(double deltaTimeSeconds) {
	
	auto it = platforms.begin();
	bool supported = false; // the player can only touch one platform at a time
	
	// PLATFORMS
	while (it != platforms.end()) {
		BasePlatform* platform = (*it);
		
		platform->SetSpeed(player->GetSpeed());
		platform->Update(deltaTimeSeconds);

		if (platform->IsDead()) {
			it = platforms.erase(it);
			delete platform;
			continue;
		}

		if (!supported && platform->IsBelow(player))
			supported = true;

		it++;
	}

	// PLAYER
	player->SetSupported(supported);
	player->Update(deltaTimeSeconds);

	// HUD - FUEL INDICATOR
	fuelForeground->RevealAt(fuelBackground->GetPosition(), player->GetFuel() / PLAYER_MAX_FUEL_AMOUNT);

	if (player->IsDead())
		EndGame();

}

void Game::UpdateDistanceTraveled(double deltaTimeSeconds) {
	float distance = player->GetSpeed() * deltaTimeSeconds;

	distanceTraveled += distance;
	distToNextSpawn -= distance;
}

void Game::UpdateStartAnimations(double deltaTimeSeconds) {

	auto deltaToRotationX = MathUtils::MapSmooth(
		GAME_START_DURATION, 0,
		M_PI / 12, 0,
		GAME_START_ANIMATION_SMOOTHNESS
	);

	auto deltaToRotationY = MathUtils::MapSmooth(
		GAME_START_DURATION, 0,
		-M_PI, 0,
		GAME_START_ANIMATION_SMOOTHNESS
	);

	auto deltaToPositionX = MathUtils::MapSmooth(
		GAME_START_DURATION, 0,
		-2.f * PLATFORM_WIDTH * GAME_COLUMN_COUNT, player->GetPosition().x,
		GAME_START_ANIMATION_SMOOTHNESS
	);

	auto deltaToPositionY = MathUtils::MapSmooth(
		GAME_START_DURATION, 0,
		1.1f * PLATFORM_HEIGHT, player->GetPosition().y,
		GAME_START_ANIMATION_SMOOTHNESS
	);

	auto deltaToPositionZ = MathUtils::MapSmooth(
		GAME_START_DURATION, 0,
		0.5f * PLATFORM_LENGTH, player->GetPosition().z,
		GAME_START_ANIMATION_SMOOTHNESS
	);

	auto deltaToCDistance = MathUtils::MapSmooth(
		GAME_START_DURATION, 0,
		10.f * CAMERA_INITIAL_3RD_PERSON_DISTANCE, CAMERA_INITIAL_3RD_PERSON_DISTANCE,
		GAME_START_ANIMATION_SMOOTHNESS
	);

	float cameraPosX = deltaToPositionX(startDelta);
	float cameraPosY = deltaToPositionY(startDelta);
	float cameraPosZ = deltaToPositionZ(startDelta);

	float cameraRotX = deltaToRotationX(startDelta);
	float cameraRotY = deltaToRotationY(startDelta);

	float cameraDist = deltaToCDistance(startDelta);

	camera->Set(glm::vec3(cameraPosX, cameraPosY, cameraPosZ), AXIS_OZ, AXIS_OY);
	camera->RotateThirdPerson_OX(cameraRotX);
	camera->RotateThirdPerson_OY(cameraRotY);
	camera->SetDistanceToTarget(cameraDist);

	if (startDelta < 0) {
		state = GameState::PLAYING;
		camera->SetDistanceToTarget(CAMERA_INITIAL_3RD_PERSON_DISTANCE);
	}

}

void Game::UpdateDifficulty() {

	if (currDifficulty == DIFFICULTY_EASY && distanceTraveled > GAME_DIFFICULTY_MEDIUM_DISTANCE_THRESHOLD)
		LoadSettings(DIFFICULTY_MEDIUM);
	else if (currDifficulty == DIFFICULTY_MEDIUM && distanceTraveled > GAME_DIFFICULTY_HARD_DISTANCE_THRESHOLD)
		LoadSettings(DIFFICULTY_HARD);

}

void Game::OnInputUpdate(WindowObject* window, float deltaTime, int mods) {

	if (!IsPlaying())
		return;

	if (window->KeyHold(GLFW_KEY_W))
		player->Accelerate(deltaTime);

	if (window->KeyHold(GLFW_KEY_S))
		player->Decelerate(deltaTime);

	if (window->KeyHold(GLFW_KEY_A))
		player->MoveLeft(deltaTime);

	if (window->KeyHold(GLFW_KEY_D))
		player->MoveRight(deltaTime);

}

void Game::OnKeyPress(int key, int mods) {

	if (key == GLFW_KEY_R)
		Restart();

	if (IsEnded() && IsStarting())
		return;

	if (key == GLFW_KEY_C) {
		camera->ToggleThirdPerson();
		fuelBackground->TogglePosition();
		fuelForeground->TogglePosition();
		frame->TogglePosition();
	}

	if (key == GLFW_KEY_P)
		TogglePause();

	if (!IsPlaying())
		return;

	switch (key) {
	case GLFW_KEY_SPACE:
		player->Jump();
		break;

	case GLFW_KEY_LEFT_BRACKET:
		DecreaseGameTick();
		break;

	case GLFW_KEY_RIGHT_BRACKET:
		IncreaseGameTick();
		break;

	default:
		break;
	}

}

void Game::SpawnNewGamePlatforms() {

	// prepare a starting area full of platforms for the player
	for (int row = 0; row < GAME_SPAWN_DISTANCE; row++)
		for (int column = 0; column < GAME_COLUMN_COUNT; column++)
			platforms.push_back(new BasePlatform(glm::vec3(PLATFORM_WIDTH * column, 0, PLATFORM_LENGTH * row)));

}

void Game::NewGame() {
	
	state = GameState::STARTING;

	delete player;
	player = new Player(glm::vec3(PLATFORM_WIDTH / 2 * (GAME_COLUMN_COUNT - 1), PLATFORM_HEIGHT + PLAYER_SPHERE_RADIUS, 0));

	for (auto platform : platforms)
		delete platform;
	platforms.clear();

	delete fuelBackground;
	delete fuelForeground;
	delete frame;

	fuelBackground = new FuelIndicatorBackground();
	fuelForeground = new FuelIndicatorForeground();
	frame = new Frame();

	distanceTraveled = 0.f;
	distToNextSpawn = 0.f;

	prevGameTick = 1.f;
	currGameTick = 1.f;
	
	LoadSettings(DIFFICULTY_EASY);

	SpawnNewGamePlatforms();

	delete camera;
	camera = new Student::Camera();

	startDelta = GAME_START_DURATION;

}

void Game::EndGame() {
	state = GameState::ENDED;
	currGameTick = 0;
}

void Game::LoadSettings(std::string difficulty) {

	SettingsStore* instance = SettingsStore::GetInstance();

	// NORMAL PLATFORM
	normalPlatformSpawnCountDist = IntDist(
		instance->GetIntSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MIN, difficulty),
		instance->GetIntSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MAX, difficulty)
	);

	// BAD PLATFORMS
	badPlatformSpawnPool.clear();

	badPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {
			platforms.push_back(new EndGamePlatform(position));
		}, instance->GetFloatSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_END_GAME, difficulty)
	});

	badPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {
			platforms.push_back(new OverdrivePlatform(position));
		}, instance->GetFloatSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_OVERDRIVE, difficulty)
	});

	badPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {
			platforms.push_back(new LoseFuelPlatform(position));
		}, instance->GetFloatSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_LOSE_FUEL, difficulty)
	});

	badPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {

		}, instance->GetFloatSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_NONE, difficulty)
	});

	// calculate the total weight of the bad platform spawn pool
	float totalWeight = 0;

	for (auto entry : badPlatformSpawnPool)
		totalWeight += entry.second;

	badPlatformSpawnWeightDist = FloatDist(0, totalWeight);

	// GOOD PLATFORMS
	goodPlatformSpawnPool.clear();

	goodPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {
			platforms.push_back(new InvulnerabilityPlatform(position));
		}, instance->GetFloatSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_INVULNERABLE, difficulty)
	});

	goodPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {
			platforms.push_back(new GetFuelPlatform(position));
		}, instance->GetFloatSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_GET_FUEL, difficulty)
	});

	goodPlatformSpawnPool.push_back({
		[this](glm::vec3 position) {

		}, instance->GetFloatSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_NONE, difficulty)
	});

	// calculate the total weight of the good platform spawn pool
	totalWeight = 0;

	for (auto entry : goodPlatformSpawnPool)
		totalWeight += entry.second;

	goodPlatformSpawnWeightDist = FloatDist(0, totalWeight);

	// load player settings
	player->LoadSettings(difficulty);

	timelessMinSpeed = instance->GetFloatSetting(SETTING_PLAYER_SPEED_MIN, DIFFICULTY_EASY);
	timelessMaxSpeed = instance->GetFloatSetting(SETTING_PLAYER_SPEED_MAX, DIFFICULTY_HARD);

	currDifficulty = difficulty;

}

void Game::AttemptSpawn() {

	// don't spawn anything yet
	if (distToNextSpawn > 0)
		return;

	// reset to a platform length, minus the time wasted
	distToNextSpawn += PLATFORM_LENGTH;

	std::vector<int> availableColumns;
	for (int i = 0; i < GAME_COLUMN_COUNT; i++)
		availableColumns.push_back(i);

	// spawn a specific number of normal platforms
	int normalPlatformsToSpawn = normalPlatformSpawnCountDist(RandomUtils::mt);

	while (availableColumns.size() > 0 && normalPlatformsToSpawn > 0) {

		glm::vec3 location = ComputeSpawnLocation(availableColumns);

		// spawn the normal platform
		platforms.push_back(new BasePlatform(location));

		normalPlatformsToSpawn--;

	}

	// attempt to spawn a bad platform
	if (availableColumns.size() == 0)
		return;

	SpawnPlatformFromPool(badPlatformSpawnPool, badPlatformSpawnWeightDist ,ComputeSpawnLocation(availableColumns));

	// attempt to spawn a good platform
	if (availableColumns.size() == 0)
		return;

	SpawnPlatformFromPool(goodPlatformSpawnPool, goodPlatformSpawnWeightDist, ComputeSpawnLocation(availableColumns));

}

void Game::SpawnPlatformFromPool(SpawnPool spawnPool, FloatDist dist, glm::vec3 location) {

	float randomWeight = dist(RandomUtils::mt);
	float currWeight = 0;

	for (auto& entry : spawnPool) {
		currWeight += entry.second;

		if (randomWeight < currWeight) {
			entry.first(location); // spawn the object
			return;
		}
	}

}

glm::vec3 Game::ComputeSpawnLocation(std::vector<int>& availableColumns) {

	// choose a random column
	platformSpawnColumnDist = IntDist(0, availableColumns.size() - 1);
	int index = platformSpawnColumnDist(RandomUtils::mt);
	int column = availableColumns[index];

	// remove this column from the available ones
	availableColumns.erase(availableColumns.begin() + index);

	return glm::vec3(PLATFORM_WIDTH * column, 0, PLATFORM_LENGTH * GAME_SPAWN_DISTANCE);
}

glm::vec3 Game::GetCameraShake() {

	auto speedToShakeFactor = MathUtils::MapLinear(
		timelessMinSpeed, timelessMaxSpeed,
		0.0, 1.0
	);

	float shakeFactor = speedToShakeFactor(player->GetSpeed());

	float amplitudeX = CAMERA_SHAKE_AMPLITUDE_X * shakeFactor;
	float amplitudeY = CAMERA_SHAKE_AMPLITUDE_Y * shakeFactor;
	float amplitudeZ = CAMERA_SHAKE_AMPLITUDE_Z * shakeFactor;

	float frequencyX = CAMERA_SHAKE_FREQUENCY_X;
	float frequencyY = CAMERA_SHAKE_FREQUENCY_Y;
	float frequencyZ = CAMERA_SHAKE_FREQUENCY_Z;

	float shakeX = amplitudeX * sin(frequencyX * distanceTraveled + CAMERA_SHAKE_OFFSET_X);
	float shakeY = amplitudeY * sin(frequencyY * distanceTraveled + CAMERA_SHAKE_OFFSET_Y);
	float shakeZ = amplitudeZ * sin(frequencyZ * distanceTraveled + CAMERA_SHAKE_OFFSET_Z);

	return glm::vec3(shakeX, shakeY, shakeZ);

}

float Game::GetFOV() {

	auto speedToFOV = MathUtils::MapLinear(
		timelessMinSpeed, timelessMaxSpeed,
		GAME_FOV_MIN, GAME_FOV_MAX
	);

	return speedToFOV(player->GetSpeed());

}

float Game::GetStartDelta() {
	return startDelta;
}

Student::Camera* Game::GetCamera() {
	return camera;
}

std::vector<BaseObject*> Game::GetObjectsToRender() {

	std::vector<BaseObject*> objects;

	if (camera->IsThirdPerson())
		objects.push_back(player);

	for (auto platform : platforms)
		objects.push_back(platform);

	objects.push_back(fuelForeground);
	objects.push_back(fuelBackground);
	objects.push_back(frame);

	return objects;

}

std::vector<std::pair<std::string, glm::vec3>> Game::GetTextsToRender() {
	std::vector<std::pair<std::string, glm::vec3>> texts;

	bool thirdPerson = camera->IsThirdPerson();
	glm::vec3 upDown = glm::vec3(1.0, thirdPerson ? 1.0 : -1.0, 1.0);
	glm::vec3 offset = glm::vec3(0.0, thirdPerson ? 0.0 : INDICATOR_HEIGHT, 0.0);

	// FUEL
	{
		glm::vec3 origin = glm::vec3(TEXT_FUEL_POS_X, TEXT_FUEL_POS_Y, 1.0);
		glm::vec3 position = origin * upDown + offset;
		texts.push_back({ "Fuel", position });
	}

	// DISTANCE
	{
		glm::vec3 origin = glm::vec3(TEXT_DISTANCE_POS_X, TEXT_DISTANCE_POS_Y, 1.0);
		glm::vec3 position = origin * upDown + offset;
		texts.push_back({ "Distance: " + std::to_string((int) distanceTraveled), position });
	}

	// LIFE POINTS
	{
		glm::vec3 origin = glm::vec3(TEXT_LIFE_POINTS_POS_X, TEXT_LIFE_POINTS_POS_Y, 1.0);
		glm::vec3 position = origin * upDown + offset;
		texts.push_back({ "Life points: " + std::to_string(player->GetLifePoints()), position });
	}

	return texts;
}

void Game::TogglePause() {

	if (IsPlaying()) {
		state = GameState::PAUSED;
		prevGameTick = currGameTick;
		currGameTick = 0;
	} else if (IsPaused()) {
		state = GameState::PLAYING;
		currGameTick = prevGameTick;
	}

}

void Game::Restart() {
	NewGame();
}

void Game::DecreaseGameTick() {
	currGameTick -= GAME_TICK_CHANGE_STEP;

	if (currGameTick <= 0)
		currGameTick = 0;
}

void Game::IncreaseGameTick() {
	currGameTick += GAME_TICK_CHANGE_STEP;
}

bool Game::IsStarting() {
	return state == GameState::STARTING;
}

bool Game::IsPlaying() {
	return state == GameState::PLAYING;
}

bool Game::IsPaused() {
	return state == GameState::PAUSED;
}

bool Game::IsEnded() {
	return state == GameState::ENDED;
}
