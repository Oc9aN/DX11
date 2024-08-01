#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
    m_ambientColor = XMFLOAT4(red, green, blue, alpha);
    return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
    return;
}


void LightClass::SetDirection(float x, float y, float z)
{
    m_direction = XMFLOAT3(x, y, z);
    return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
    m_specularColor = XMFLOAT4(red, green, blue, alpha);
    return;
}

void LightClass::SetSpecularPower(float power)
{
    m_specularPower = power;
    return;
}

void LightClass::SetPosition(float x, float y, float z)
{
    m_position = XMFLOAT4(x, y, z, 1.0f);
    return;
}

void LightClass::SetAttenuation(float a0, float a1, float a2, float padding)
{
    m_attenuation = XMFLOAT4(a0, a1, a2, padding);
    return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
    return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
    return m_diffuseColor;
}


XMFLOAT3 LightClass::GetDirection()
{
    return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
    return m_specularColor;
}

float LightClass::GetSpecularPower()
{
    return m_specularPower;
}

XMFLOAT4 LightClass::GetPosition()
{
    return m_position;
}

XMFLOAT4 LightClass::GetAttenuation()
{
    return m_attenuation;
}