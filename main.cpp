#include "D3D11_Framework.h"

using namespace D3D11Framework;

class MyInput : public InputListener
{
public:
	bool KeyPressed(const KeyEvent& arg)
	{
		printf("key press %c\n", arg.wc);
		return false;
	}
	bool MouseMove(const MouseEvent& arg)
	{
		printf("mouse %d - %d\n", arg.x, arg.y);
		return false;
	}
};




int main()
{
	Framework framework;

	Render* render = new Render();

	framework.SetRender(render);
	framework.Init();

	framework.Run();

	framework.Close();

	return 0;

	return 0;
}