/*
 * Handle the text elements in SDL
 */

#include "include/text.h"

Text::Text()
{
	name = "aText";
	content = "";
	oglTexture = 0;
	width = 300;
	height = 300;
	font = NULL;
}

Text::Text(SDL_Color aColor, TTF_Font * aFont)
{
	name = "txt";
	color = aColor;
	font = aFont;
	content = "";
	oglTexture = 0;
	width = 300;
	height = 300;
}

Text::Text(SDL_Color aColor, TTF_Font * aFont, int Xpos, int Ypos, int aWidth, int aHeight)
{
	posX = Xpos;
	name = "txt";
	posY = Ypos;
	width = aWidth;
	height = aHeight;
	color = aColor;
	font = aFont;
	oglTexture = 0;
	content = "";
}

//Specify the text to be display by the Text element
void Text::write(string aText)
{
	//Protection from crash when writing an empty string
	if(aText.empty())
	{
		cout<< "Warning: Attempting to write an empty string" << endl;
		aText = " ";
	}

	content = aText;
	texture = TTF_RenderUTF8_Blended(font, aText.c_str(), color);
	if (getTexture() == NULL)
	{
		cerr << "TTF_RenderUTF8_Blended() Failed: " << TTF_GetError() << endl;
	}

	width = getTexture()->w;
	height = getTexture()->h;

	if(getOpenGLTexture() != 0)
		glDeleteTextures(1, &oglTexture);

	graphicEngine->createOGLTexture(getTexture(), &oglTexture, false);

	setAnimX(0);
	setAnimY(0);
}

//Used when changes are made on the text elements
//(e.g. the color of the text)
void Text::update()
{
	SDL_FreeSurface(getTexture());
	texture = NULL;
	texture =  TTF_RenderUTF8_Blended(font, content.c_str(), color);
	if (getTexture() == NULL)
	{
		cerr << "TTF_RenderUTF8_Blended() Failed: " << TTF_GetError() << endl;
	}

	width = getTexture()->w;
	height = getTexture()->h;

	glDeleteTextures(1, &oglTexture);
	oglTexture = 0;
	graphicEngine->createOGLTexture(getTexture(), &oglTexture, false);

	setAnimX(0);
	setAnimY(0);
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

AnimatedText::AnimatedText()
{

}
/*
AnimatedText::AnimatedText(int x, int y, string aName, string aText)
{
	color = ge->availableColors.at("WHITE");
	font = ge->availableFonts.at("lCrystal");

	name = aName;
	width = 300;
	height = 300;

	posX = x;
	posY = y;
	display = TRUE;

	//anAnimatedDrawable->copyFrom((*aLayer).second);

	write(aText);
}*/
