////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Create the system object. 캡슐화 구조이기 때문에 system 객체를 생성
	System = new SystemClass;

	// Initialize and run the system object.
	result = System->Initialize();
	if (result)	//초기화가 정상적이면 메인 루프 실행
	{
		System->Run();
	}

	// Shutdown and release the system object. 마무리 작업
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}