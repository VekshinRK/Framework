#pragma once
#include "Textures.h"
#include<DirectXMath.h>
#include <d3dcompiler.h>
#include<d3d11shader.h>
#include"WICTextureLoader.h"


using namespace DirectX;
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct ConstantBuffer
{
	XMMATRIX  WVP;
};

Textures::Textures()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pConstantBuffer = nullptr;

	m_pTextureRV = nullptr;
	m_pSamplerLinear = nullptr;
	m_rot = 0.01;
}

bool Textures::Init(HWND hwnd)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader_texture.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		_RELEASE(pVSBlob);
		return false;
	}
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
	_RELEASE(pVSBlob);
	if (FAILED(hr))
		return false;

	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

	ID3DBlob* pPSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader_texture.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	_RELEASE(pPSBlob);
	if (FAILED(hr))
		return false;

	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return false;

	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
		return false;
	
	
	CreateWICTextureFromFile(m_pd3dDevice, L"texture.png", NULL, &m_pTextureRV);
	
	/*hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, L"texture.png", NULL, NULL, &m_pTextureRV, NULL);
	if (FAILED(hr))
		return false;*/

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);
	if (FAILED(hr))
		return false;
	m_World1 = XMMatrixIdentity();
	m_World2 = XMMatrixIdentity();

	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(Eye, At, Up);

	m_Projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)1080 / 960, 0.1f, 10000.0f);

	return true;
}
bool Textures::Draw()
{

	SolarSystem();
	

	return true;
}
void Textures::Close()
{
	_RELEASE(m_pConstantBuffer);
	_RELEASE(m_pVertexBuffer);
	_RELEASE(m_pIndexBuffer);
	_RELEASE(m_pVertexLayout);
	_RELEASE(m_pVertexShader);
	_RELEASE(m_pPixelShader);
	_RELEASE(m_pTextureRV);
	_RELEASE(m_pSamplerLinear);
}

void Textures::SolarSystem()
{


	m_rot += .0005f;
	if (m_rot > 6.26f)
		m_rot = 0.0f;

	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, m_rot);
	XMMATRIX RotationSputnik = XMMatrixRotationAxis(rotaxis, m_rot);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
	XMMATRIX TranslationSputnik = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
	m_World1 = Translation * Rotation;
	
	Rotation = XMMatrixRotationAxis(rotaxis, -m_rot);
	XMMATRIX Scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	XMMATRIX ScaleSputnik = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	
	XMMATRIX WVP = m_World1 * m_View * m_Projection;
	ConstantBuffer cb;
	m_World2 = Rotation * Scale;
	WVP = m_World2 * m_View * m_Projection;
	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);
	


	// 1 спутник
	Translation = XMMatrixTranslation(0.35f, -0.35, 0.35f);
	rotaxis = XMVectorSet(0.2f, 0.5f, 0.3f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot);
	m_World1 = XMMatrixScaling(0.05f, 0.05f, 0.05f) * Translation * Rotation ;
	WVP = m_World1 * m_View * m_Projection;

	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	//// 2 планета


	
	

	rotaxis = XMVectorSet(0.5f, 0.3f, 0.0f, 0.0f);
	Translation = XMMatrixTranslation(2.0f, 1.0, 4.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot);


	m_World_sputnik = Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot);
	m_World1 = XMMatrixScaling(0.6f, 0.6f, 0.6f)* Translation  * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f) ;
	
	WVP = m_World1 * m_View * m_Projection;

	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);
	
	// 2 спутник
	
	m_World_sputnik *= Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	WVP = m_World_sputnik * m_View * m_Projection;
	m_World_sputnik *= Rotation;
	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);


	//// 3 планета
	m_rot2 += .0003f;
	if (m_rot2 > 6.26f)
		m_rot2 = 0.0f;




	rotaxis = XMVectorSet(0.0f,1.0f, -0.5f, 0.0f);
	Translation = XMMatrixTranslation(6.0f, 2.0, 7.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot2);


	m_World_sputnik = Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot2);
	m_World1 = XMMatrixScaling(0.5f, 0.5f, 0.5f) * Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);

	WVP = m_World1 * m_View * m_Projection;

	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// 3 спутник

	m_World_sputnik *= Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	WVP = m_World_sputnik * m_View * m_Projection;
	m_World_sputnik *= Rotation;
	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);


	//// 4 планета
	m_rot3 += .0003f;
	if (m_rot3 > 6.26f)
		m_rot3 = 0.0f;




	rotaxis = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	Translation = XMMatrixTranslation(12.0f, -5.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot3);


	m_World_sputnik = Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	rotaxis = XMVectorSet(0.1f, 0.2f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot3);
	m_World1 = XMMatrixScaling(0.5f, 0.5f, 0.5f) * Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);

	WVP = m_World1 * m_View * m_Projection;

	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// 4 спутник

	m_World_sputnik *= Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	WVP = m_World_sputnik * m_View * m_Projection;
	m_World_sputnik *= Rotation;
	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);



	//// 5 планета
	m_rot4 += .001f;
	if (m_rot4 > 6.26f)
		m_rot4 = 0.0f;




	rotaxis = XMVectorSet(-0.8f, -0.9f, -0.3f, 0.0f);
	Translation = XMMatrixTranslation(16.0f, -3.0f, -2.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot3);


	m_World_sputnik = Translation * Rotation * XMMatrixScaling(0.1f, 0.1f, 0.1f);
	rotaxis = XMVectorSet(-0.1f,0.2f, 0.1f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, m_rot3);
	m_World1 = XMMatrixScaling(0.9f, 0.9f, 0.9f) * Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);

	WVP = m_World1 * m_View * m_Projection;

	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// 5 спутник

	m_World_sputnik *= XMMatrixScaling(2.3f, 2.3f, 2.3f)*Translation * Rotation * XMMatrixScaling(0.3f, 0.3f, 0.3f);
	WVP = m_World_sputnik * m_View * m_Projection;
	m_World_sputnik *= Rotation;
	cb.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	m_pImmediateContext->DrawIndexed(36, 0, 0);
































	//////3 планета
	//rotaxis = XMVectorSet(-0.1f, 1.0f, 0.0f, 0.0f);
	//m_rot1 += .0009f;
	//if (m_rot1 > 6.26f)
	//	m_rot1 = 0.0f;
	//
	//Rotation = XMMatrixRotationAxis(rotaxis, m_rot1);
	//Translation = XMMatrixTranslation(4.0f, -2.0f, 8.0f);
	//m_World1 = Translation * Rotation * Scale;
	//WVP = m_World1 * m_View * m_Projection;

	//cb.WVP = XMMatrixTranspose(WVP);
	//m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	//m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	//m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	//m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	//m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);

	//////4 планета
	//rotaxis = XMVectorSet(0.2f, 1.0f, 0.3f, 0.0f);
	//m_rot2 += .0003f;
	//if (m_rot2 > 6.26f)
	//	m_rot2 = 0.0f;
	//Rotation = XMMatrixRotationAxis(rotaxis, m_rot2);
	//Translation = XMMatrixTranslation(2.0f, 0.0f,10.0f);
	// m_World1 = Translation * Rotation * Scale;
	//WVP = m_World1 * m_View * m_Projection;

	//cb.WVP = XMMatrixTranspose(WVP);
	//m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	//m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	//m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	//m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	//m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);

	//////5 планета
	//

	//rotaxis = XMVectorSet(0.1f, 1.0f, 0.0f, 0.0f);
	//Rotation = XMMatrixRotationAxis(rotaxis, m_rot);

	//Translation = XMMatrixTranslation(5.0f, 4.0f, 14.0f);
	//m_World1 = Translation * Rotation * Scale;
	//WVP = m_World1 * m_View * m_Projection;

	//cb.WVP = XMMatrixTranspose(WVP);
	//m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	//m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	//m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	//m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	//m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);



}


