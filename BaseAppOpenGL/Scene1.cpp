#include "Scene1.h"

CScene1::CScene1()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	pModelTerrain = NULL;
	pModelWater = NULL;
	pModelBridge = NULL;
	pModelMuro = NULL;
	pModelLighthouse = NULL;
	
	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();	

	// skybox textures
	pTextures->CreateTextureClamp(0, "../Scene1/back.bmp");
	pTextures->CreateTextureClamp(1, "../Scene1/front.bmp");
	pTextures->CreateTextureClamp(2, "../Scene1/down.bmp");
	pTextures->CreateTextureClamp(3, "../Scene1/up.bmp");
	pTextures->CreateTextureClamp(4, "../Scene1/left.bmp");
	pTextures->CreateTextureClamp(5, "../Scene1/right.bmp");

	pTextures->CreateTextureTGA(6, "../Scene1/cypress.tga");

	PointLightAmbient[0] = 1.0f;	PointLightAmbient[1] = 1.0f;	PointLightAmbient[2] = 1.0f;	PointLightAmbient[3] = 1.0f;
	PointLightDiffuse[0] = 1.0f;	PointLightDiffuse[1] = 1.0f;	PointLightDiffuse[2] = 1.0f;	PointLightDiffuse[3] = 1.0f;
	PointLightSpecular[0] = 1.0f;	PointLightSpecular[1] = 1.0f;	PointLightSpecular[2] = 1.0f;	PointLightSpecular[3] = 1.0f;
	PointLightPosition[0] = -1000.0f;
	PointLightPosition[1] = 500.0f;
	PointLightPosition[2] = 1000.0f;
	PointLightPosition[3] = 1.0f;

	fFogDensity = 0.01f;
	vFogColor[0] = 0.8f;
	vFogColor[1] = 0.8f;
	vFogColor[2] = 0.8f;
	vFogColor[3] = 1.0f;
	bIsFogEnabled = false;

	pModelTerrain = new CModel_3DS();
	pModelTerrain->Load("../Scene1/terrain.3DS");

	pModelWater = new CModel_3DS();
	pModelWater->Load("../Scene1/Water.3DS");

	pModelBridge = new CModel_3DS();
	pModelBridge->Load("../Scene1/bridge.3DS");

	pModelMuro = new CModel_3DS();
	pModelMuro->Load("../Scene1/Wall.3DS");

	pModelLighthouse = new CModel_3DS();
	pModelLighthouse->Load("../Scene1/lighthouse.3DS");

}


CScene1::~CScene1(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}	

	if (pModelTerrain)
	{
		delete pModelTerrain;
		pModelTerrain = NULL;
	}

	if (pModelWater)
	{
		delete pModelWater;
		pModelWater = NULL;
	}

	if (pModelBridge)
	{
		delete pModelBridge;
		pModelBridge = NULL;
	}

	if (pModelMuro)
	{
		delete pModelMuro;
		pModelMuro = NULL;
	}

	if (pModelLighthouse)
	{
		delete pModelLighthouse;
		pModelLighthouse = NULL;
	}
}




int CScene1::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClearColor(vFogColor[0], vFogColor[1], vFogColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	//DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	
	// d. Iluminação
	glLightfv(GL_LIGHT0, GL_AMBIENT, PointLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, PointLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, PointLightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, PointLightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// j. Neblina (fog)
	if (bIsFogEnabled) {
		glEnable(GL_FOG);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogfv(GL_FOG_COLOR, vFogColor);
		glFogf(GL_FOG_START, 20.0);
		glFogf(GL_FOG_END, 1000.0);
		glFogi(GL_FOG_MODE, GL_LINEAR);
	}
	else {
		DrawSkyBox(0.0f, 300.0f, 0.0f,
			2500.0f, 2500.0f, 2500.0f,
			pTextures);
	}

	// a. Modelagem com modo imediato e material
	/*MatAmbient[0] = 0.1745f;	MatAmbient[1] = 0.01175f;	MatAmbient[2] = 0.01175f;	MatAmbient[3] = 0.55f;
	MatDiffuse[0] = 0.61424f;	MatDiffuse[1] = 0.04136f;	MatDiffuse[2] = 0.04136f;	MatDiffuse[3] = 0.55f;
	MatSpecular[0] = 0.727811f;	MatSpecular[1] = 0.626959f;	MatSpecular[2] = 0.626959f;	MatSpecular[3] = 0.55f;
	MatShininess = 76.8f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);*/
	/*glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glEnd();
	glPopMatrix();*/

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pModelTerrain->Draw();
	glPopMatrix();

	glPushMatrix();
	pModelMuro->Draw();
	glPopMatrix();

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(5.0f, 136.0f, -175.0f);
	pModelLighthouse->Draw();
	glPopMatrix();

	/*glPushMatrix();
	glColor4ub(91, 163, 218, 255);
	glTranslatef(0.0f, 40.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-250.0f, 0.0f, 250.0f);
	glVertex3f(250.0f, 0.0f, 250.0f);
	glVertex3f(250.0f, 0.0f, -250.0f);
	glVertex3f(-250.0f, 0.0f, -250.0f);
	glEnd();
	glPopMatrix();*/

	// Mapeamento de textura em modelagem com modo imediato
	/*glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.0f, 0.0f, 0.0f);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 2.0f, 0.0f);
	glEnd();
	glPopMatrix();*/

	// b. Modelagem com 3DS Max
	/*glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	pModelX->Draw();
	glPopMatrix();*/

	// c. Modelo com mapeamento de textures Unrap UVW
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 8.0f, 315.0f);
	pModelBridge->Draw();
	glPopMatrix();


	// k. Transparência (blending)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.95f);

	DrawTree(42.0f, 45.0f, 20.0f); // 1
	DrawTree(45.0f, 52.0f, 0.0f); // 2
	DrawTree(50.0f, 58.0f, -20.0f); // 3
	DrawTree(55.0f, 64.0f, -40.0f); // 4
	DrawTree(60.0f, 70.0f, -60.0f); // 5
	DrawTree(65.0f, 76.0f, -80.0f); // 6
	DrawTree(70.0f, 85.0f, -100.0f); // 7
	DrawTree(72.0f, 89.0f, -120.0f); // 8
	DrawTree(73.0f, 94.0f, -140.0f); // 9
	DrawTree(76.0f, 97.0f, -160.0f); // 10
	DrawTree(76.0f, 100.0f, -180.0f); // 11
	DrawTree(75.0f, 107.0f, -200.0f); // 12
	DrawTree(70.0f, 112.0f, -220.0f); // 13
	DrawTree(58.0f, 114.0f, -233.0f); // 14
	DrawTree(45.0f, 116.0f, -245.0f); // 15
	DrawTree(30.0f, 118.0f, -258.0f); // 16
	DrawTree(10.0f, 118.0f, -258.0f); // 17
	DrawTree(-10.0f, 119.0f, -250.0f); // 18
	DrawTree(-30.0f, 121.0f, -237.0f); // 19
	DrawTree(-48.0f, 123.0f, -225.0f); // 20
	DrawTree(-55.0f, 125.0f, -205.0f); // 21
	DrawTree(-56.0f, 129.0f, -185.0f); // 22
	DrawTree(-53.0f, 132.0f, -165.0f); // 23
	DrawTree(-44.0f, 134.0f, -145.0f); // 24

	glDisable(GL_ALPHA_TEST);

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	pModelWater->Draw();
	glPopMatrix();

	/*glPushMatrix();
	glColor4f(0.222f, 0.352f, 0.652f, 0.8f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-500.0f, 0.0f, 500.0f);
	glVertex3f(500.0f, 0.0f, 500.0f);
	glVertex3f(500.0f, 0.0f, -500.0f);
	glVertex3f(-500.0f, 0.0f, -500.0f);
	glEnd();
	glPopMatrix();*/

	glDisable(GL_BLEND);

	if (bIsFogEnabled) {
		glDisable(GL_FOG);
	}
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}

	glRasterPos2f(10.0f, 20.0f);
	if (bIsFogEnabled)
		pTexto->glPrint("[ENTER] FOG: TRUE");
	else
		pTexto->glPrint("[ENTER] FOG: FALSE");

	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));


	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene1::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene1::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}



}

void CScene1::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		bIsFogEnabled = !bIsFogEnabled;
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene1::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene1::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}

void CScene1::DrawSkyBox(float x, float y, float z, float width, float height, float length, CTexture* pTextures)
{

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posição especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();

}

void CScene1::DrawTree(GLfloat x, GLfloat y, GLfloat z)
{
	pTextures->ApplyTexture(6);
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(5.0f, 5.0f, 5.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0, 10.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0, 10.0, 0.0);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 5.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 0.0, -5.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 10.0, -5.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 10.0, 5.0);
	glEnd();
	glPopMatrix();
}



