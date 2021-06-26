#include "D3D11_Framework.h"
#include"Textures.h"
#include"Light.h"
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
	//решение с текстурой
	/*{Framework framework;

	Textures* render = new Textures();
	FrameworkDesc desc;
	desc.render = render;

	framework.Init(desc);

	framework.Run();

	framework.Close(); }*/
	// решение со светом
	{Framework framework;

	Light* render = new Light();

	FrameworkDesc desc;
	desc.render = render;

	framework.Init(desc);

	framework.Run();

	framework.Close(); }

	system("pause");
	return 0;
}