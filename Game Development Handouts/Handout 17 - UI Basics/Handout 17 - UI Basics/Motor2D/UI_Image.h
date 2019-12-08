#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

//#include "j1Textures.h"
#include "UI.h"			//UI_Image is included in UI.h (?).

class UI_Image : public UI
{
public:
	UI_Image(UI_Element element, int x, int y, SDL_Rect imgRect, UI_Image* elementCallback, UI* parent = nullptr);

	bool Draw();

private:
	SDL_Texture*	tex;			//UI image texture.
	UI_Image*		imgCallback;	//Pointer of the image, if we want to assign a function to a specific button, the pointer needs to be compared
};

#endif // !__UI_IMAGE_H__