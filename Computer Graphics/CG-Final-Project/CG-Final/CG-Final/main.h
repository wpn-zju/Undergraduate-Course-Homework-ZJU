#pragma once

void InitSingleBoxCollider(VECTOR3D pos, VECTOR3D scalar);

void InitBoxCollidersProperty();

void SetBoxColliderBoundary();

// Called for initiation
bool Init(void);

void DrawRect(GLuint texture);

void DrawSkybox();

// Called to draw scene
void Display();

// Called on window resize
void Reshape(int w, int h);

void Grab();

// Called when a key is pressed
void Keyboard(unsigned char key, int x, int y);

void KeyUp(unsigned char key, int x, int y);

void Idle();

void Mouse(int button, int state, int x, int y);

void Motion(int x, int y);

void UpdateCameraHoriMovement();

void UpdateView();

void DrawScene();

int main(int argc, char** argv);