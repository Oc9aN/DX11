#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")           // Direct3D 그래픽 설정 및 그리는 기능을 포함하는 라이브러리
#pragma comment(lib, "dxgi.lib")            // 하드웨어 정보를 얻는 라이브러리 (새로고침 빈도, 비디오 카드 정보 등)
#pragma comment(lib, "d3dcompiler.lib")     // 셰이더 컴파일 라이브러리

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>                          // DirectX 타입 헤더
#include <directxmath.h>                    // DirectX 수학 계산 헤더
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
//뷰 매트릭스는 카메라 클래스에서 넣을 예정
class D3DClass
{
public:
    D3DClass();
    D3DClass(const D3DClass&);
    ~D3DClass();

    //초기화와 종료 기능 *중요*
    ///summary>넓이, 높이, 새로고침 빈도에 따라 렌더링할지 or 최대한 빠르게할지, 윈도우 핸들러, 전체화면 유무, 3D환경의 깊이 depth and near</summary>
    bool Initialize(int, int, bool, HWND, bool, float, float);
    void Shutdown();

    void BeginScene(float, float, float, float);
    void EndScene();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    void GetProjectionMatrix(XMMATRIX&);
    void GetWorldMatrix(XMMATRIX&);
    void GetOrthoMatrix(XMMATRIX&);

    void GetVideoCardInfo(char*, int&);

    void SetBackBufferRenderTarget();
    void ResetViewport();

private:
    bool m_vsync_enabled;
    int m_videoCardMemory;
    char m_videoCardDescription[128];
    IDXGISwapChain* m_swapChain;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState* m_rasterState;
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_worldMatrix;
    XMMATRIX m_orthoMatrix;
    D3D11_VIEWPORT m_viewport;
};

#endif