#include "Tema2.h"

Tema2::Tema2() {}

Tema2::~Tema2() {}

void Tema2::Init() {

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("trapezoid");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "trapezoid.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("Tema2");
		shader->AddShader("Source/Laboratoare/Tema2/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	initialResolution = window->GetResolution();

	projectionMatrix = glm::perspective(RADIANS(game->GetFOV()), window->props.aspectRatio, 0.01f, 200.0f);

}

void Tema2::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glm::vec3 clearColor = GAME_CLEAR_COLOR;
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currResolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, currResolution.x, currResolution.y);

	if (currResolution != oldResolution) {
		printf("diff res\n");
		delete textRenderer;
		textRenderer = new TextRenderer(currResolution.x, currResolution.y);
		textRenderer->Load("Source/TextRenderer/Fonts/Arial.ttf", 18);

		oldResolution = currResolution;
	}
}

void Tema2::Update(float deltaTimeSeconds) {

	projectionMatrix = glm::perspective(RADIANS(game->GetFOV()), window->props.aspectRatio, 0.01f, 200.0f);
	viewMatrix = game->GetCamera()->GetViewMatrix();

	game->Update(deltaTimeSeconds);

	for (auto object : game->GetObjectsToRender())
		RenderSimpleMesh(object);

}

void Tema2::FrameEnd() {

	glm::vec2 scaleVec = glm::vec2(currResolution) / glm::vec2(initialResolution);
	float scale = (scaleVec.x + scaleVec.y) / 2;

	for (auto textData : game->GetTextsToRender())
		RenderText(textData, scale);

	if (game->IsPaused()) {

		textRenderer->RenderText("PAUSED", TEXT_PAUSED_RES_FACTOR_X * currResolution.x,
			TEXT_PAUSED_RES_FACTOR_Y * currResolution.y, scale * TEXT_STATUS_SCALE);

	} else if (game->IsEnded()) {

		textRenderer->RenderText("GAME OVER", TEXT_GAMEOVER_RES_FACTOR_X * currResolution.x,
			TEXT_GAMEOVER_RES_FACTOR_Y * currResolution.y, scale * TEXT_STATUS_SCALE);

	} else if (game->IsStarting()) {

		float startDelta = game->GetStartDelta();
		int startCount = ceil(startDelta);
		float startCountScale = 1.5f - (startCount - startDelta) / 2;

		float displacement = TEXT_COUNTDOWN_DISPLACEMENT_FACTOR * (startCount - startDelta);

		std::string timer = std::to_string(startCount);
		textRenderer->RenderText(timer, (TEXT_COUTNDOWN_RES_FACTOR_X + displacement) * currResolution.x,
			TEXT_COUNTDOWN_RES_FACTOR_Y * currResolution.y, startCountScale * TEXT_STATUS_SCALE);

	} else if (game->IsPlaying()) {

		float startDelta = game->GetStartDelta();

		if (startDelta >= -1) {
			int startCount = ceil(startDelta);
			float startCountScale = 1.5f - (startCount - startDelta) / 2;

			float displacement = TEXT_START_DISPLACEMENT_FACTOR * (startCount - startDelta);

			textRenderer->RenderText("START", (TEXT_START_RES_FACTOR_X + displacement) * currResolution.x,
				TEXT_START_RES_FACTOR_Y * currResolution.y, startCountScale * TEXT_STATUS_SCALE);
		}

	}

}

void Tema2::RenderText(std::pair<std::string, glm::vec3>& textData, float scale) {

	std::string text = textData.first;
	glm::vec3 position = textData.second;

	auto hudToResX = MathUtils::MapLinear(-1, 1, 0, currResolution.x);
	auto hudToResY = MathUtils::MapLinear(-1, 1, 0, currResolution.y);

	textRenderer->RenderText(text, hudToResX(position.x), currResolution.y - hudToResY(position.y), scale);

}

void Tema2::RenderSimpleMesh(BaseObject* object) {

	Mesh* mesh = meshes[object->GetMeshName()];
	Shader* shader = shaders["Tema2"];

	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(object->GetMeshColor()));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(object->GetModelMatrix()));

	// Bind view matrix
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Bind HUD element identifier
	int loc_is_hud_element = glGetUniformLocation(shader->program, "is_hud_element");
	glUniform1i(loc_is_hud_element, object->IsHudElement());

	// Bind deformation type
	int loc_deformation_type = glGetUniformLocation(shader->program, "deformation_type");
	glUniform1i(loc_deformation_type, object->GetDeformation());

	// Bind deformation delta
	int loc_deformation_delta = glGetUniformLocation(shader->program, "deformation_delta");
	glUniform1f(loc_deformation_delta, object->GetDeformationDelta());

	// Bind deformation duration
	int loc_deformation_duration = glGetUniformLocation(shader->program, "deformation_duration");
	glUniform1f(loc_deformation_duration, object->GetDeformationDuration());

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
	game->OnInputUpdate(window, deltaTime, mods);
}

void Tema2::OnKeyPress(int key, int mods) {
	game->OnKeyPress(key, mods);
}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}