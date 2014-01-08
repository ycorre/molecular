/*
 * Text
 * Contains functions for displaying text
 */
#ifndef TEXT_H
#define TEXT_H
#include "drawable.h"

class Text: virtual public Drawable
{
	public:
	SDL_Color color;
	TTF_Font * font;
	string content;

	Text();
	Text(SDL_Color aColor, TTF_Font * aFont);
	Text(SDL_Color aColor, TTF_Font * aFont, int Xpos, int Ypos, int aWidth, int aHeight);
	void write(string aText);
	void update();
	void setColor(SDL_Color aColor);
	void setFont(TTF_Font * aFont);
	virtual int isText() {return TRUE;}
};

class AnimatedText: virtual public Text, virtual public AnimatedDrawable
{
	public:
		//AnimatedText(SDL_Color aColor, TTF_Font * aFont);
		AnimatedText();
		virtual int isText() {return TRUE;}
		//AnimatedText(int x, int y, string aName, string aText);
};

#endif

