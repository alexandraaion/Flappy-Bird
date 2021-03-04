#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;
float birdY;
float animationY;
float rectangleSide;
float squareSide;
double idxScaleMin = 0.5;
double idxScaleMax = 2.0;
bool checkPrint = true;
bool checkFinish = false;
int score = 0;
double diff = idxScaleMax - idxScaleMin;
bool spacePress;
float coordBirdX = 100;	//coordonata pasarii pe OX
float coordBirdY;		//coordonata pasarii pe OY
int nrObs = 4;			//nr de obstacole din scena

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	squareSide = 20;					//latura corp pasare (patrat)
	rectangleSide = 120;				//dimensiune obstacol pe OX
	birdY = 4 * squareSide;				//dimensiunea pasarii pe OY 
	animationY = 2 * rectangleSide;		//dimensiune obstacol pe OY
	spacePress = false;
	float spaceSide = (resolution.x - (nrObs - 1) * rectangleSide) / nrObs;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	
	// initialize angularStep
	angularStep = 0;
	

	Mesh* bird = Object2D::CreateBird("bird", corner, squareSide, glm::vec3(0.50, 0.0, 0.6), true);
	AddMeshToList(bird);

	Mesh* rectangle = Object2D::CreateRectangle("rectangle", corner, rectangleSide, glm::vec3(0.30, 0.0, 0.05), true);
	AddMeshToList(rectangle);

	for (int i = 0; i < nrObs; i++) {			//vector de rezolutii la care sa cream obstacolele
		float aux = resolution.x + i * (rectangleSide + spaceSide) ;
		res.push_back(aux);
		float aux1 = ((double)rand() / RAND_MAX) * diff + idxScaleMin;
		idxScale.push_back(aux1);
		transX.push_back(0);
	}

	coordBirdY = resolution.y / 2;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(coordBirdX, coordBirdY);
	RenderMesh2D(meshes["bird"], shaders["VertexColor"], modelMatrix);

}

void Tema1::FrameStart()
{
	glClearColor(1, 0.8, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();
	
	if (spacePress == false) {
		coordBirdY -= deltaTimeSeconds * 100;
		if (angularStep > -0.785398) {
			angularStep -= deltaTimeSeconds * 0.2f;
		}
	}
	else {
		coordBirdY += deltaTimeSeconds * 200;
		if (angularStep <  0.785398) {
			angularStep += deltaTimeSeconds * 0.4f;
		}
		spacePress = false;
	}

	for (int i = 0; i < nrObs; i++) {
		transX[i] += deltaTimeSeconds * 100;
	}

	if (!checkFinish) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(coordBirdX, coordBirdY);
		modelMatrix *= Transform2D::Translate(-2 * squareSide, -5.5 * squareSide);
		modelMatrix *= Transform2D::Rotate(angularStep);
		modelMatrix *= Transform2D::Translate(2 * squareSide, 5.5 * squareSide);
		RenderMesh2D(meshes["bird"], shaders["VertexColor"], modelMatrix);



		for (int i = 0; i < nrObs; i++) {

			idxScaleUp = (resolution.y - idxScale[i] * animationY - 3 * birdY) / animationY;
			if (idxScaleUp < idxScaleMin) {
				idxScale[i] = ((double)rand() / RAND_MAX) * diff + idxScaleMin;
				idxScaleUp = (resolution.y - idxScale[i] * animationY - 3 * birdY) / animationY;
			}

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(res[i] - transX[i], 0);
			modelMatrix *= Transform2D::Scale(1, idxScale[i]);

			if (res[i] - transX[i] <= -rectangleSide) {
				res[i] = resolution.x;
				transX[i] = 0;
				if (checkPrint == true) {
					score++;
					printf("SCORUL ESTE: %d\n", score);
				}
				idxScale[i] = ((double)rand() / RAND_MAX) * diff + idxScaleMin;
			}
			RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(res[i] + rectangleSide - transX[i], resolution.y);
			modelMatrix *= Transform2D::Rotate(3.14);
			modelMatrix *= Transform2D::Scale(1, idxScaleUp);

			RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

			if ((coordBirdY <= 0 || coordBirdY >= resolution.y) && !checkFinish) {
				checkPrint = false;
				checkFinish = true;
				printf("GAME OVER!!\n");
				printf("SCORUL LA CARE ATI AJUNS ESTE: %d\n", score);
			}

		}
	}


}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		spacePress = true;
	}
	
}

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		spacePress = true;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
