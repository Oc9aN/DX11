#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	m_obj = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	bool result;

	// 모델 불러오기
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// 모델 해제
	ReleaseModel();
	// Release the model texture.
	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	//간단한 삼각형을 그리므로 버퍼에 점을 수동으로 입력함
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array.
	// 버퍼를 채우기 위한 임시 버퍼
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	// 정점 버퍼 설정
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	// 정점에 대한 정보 설정
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	// 정점 버퍼 생성
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	// 인덱스 버퍼 설정
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	// 인덱스 정보 설정
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	// 인덱스 버퍼 생성
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	// 사용이 끝난 배열 해제
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	m_Texture = new TextureClass;

	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	bool result;
	char* type;

	// 타입 추정 후 모델 불러오기
	type = GetModelType(filename);

	if (strcmp(type, ".txt") == 0)
	{
		result = ParsingTxtModel(filename);
	}
	else if (strcmp(type, ".obj") == 0)
	{
		result = ParsingObjModel(filename);
	}

	return result;
}

bool ModelClass::ParsingTxtModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool ModelClass::ParsingObjModel(char* filename)
{
	ifstream fin;
	string line;
	XMFLOAT3 vertex;
	XMFLOAT2 uv;
	XMFLOAT3 nomal;
	float vi[4] = {-1,}, ti[4] = { -1, }, ni[4] = { -1, };
	string type;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	m_obj = new ObjType();

	// 데이터 읽어오기
	while (std::getline(fin, line))
	{
		if (!line.empty())
		{
			stringstream stream(line);
			stream >> type;
			if (type == "v")
			{
				stream >> vertex.x >> vertex.y >> vertex.z;
				m_obj->vertex.push_back(vertex);
			}
			else if (type == "vt")
			{
				stream >> uv.x >> uv.y;
				m_obj->uv.push_back(uv);
			}
			else if (type == "vn")
			{
				stream >> nomal.x >> nomal.y >> nomal.z;
				m_obj->nomal.push_back(nomal);
			}
			else if (type == "f")
			{
				//'/'을 공백으로 전환해서 데이터 대입
				string value;
				stringstream parse;
				while (getline(stream, value, '/'))
				{
					parse << value << " ";
				}
				parse >> vi[0] >> ti[0] >> ni[0]
					>> vi[1] >> ti[1] >> ni[1]
					>> vi[2] >> ti[2] >> ni[2]
					>> vi[3] >> ti[3] >> ni[3];
				m_obj->vertexIndex.push_back(vi[0]);
				m_obj->vertexIndex.push_back(vi[1]);
				m_obj->vertexIndex.push_back(vi[2]);
				m_obj->uvIndex.push_back(ti[0]);
				m_obj->uvIndex.push_back(ti[1]);
				m_obj->uvIndex.push_back(ti[2]);
				m_obj->nomalIndex.push_back(ni[0]);
				m_obj->nomalIndex.push_back(ni[1]);
				m_obj->nomalIndex.push_back(ni[2]);
				// 인덱스 형식이 trianglelist가 아닌 trianglefan일 경우, 즉 f에 관한 데이터가 4개인 경우
				// fan을 list형식으로 인덱스 순서를 추가해줌
				if (vi[3] != -1)
				{
					m_obj->vertexIndex.push_back(vi[0]);
					m_obj->vertexIndex.push_back(vi[2]);
					m_obj->vertexIndex.push_back(vi[3]);
					m_obj->uvIndex.push_back(ti[0]);
					m_obj->uvIndex.push_back(ti[2]);
					m_obj->uvIndex.push_back(ti[3]);
					m_obj->nomalIndex.push_back(ni[0]);
					m_obj->nomalIndex.push_back(ni[2]);
					m_obj->nomalIndex.push_back(ni[3]);
				}
			}
		}
	}

	// Close the model file.
	fin.close();

	// 모델 타입에 정보 대입
	m_vertexCount = m_obj->vertexIndex.size();
	m_indexCount = m_vertexCount;

	// 모델 타입 초기화
	m_model = new ModelType[m_vertexCount];

	for (int i = 0; i < m_indexCount; i++)
	{
		UINT vertexIndex = m_obj->vertexIndex[i] - 1;
		UINT uvIndex = m_obj->uvIndex[i] - 1;
		UINT nomalIndex = m_obj->nomalIndex[i] - 1;

		m_model[i].x = m_obj->vertex[vertexIndex].x;
		m_model[i].y = m_obj->vertex[vertexIndex].y;
		m_model[i].z = m_obj->vertex[vertexIndex].z;

		m_model[i].tu = m_obj->uv[uvIndex].x;
		m_model[i].tv = m_obj->uv[uvIndex].y;

		m_model[i].nx = m_obj->nomal[nomalIndex].x;
		m_model[i].ny = m_obj->nomal[nomalIndex].y;
		m_model[i].nz = m_obj->nomal[nomalIndex].z;
	}

	return true;
}

char* ModelClass::GetModelType(char* filename)
{
	//.을 기준으로 모델의 타입을 추정
	char type[6];
	char* fin;
	fin = strrchr(filename, '.');
	strcpy_s(type, fin);
	return type;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	// 데이터를 삼각형으로 그려줌 opgl에서 그려주는 타입 설정과 비슷한듯
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
