#pragma once

#include <TextRenderer/TextRenderer.h>
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <sstream>
#include <fstream>

#include "Game.h"

#define GAME_CLEAR_COLOR	(COLOR_DARK_SEA_GREEN)

class Tema2 : public SimpleScene {

public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(BaseObject *object);
	void RenderText(std::pair<std::string, glm::vec3>& textData, float scale);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	Game* game = new Game();

	glm::ivec2 initialResolution = glm::ivec2(0);
	glm::ivec2 oldResolution = glm::ivec2(0);
	glm::ivec2 currResolution = glm::ivec2(0);

	TextRenderer* textRenderer = new TextRenderer(currResolution.x, currResolution.y);
};