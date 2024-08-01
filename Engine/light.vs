/////////////
// DEFINES //
/////////////
#define NUM_LIGHTS 4

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

cbuffer LightPositionBuffer
{
    float4 lightPosition[NUM_LIGHTS];
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 lightPos[NUM_LIGHTS] : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    int i;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    // Calculate the normal vector against the world matrix only.
    // 노말 벡터를 월드 행렬과 계산
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    // 계산 후 혹시 크기가 변했을 수도 있기때문에 노말라이즈 
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    // 정점의 월드 위치를 구함
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    // 정점의 위치와 카메라의 위치를 빼서 뷰어 벡터를 구함
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    // 뷰어의 방향을 정규화
    output.viewDirection = normalize(output.viewDirection);

    for(i=0; i<NUM_LIGHTS; i++)
    {
        // 월드상의 정점 위치와 빛의 위치를 비교해서 정점에서의 빛의 위치를 설정 -> 기존 ps에서 lightDir역할
        output.lightPos[i] = lightPosition[i].xyz - worldPosition.xyz;

        // 빛의 위치 벡터를 초기화
        // output.lightPos[i] = normalize(output.lightPos[i]);
    }

    return output;
}