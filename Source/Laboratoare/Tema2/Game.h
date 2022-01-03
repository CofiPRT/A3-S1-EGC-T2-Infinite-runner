#pragma once

#include <random>

#include "RandomUtils.h"
#include "MathUtils.h"
#include "Camera.h"
#include "Player.h"
#include "Frame.h"
#include "EndGamePlatform.h"
#include "OverdrivePlatform.h"
#include "LoseFuelPlatform.h"
#include "InvulnerabilityPlatform.h"
#include "GetFuelPlatform.h"

#define GAME_COLUMN_COUNT (5)
#define GAME_SPAWN_DISTANCE (4)

#define GAME_DIFFICULTY_MEDIUM_DISTANCE_THRESHOLD	(5000.f)
#define GAME_DIFFICULTY_HARD_DISTANCE_THRESHOLD		(15000.f)

#define GAME_TICK_CHANGE_STEP	(0.1f)

#define GAME_FOV_MIN (60.f)
#define GAME_FOV_MAX (90.f)

#define GAME_START_DURATION (3.f)
#define GAME_START_ANIMATION_SMOOTHNESS (1.f / 8.f)

#define TEXT_FUEL_POS_X			(INDICATOR_POS_X - 0.5f * INDICATOR_WIDTH)
#define TEXT_FUEL_POS_Y			(INDICATOR_POS_Y + 2.f * INDICATOR_HEIGHT)
#define TEXT_DISTANCE_POS_X		(FRAME_POS_X - 0.05f * FRAME_WIDTH)
#define TEXT_DISTANCE_POS_Y		(FRAME_POS_Y - 0.1f * FRAME_HEIGHT)
#define TEXT_LIFE_POINTS_POS_X	(FRAME_POS_X + 0.2f * FRAME_WIDTH)
#define TEXT_LIFE_POINTS_POS_Y	(FRAME_POS_Y - 0.1f * FRAME_HEIGHT)

#define TEXT_PAUSED_RES_FACTOR_X			(0.35f)
#define TEXT_PAUSED_RES_FACTOR_Y			(0.3f)
#define TEXT_GAMEOVER_RES_FACTOR_X			(0.275f)
#define TEXT_GAMEOVER_RES_FACTOR_Y			(0.3f)
#define TEXT_COUNTDOWN_DISPLACEMENT_FACTOR	(0.01f)
#define TEXT_COUTNDOWN_RES_FACTOR_X			(0.47f)
#define TEXT_COUNTDOWN_RES_FACTOR_Y			(0.3f)
#define TEXT_START_DISPLACEMENT_FACTOR		(0.06f)
#define TEXT_START_RES_FACTOR_X				(0.325f)
#define TEXT_START_RES_FACTOR_Y				(0.3f)

#define TEXT_STATUS_SCALE (5.f)

#define CAMERA_SHAKE_AMPLITUDE_X (0.1f)
#define CAMERA_SHAKE_AMPLITUDE_Y (0.1f)
#define CAMERA_SHAKE_AMPLITUDE_Z (0.1f)

#define CAMERA_SHAKE_FREQUENCY_X (0.5f)
#define CAMERA_SHAKE_FREQUENCY_Y (0.5f)
#define CAMERA_SHAKE_FREQUENCY_Z (0.5f)

#define CAMERA_SHAKE_OFFSET_X (0.f)
#define CAMERA_SHAKE_OFFSET_Y (M_PI / 6)
#define CAMERA_SHAKE_OFFSET_Z (M_PI / 3)

typedef std::function<void(glm::vec3)> ObjectSpawner;
typedef std::vector<std::pair<ObjectSpawner, float>> SpawnPool;
typedef std::uniform_int_distribution<int> IntDist;
typedef std::uniform_real_distribution<float> FloatDist;

class Game {

private:
	enum class GameState { STARTING, PLAYING, PAUSED, ENDED };

	GameState state = GameState::STARTING;

	Student::Camera* camera = new Student::Camera();

	// game objects
	Player* player;
	std::vector<BasePlatform*> platforms;

	// HUD related
	FuelIndicatorBackground* fuelBackground = new FuelIndicatorBackground();
	FuelIndicatorForeground* fuelForeground = new FuelIndicatorForeground();
	Frame* frame = new Frame();

	float distanceTraveled = 0.f;

	// game logic
	float distToNextSpawn = 0.f;

	SpawnPool badPlatformSpawnPool;
	SpawnPool goodPlatformSpawnPool;
	FloatDist badPlatformSpawnWeightDist;
	FloatDist goodPlatformSpawnWeightDist;
	IntDist normalPlatformSpawnCountDist;
	IntDist platformSpawnColumnDist;

	float timelessMinSpeed = 0.f;
	float timelessMaxSpeed = 0.f;

	float prevGameTick = 1.f;
	float currGameTick = 1.f;

	float startDelta = 0;

	std::string currDifficulty = DIFFICULTY_EASY;

	void UpdateObjects(double deltaTimeSeconds);
	void UpdateDistanceTraveled(double deltaTimeSeconds);
	void UpdateStartAnimations(double deltaTimeSeconds);
	void UpdateDifficulty();

	void SpawnNewGamePlatforms();
	void NewGame();
	void EndGame();

	void LoadSettings(std::string difficulty);
	void AttemptSpawn();
	void SpawnPlatformFromPool(SpawnPool spawnPool, FloatDist dist, glm::vec3 location);
	glm::vec3 ComputeSpawnLocation(std::vector<int>& availableColumns);

	glm::vec3 GetCameraShake();

	void TogglePause();
	void Restart();
	void DecreaseGameTick();
	void IncreaseGameTick();

public:
	Game();
	~Game();

	void Update(double deltaTimeSeconds);

	void OnInputUpdate(WindowObject* window, float deltaTime, int mods);
	void OnKeyPress(int key, int mods);

	float GetFOV();
	float GetStartDelta();
	Student::Camera* GetCamera();

	std::vector<BaseObject*> GetObjectsToRender();
	std::vector<std::pair<std::string, glm::vec3>> GetTextsToRender();

	bool IsStarting();
	bool IsPlaying();
	bool IsPaused();
	bool IsEnded();

};