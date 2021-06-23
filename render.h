#pragma once
#include"header.h"
namespace D3D11Framework
{
	

	class Render
	{
	public:
		Render();
		virtual ~Render();

		virtual bool Init(HWND hwnd) ;
		virtual bool Draw();
		virtual void Close();
		
	private:
		D3D_DRIVER_TYPE m_driverType;
		D3D_FEATURE_LEVEL m_featureLevel;
		ID3D11Device* m_pd3dDevice;
		ID3D11DeviceContext* m_pImmediateContext;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
	};

	
}