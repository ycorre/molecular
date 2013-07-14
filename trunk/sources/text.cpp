/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */

#include "text.h"

Text::Text(SDL_Color aColor, TTF_Font * aFont)
{
	Drawable();
	color = aColor;
	font = aFont;
	content = "";
}

void Text::write(string aText)
{
	content = aText;
	texture = TTF_RenderText_Solid(font, aText.c_str(), color);
	if (texture == NULL)
	{
		cerr << "TTF_RenderText_Solid() Failed: " << TTF_GetError() << "\n";
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}
}

//Used when for example the color of the text changes
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
