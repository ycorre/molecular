/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */
#ifndef TEXT_H
#define TEXT_H
#include "drawable.h"

class Text: public Drawable
{
	public:
	SDL_Color color;
	TTF_Font * font;
	string content;

	Text(SDL_Color aColor, TTF_Font * aFont);
	void write(string aText);
	void update();
	void setColor(SDL_Color aColor);
	void setFont(TTF_Font * aFont);
};

#endif

