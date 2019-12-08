#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "p2SString.h"
#include "j1Fonts.h"
#include "UI.h"

//class j1Fonts;

class UI_Text : public UI
{
public:

	UI_Text(UI_Element type, int x, int y, SDL_Rect hitbox, _TTF_Font* font, SDL_Color fontColour, UI_Text* elementCallback, UI* parent = nullptr, p2SString* string = nullptr, p2SString* hoverString = nullptr,
			p2SString* focusString = nullptr, p2SString* leftClickString = nullptr, p2SString* rightClickString = nullptr);

	bool Draw();

private:
	p2SString*		string;				//String of the UI Text element
	_TTF_Font*		font;				//Loaded font.
	SDL_Color		font_colour;		//Colour of the font.
	SDL_Texture*	idleTex;			//Texture for the idle state of the text.
	SDL_Texture*	hoverTex;			//Texture for the hover state of the text.
	SDL_Texture*	focusTex;			//Texture for the focused state fo the text.
	SDL_Texture*	leftClickTex;		//Texture for the left-Clicked state of the text.
	SDL_Texture*	rightClickTex;		//Texture for the right-Clicked state of the text.
	SDL_Texture*	currentTex;			//Current texture to be blitted. Depends on the input the UI Text element receives.

	UI_Text*		textCallback;		//Pointer of the text, if we want to assign a function to a specific button, the pointer needs to be compared
};

#endif // !__UI_TEXT_H__