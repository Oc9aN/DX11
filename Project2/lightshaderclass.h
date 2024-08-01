#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

/////////////
// GLOBALS //
/////////////
const int NUM_LIGHTS = 4;

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: LightShaderClass
////////////////////////////////////////////////////////////////////////////////
class LightShaderClass
{
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };
    struct CameraBufferType
    {
        XMFLOAT3 cameraPostion;
        float padding;
    };
    struct LightColorBufferType
    {
        XMFLOAT4 ambientColor[NUM_LIGHTS];
        XMFLOAT4 diffuseColor[NUM_LIGHTS];
        XMFLOAT4 specularColor[NUM_LIGHTS];
        XMFLOAT4 specularPower[NUM_LIGHTS];
    };
    struct LightPositionBufferType
    {
        XMFLOAT4 lightPosition[NUM_LIGHTS];
    };

public:
    LightShaderClass();
    LightShaderClass(const LightShaderClass&);
    ~LightShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4[], XMFLOAT4[], XMFLOAT4[], XMFLOAT3, XMFLOAT4[], float[]);

private:
    bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4[], XMFLOAT4[], XMFLOAT4[], XMFLOAT3, XMFLOAT4[], float[]);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_cameraBuffer;
    ID3D11Buffer* m_lightColorBuffer;
    ID3D11Buffer* m_lightPositionBuffer;
};

#endif