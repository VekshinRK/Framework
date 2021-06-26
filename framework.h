#pragma once
#include "window.h"
#include "render.h"
#include "input_mgr.h"
#include "log.h"

namespace D3D11Framework
{
	
	struct FrameworkDesc
	{
		DescWindow wnd;
		Render* render;
	};

	class Framework
	{
	public:
		Framework();
		~Framework();

		bool Init(const FrameworkDesc& desc);
		void Run();
		void Close();

		void SetRender(Render* render) { m_render = render; }
		void AddInputListener(InputListener* listener);
	protected:
		bool m_frame();

		FrameworkDesc m_desc;
		Window* m_wnd;
		Render* m_render;
		InputMgr* m_input;
		Log m_log;
		bool m_init;		// если было инициализировано
	};

	
}