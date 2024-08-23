#ifndef _SPRITECLASS_H_
#define _SPRITECLASS_H_
/////////////
// INCLUDES //
//////////////
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SpriteClass
////////////////////////////////////////////////////////////////////////////////
class SpriteClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    SpriteClass();
    SpriteClass(const SpriteClass&);
    ~SpriteClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, char*, int, int);
    void Shutdown();
    bool Render(ID3D11DeviceContext*);
    void Update(float); //매 프레임마다 실행해서 프레임 시간 체크

    int GetIndexCount();
    ID3D11ShaderResourceView* GetTexture();

    void SetRenderLocation(int, int);

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    bool UpdateBuffers(ID3D11DeviceContext*);
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, char*); //파일 이름으로 그려줄 sprite가져오기
    void ReleaseTextures();

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount, m_screenWidth, m_screenHeight, m_bitmapWidth, m_bitmapHeight, m_renderX, m_renderY, m_prevPosX, m_prevPosY;
    TextureClass* m_Textures;
    float m_frameTime, m_cycleTime;
    int m_currentTexture, m_textureCount;
};
#endif