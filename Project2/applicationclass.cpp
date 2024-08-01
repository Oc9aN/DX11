////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Lights = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128];
	char textureFilename[128];
	bool result;

	// Create and initialize the Direct3D object
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Cloud not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -12.0f);

	// 모델 데이터 읽어오기
	strcpy_s(modelFilename, "../Engine/data/sphere.txt");

	// Create and initialize the model object.
	m_Model = new ModelClass;

	// Set the name of the texture file that we will be loading.
	strcpy_s(textureFilename, "../Engine/data/stone01.tga");
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), textureFilename, modelFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light shader object.
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the number of lights we will use.
	m_numLights = 4;

	// Create and initialize the light objects array.
	m_Lights = new LightClass[NUM_LIGHTS];

	// 빛의 색과 방향을 설정
	m_Lights[0].SetAmbientColor(0.15f, 0.0f, 0.0f, 1.0f);
	m_Lights[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Lights[0].SetSpecularColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Lights[0].SetSpecularPower(32.0f);
	m_Lights[0].SetPosition(-10.0f, 0.0f, -10.0f);
	m_Lights[0].SetAttenuation(0.0f, 0.0f, 0.1f, 1.0f);

	m_Lights[1].SetAmbientColor(0.0f, 0.15f, 0.0f, 1.0f);
	m_Lights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Lights[1].SetSpecularColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Lights[1].SetSpecularPower(32.0f);
	m_Lights[1].SetPosition(3.0f, 0.0f, -3.0f);
	m_Lights[1].SetAttenuation(0.0f, 0.0f, 0.1f, 1.0f);

	m_Lights[2].SetAmbientColor(0.0f, 0.0f, 0.15f, 1.0f);
	m_Lights[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Lights[2].SetSpecularColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Lights[2].SetSpecularPower(32.0f);
	m_Lights[2].SetPosition(0.0f, 3.0f, -3.0f);
	m_Lights[2].SetAttenuation(0.0f, 0.0f, 0.1f, 1.0f);

	m_Lights[3].SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Lights[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[3].SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[3].SetSpecularPower(32.0f);
	m_Lights[3].SetPosition(0.0f, -3.0f, -3.0f);
	m_Lights[3].SetAttenuation(0.0f, 0.0f, 0.1f, 1.0f);

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the light object.
	if (m_Lights)
	{
		delete [] m_Lights;
		m_Lights = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	// Release the Direct3D object
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}


bool ApplicationClass::Frame()
{
	static float rotation = 0.0f;
	bool result;

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.3f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	//Render the graphics scene
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}


bool ApplicationClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotateMatrix;
	XMFLOAT4 diffuseColor[NUM_LIGHTS], lightPosition[NUM_LIGHTS], ambientColor[NUM_LIGHTS], specularColor[NUM_LIGHTS], attenuation[NUM_LIGHTS];
	float SpecularPower[NUM_LIGHTS];
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	rotateMatrix = XMMatrixRotationY(rotation); // 로테이션 매트릭스 생성

	// 두 행렬을 곱해서 이동값을 가진 행렬을 생성 후 월드 매트릭스에 대입
	//worldMatrix = XMMatrixMultiply(worldMatrix, rotateMatrix);

	// Get the light properties.
	for (int i = 0; i < m_numLights; i++)
	{
		// Create the diffuse color array from the four light colors.
		diffuseColor[i] = m_Lights[i].GetDiffuseColor();

		// Create the light position array from the four light positions.
		lightPosition[i] = m_Lights[i].GetPosition();

		// 4개 조명의 스펙큘러 컬러 배열 생성
		specularColor[i] = m_Lights[i].GetSpecularColor();

		// 4개 조명의 엠비언트 컬러 배열 생성
		ambientColor[i] = m_Lights[i].GetAmbientColor();

		// 4개 조명의 스펙큘러 파워 배열 생성
		SpecularPower[i] = m_Lights[i].GetSpecularPower();

		// 4개 조명의 감쇠 계수 배열 생성
		attenuation[i] = m_Lights[i].GetAttenuation();
	}

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		lightPosition, diffuseColor, ambientColor, m_Camera->GetPosition(), specularColor, SpecularPower, attenuation);
	if (!result)
	{
		return false;
	}
	

	//Present the rendered scene to the screen
	m_Direct3D->EndScene();
	return true;
}
