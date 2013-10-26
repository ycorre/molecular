/*
 * Handle the text elements in SDL
 */

#include "text.h"

Text::Text(SDL_Color aColor, TTF_Font * aFont)
{
	Drawable();
	color = aColor;
	font = aFont;
	content = "";
}

Text::Text(SDL_Color aColor, TTF_Font * aFont, int Xpos, int Ypos, int aWidth, int aHeight)
{
	Drawable();
	posX = Xpos;
	posY = Ypos;
	width = aWidth;
	height = aHeight;
	color = aColor;
	font = aFont;
	content = "";
}

//Specify the text to be display by the Text element
void Text::write(string aText)
{
	//Protection from crash when writing an empty string;
	if(aText.empty())
	{
		cout<< "Warning: Attempting to write an empty string" << endl;
		aText = " ";
	}

	content = aText;
	texture = TTF_RenderText_Solid(font, aText.c_str(), color);
	if (texture == NULL)
	{
		cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}
}

//Used when changes are made on the text elements
//(e.g. the color of the text)
void Text::update()
{
	texture = TTF_RenderText_Solid(font, content.c_str(), color);
}

void Text::setColor(SDL_Color aColor)
{
	color = aColor;
	update();
}

void Text::setFont(TTF_Font * aFont)
{
	font = aFont;
	update();
}
