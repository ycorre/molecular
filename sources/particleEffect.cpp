#include "include/particleEffect.h"

ParticleEffect::ParticleEffect()
{
	type = "";
	opacity = 1;
	scaleX = scaleY = 1;
	rotX = rotY = rotZ = 0;
	posX = posY = 0;
	angle = 0;
	animationLength = 0;
	currentFrame = 0;
	colorR = colorG = colorB = 0;
	animationUpdateFrequency = 40;
	lastTimeUpdated = 0;
	particleEmitter = NULL;
	lifeTime = 1;
	speed = 0;
	particleTypes["point"] = PARTICLE_POINT;
	particleTypes["line"] = PARTICLE_LINE;
	particleTypes["disc"] = PARTICLE_DISC;
	particleTypes["textured"] = PARTICLE_TEXTURED;
}

ParticleEffect::~ParticleEffect()
{
	for (list<PointEffect *>::iterator aPoint = pointEffects.begin(); aPoint != pointEffects.end(); ++aPoint)
	{
		delete (*aPoint);
	}
	pointEffects.clear();

	for (list<LineEffect *>::iterator aLine = lineEffects.begin(); aLine != lineEffects.end(); ++aLine)
	{
		delete (*aLine);
	}
	lineEffects.clear();

	for (list<CircleEffect *>::iterator aCircle = circleEffects.begin(); aCircle != circleEffects.end(); ++aCircle)
	{
		delete (*aCircle);
	}
	circleEffects.clear();
}

//Set color values in linear decreasing from the starting value to the finishing one
void ParticleEffect::setColorValues(vector<float> startingColorValue, vector<float> endingColorValue)
{
	int i, j;
	vector<float> currentColor(3);
	vector<float> colorModifyingFactor(3);

	for (i = 0; i < 3; i++)
	{
		colorModifyingFactor.at(i) = (endingColorValue.at(i) - startingColorValue.at(i)) / animationLength;
		currentColor.at(i) = startingColorValue.at(i);
	}

	for (i = 0; i < animationLength; i++)
	{
		colorValues.push_back(currentColor);
		for (j = 0; j < 3; j++)
		{
			currentColor.at(j) = currentColor.at(j) + colorModifyingFactor.at(j);
		}
	}
}

void ParticleEffect::animate()
{
	unsigned int updateTime = lastTimeUpdated + animationUpdateFrequency;

	if (updateTime < ProgramTimer)
	{
		unsigned int i = 0, j = 0;

		if(!opacityValues.empty())
		{
			opacity = opacityValues.at(currentFrame);
		}

		if(!colorValues.empty())
		{
			colorR = colorValues.at(currentFrame).at(0);
			colorG = colorValues.at(currentFrame).at(1);
			colorB = colorValues.at(currentFrame).at(2);
		}

		for (list<PointEffect *>::iterator aPoint = pointEffects.begin(); aPoint != pointEffects.end(); ++aPoint)
		{
			(*aPoint)->animate();
			pointCoordinateValues.at(i) = ((*aPoint)->posX);
			pointCoordinateValues.at(i + 1) = (*aPoint)->posY;
			pointColorValues.at(j) = colorR;
			pointColorValues.at(j + 1) = colorG;
			pointColorValues.at(j + 2) = colorB;
			pointColorValues.at(j + 3) = opacity;
			i = i + 2;
			j = j + 4;
		}

		currentFrame++;
		lastTimeUpdated = ProgramTimer;
	}
}

void ParticleEffect::instantiateEffects(Json::Value aConf, float x, float y)
{
	Json::Value effectConfig = aConf["effectType"];
	unsigned int i;

	for(i = 0; i < effectConfig.size(); i++)
	{
		string anEffectType = effectConfig[i].get("type", "").asString();

		switch(particleTypes.at(anEffectType))
		{
			case PARTICLE_POINT:
				instantiatePointEffects(effectConfig[i] , x, y);
				break;

			case PARTICLE_LINE:
				instantiateLineEffects(effectConfig[i] , x, y);
				break;

			case PARTICLE_DISC:
				instantiatecircleEffects(effectConfig[i] , x, y);
				break;

			default:
				cerr << "Warning: ParticleEffect::instantiateEffect unknown effect type: " << particleTypes.at(anEffectType) << endl;
				break;
		}
	}
}

void ParticleEffect::instantiatePointEffects(Json::Value aConfig, float x, float y)
{
	unsigned int i;
	int numberOfParticle = aConfig.get("number", 0).asInt();
	bool randomized = aConfig.get("randomNumber", false).asBool();
	animationLength = aConfig.get("lifetime", 0).asInt();
	float pointSize = aConfig.get("size", 2).asFloat();
	float pointSpeed = aConfig.get("speed", -1).asFloat();
	float maxSpeed = aConfig.get("maxSpeed", 20.0f).asFloat();
	float deceleration = aConfig.get("decelerationFactor", 0.85f).asFloat();
	float lowBoundAngle = aConfig.get("lowBoundAngle", 0.0).asFloat();
	float highBoundAngle = aConfig.get("highBoundAngle", 360.0).asFloat();

	Json::Value sColor = aConfig["startingColor"];
	vector<float> startingColor;
	for(i = 0; i < sColor.size(); i++)
		startingColor.push_back(sColor[i].asFloat());

	Json::Value eColor = aConfig["endingColor"];
	vector<float> endingColor;
	for(i = 0; i < eColor.size(); i++)
		endingColor.push_back(eColor[i].asFloat());

	Json::Value opacity = aConfig["opacity"];
	vector<float> opacityConfigValue;
	for(i = 0; i < opacity.size(); i++)
		opacityConfigValue.push_back(opacity[i].asFloat());

	if(randomized)
	{
		numberOfParticle = rand() % numberOfParticle;
		if(numberOfParticle == 0)
		{
			return;
		}
	}

	//if color is constant
	if(endingColor.empty())
	{
		colorR = startingColor.at(0)/255.0;
		colorG = startingColor.at(1)/255.0;
		colorB = startingColor.at(2)/255.0;
	}
	else
	{
		//Normalize value on [0.0, 1.0]
		for(i = 0; i < 3; i++)
		{
			startingColor.at(i) = startingColor.at(i)/255.0;
			endingColor.at(i) = endingColor.at(i)/255.0;
		}
		setColorValues(startingColor, endingColor);
	}

	if(!opacityConfigValue.empty())
	{
		opacityValues = computeLinearValue(opacityConfigValue, animationLength);
	}

	for(i = 0; i < numberOfParticle; i++)
	{
		PointEffect * aPoint = new PointEffect();
		aPoint->createRandomPointFrom(x, y, lowBoundAngle, highBoundAngle);
		aPoint->size = pointSize;
		if(pointSpeed == -1)
		{
			aPoint->speed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxSpeed));
		}
		else
		{
			aPoint->speed = pointSpeed;
		}
		aPoint->decelerationFactor = deceleration;

		pointCoordinateValues.push_back(aPoint->posX);
		pointCoordinateValues.push_back(aPoint->posY);
		pointColorValues.push_back(colorR);
		pointColorValues.push_back(colorG);
		pointColorValues.push_back(colorB);
		pointColorValues.push_back(1.0f);

		pointEffects.push_back(aPoint);
	}

	currentFrame = 0;
}

void ParticleEffect::instantiateLineEffects(Json::Value aConfig, float x, float y)
{
	unsigned int i;
	int numberOfParticle = aConfig.get("number", 0).asInt();
	bool randomized = aConfig.get("randomNumber", false).asBool();
	animationLength = aConfig.get("lifetime", 0).asInt();

	Json::Value sColor = aConfig["startingColor"];
	vector<float> startingColor;
	for(i = 0; i < sColor.size(); i++)
		startingColor.push_back(sColor[i].asFloat());

	Json::Value eColor = aConfig["endingColor"];
	vector<float> endingColor;
	for(i = 0; i < eColor.size(); i++)
		endingColor.push_back(eColor[i].asFloat());

	Json::Value opacity = aConfig["opacity"];
	vector<float> opacityConfigValue;
	for(i = 0; i < opacity.size(); i++)
		opacityConfigValue.push_back(opacity[i].asFloat());

	if (randomized)
	{
		numberOfParticle = rand() % numberOfParticle;
	}

	for(i = 0; i < numberOfParticle; i++)
	{
		LineEffect * aLine = new LineEffect();
		aLine->createRandomLineFrom(x, y);
		lineEffects.push_back(aLine);
	}

	currentFrame = 0;

	if(endingColor.empty())
	{
		colorR = startingColor.at(0)/255.0;
		colorG = startingColor.at(1)/255.0;
		colorB = startingColor.at(2)/255.0;
	}
	else
	{
		//Normalize value on [0.0, 1.0]
		for(i = 0; i < 3; i++)
		{
			startingColor.at(i) = startingColor.at(i)/255.0;
			endingColor.at(i) = endingColor.at(i)/255.0;
		}
		setColorValues(startingColor, endingColor);
	}

	if(!opacityConfigValue.empty())
	{
		opacityValues = computeLinearValue(opacityConfigValue, animationLength);
	}
}

void ParticleEffect::instantiatecircleEffects(Json::Value aConfig, float x, float y)
{
	unsigned int i;
	animationLength = aConfig.get("lifetime", 0).asInt();
	float radius = aConfig.get("radius", 0).asFloat();

	Json::Value sColor = aConfig["startingColor"];
	vector<float> startingColor;
	for(i = 0; i < sColor.size(); i++)
		startingColor.push_back(sColor[i].asFloat());

	Json::Value eColor = aConfig["endingColor"];
	vector<float> endingColor;
	for(i = 0; i < eColor.size(); i++)
		endingColor.push_back(eColor[i].asFloat());

	Json::Value opacity = aConfig["opacity"];
	vector<float> opacityConfigValue;
	for(i = 0; i < opacity.size(); i++)
		opacityConfigValue.push_back(opacity[i].asFloat());

	CircleEffect * aCircle = new CircleEffect(x, y, radius);
	circleEffects.push_back(aCircle);
	currentFrame = 0;

	if(endingColor.empty())
	{
		colorR = startingColor.at(0)/255.0;
		colorG = startingColor.at(1)/255.0;
		colorB = startingColor.at(2)/255.0;
	}
	else
	{
		//Normalize value on [0.0, 1.0]
		for(i = 0; i < 3; i++)
		{
			startingColor.at(i) = startingColor.at(i)/255.0;
			endingColor.at(i) = endingColor.at(i)/255.0;
		}
		setColorValues(startingColor, endingColor);
	}

	if(!opacityConfigValue.empty())
	{
		opacityValues = computeLinearValue(opacityConfigValue, animationLength);
	}
}

LineEffect::LineEffect()
{
	length = 1;
	destX = destY = 0;
	center = make_pair(0,0);
	lineWidth = 2.0f;
}

void LineEffect::createRandomLineFrom(float aPosX, float aPosY)
{
	center = make_pair(aPosX, aPosY);
	posX = aPosX;
	posY = aPosY;
	destX = aPosX - (rand() % 70);
	destY = aPosY + (-35 + rand() % 70);
	lineWidth = 2.0 + ((rand() % 40)/10.0);
}

CircleEffect::CircleEffect()
{
	radius = 0;
	slices = 30;
	center = make_pair(0,0);
}

CircleEffect::CircleEffect(float x, float y, float r)
{
	center = make_pair(x, y);
	slices = 30;
	radius = r;
}

PointEffect::PointEffect()
{
	decelerationFactor = 1.0f;
	size = 2.0f;
}

void PointEffect::createRandomPointFrom(float aPosX, float aPosY)
{
	posX = aPosX;
	posY = aPosY;
	angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360.0f)) * (PI/180);
	speed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/20.0f));
}

void PointEffect::createRandomPointFrom(float aPosX, float aPosY, float anAngleLowBound, float anAngleHighBound)
{
	float amplitude = anAngleHighBound - anAngleLowBound;
	posX = aPosX;
	posY = aPosY;
	angle = (anAngleLowBound + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/amplitude))) * (PI/180.0);
}

void PointEffect::animate()
{
	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	speed = speed * decelerationFactor;

	if(!opacityValues.empty())
	{
		opacity = opacityValues.at(currentFrame);
	}

	currentFrame++;
}

/*
 * Emitters functions
 */
Emitter::Emitter()
{
	posX = 0;
	posY = 0;
}

Emitter::Emitter(float aX, float aY)
{
	posX = aX;
	posY = aY;
}

pair<float, float> Emitter::getParticlePosition()
{
	return make_pair(posX, posY);
}

list<pair<float, float> > Emitter::getParticlePosition(int numberOfParticles)
{
	int i = 0;
	list<pair<float, float> > aList;

	for(i = 0; i < numberOfParticles; i++)
	{
		aList.push_back(this->getParticlePosition());
	}

	return aList;
}

SquareEmitter::SquareEmitter(float originX, float originY, float cornerX, float cornerY)
{
	posX = originX;
	posY = originY;
	destX = cornerX;
	destY = cornerY;
}

pair<float, float> SquareEmitter::getParticlePosition()
{
	float diffX = destX - posX;
	float diffY = destY - posY;

	float x = posX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/diffX));
	float y = posY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/diffY));

	return make_pair(x, y);
}

CircleEmitter::CircleEmitter(float centerX, float centerY, float aRadius)
{
	posX = centerX;
	posY = centerY;
	radius = aRadius;
}

//X coordinate given by radius*cos(theta)
pair<float, float> CircleEmitter::getParticlePosition()
{
	float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(2.0f*PI)));
	float aRadius = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/radius));

	float x = aRadius * cos(angle);
	float y = aRadius * sin(angle);

	return make_pair(x, y);
}

