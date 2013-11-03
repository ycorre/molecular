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
	oglTexture = NULL;
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
	oglTexture = NULL;
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
	texture = TTF_RenderUTF8_Blended(font, aText.c_str(), color);
	if (texture == NULL)
	{
		cerr << "TTF_RenderUTF8_Blended() Failed: " << TTF_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}

	this->width = this->texture->w;
	this->height = this->texture->h;

	if(this->oglTexture == NULL)
	{
		createOGLTexture();
	}
	else
	{
		glDeleteTextures(1, &this->oglTexture);
		createOGLTexture();
	}
}

//Used when changes are made on the text elements
//(e.g. the color of the text)
void Text::update()
{
	texture =  TTF_RenderUTF8_Blended(font, content.c_str(), color);
	glDeleteTextures(1, &this->oglTexture);
	createOGLTexture();
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
