/*
 * utility.cpp
 *
 *  Created on: 24 janv. 2014
 *      Author: youenn
 */
#include "utility.h"

/*
 * Movement computation functions
 */
vector<pair<float, float> > computeInertialLinearMove(float xOrigin, float yOrigin, float xDestination, float yDestination, float speed)
{
	vector<pair<float, float> > movements;
	vector<pair<float, float> > tmpMovements;

	float xTmp, yTmp;
	float xDiff = xDestination - xOrigin;
	float yDiff = yDestination - yOrigin;
	float currentSpeed = 0;
	speed = fabs(speed);
	float speedFactor = speed / 50;
	float angle = atan2(yDiff, xDiff);

	//Formula to compute acceleration a, vF = final speed, vI = initial speed, d = distance
	//a = (Vf*Vf - Vi*Vi)/(2 * d)
	float distanceToStartBraking = (speed * speed) / (speedFactor * 2);

	xTmp = xOrigin;
	yTmp = yOrigin;

	do{
		xDiff = currentSpeed * cos(angle);
		yDiff = currentSpeed * sin(angle);

		xTmp = xTmp + xDiff;
		yTmp = yTmp + yDiff;

		movements.push_back(make_pair(xTmp, yTmp));

		//time to brake
		if(fabs(yTmp - yDestination) < distanceToStartBraking)
		{
			currentSpeed = currentSpeed - speedFactor;
		}
		else
		{
			currentSpeed = min(speed, currentSpeed + speedFactor);
		}
	}while(currentSpeed > 0);

	std::reverse(movements.begin(), movements.end());

	return movements;
}

vector<pair<float, float> > computeCircularMove(float xOrigin, float yOrigin, float radius, float startingAngle, float endingAngle, float speed)
{
	vector<pair<float, float> > movements;
	float angleStep = speed / radius;

	float angle;

	for(angle = startingAngle; angle <= endingAngle; angle = angle + angleStep)
	{
		float xTmp = xOrigin + radius * cos(angle);
		float yTmp = yOrigin + radius * sin(angle);
		movements.push_back(make_pair(xTmp, yTmp));
	}

	std::reverse(movements.begin(),movements.end());

	return movements;
}

/*
 * Random generation functions
 */
vector<float> getNormalDistributionNumbers(int numberOfNumbers, int mean, int deviation, int * minValue)
{
	int i;
	vector<float> results;
	float tmpRes;
	if(minValue)
		*minValue = 0;

	default_random_engine generator;
	generator.seed(time(NULL));
	//distribution(mean, deviationFromMean)
	normal_distribution<float> distribution(mean, deviation);

	for(i = 0; i <= numberOfNumbers; i++)
	{
		tmpRes = distribution(generator);
		results.push_back(tmpRes);

		if (minValue && *minValue > tmpRes)
			*minValue = tmpRes;
	}

	return results;
}

/*
 * Computation function
 */
//Compute values such as opacity, scale, etc.
//The config values for an animation are given as a series of pairs of numbers (x, y)
//*The x is the number of a key frame, where a specific value should be reached
//*The y is the actual value to be taken at frame x
//This is given in the configuration file as an array of numbers
//Given the key frames, the values and the number of frame in the animation
//we compute for each frame of the animation the value, by computing the value for each frame between two key frames
//assuming that the value is increasing (or decreasing) in a linear way
vector<float> computeLinearValue(vector<float> opacityConfigValue, int animationLength)
{
	unsigned int i, j;
	vector<float> results;
	map<int, float> tempValues;
	vector<int> frameIndices;
	float startingValue;
	float currentValue;
	float finishingValue;
	float modifyingFactor;

	//Get pair of values from the input vector
	for(i = 0; i < opacityConfigValue.size(); i = i + 2)
	{
		tempValues.insert(make_pair(opacityConfigValue.at(i), opacityConfigValue.at(i+1)));
		frameIndices.push_back(opacityConfigValue.at(i));
	}

	j = 0;
	startingValue = tempValues.at(frameIndices[j]);
	finishingValue = tempValues.at(frameIndices[j+1]);
	modifyingFactor = (finishingValue - startingValue) / (frameIndices[j+1] - frameIndices[j]);
	currentValue = startingValue;
	j++;

	//Compute the opacity values for each frame
	for (i = 0; i < animationLength; i++)
	{
		results.push_back(currentValue);
		if(j < frameIndices.size() - 1)
		{
			if(i == frameIndices[j])
			{
				startingValue = tempValues.at(frameIndices[j]);
				finishingValue = tempValues.at(frameIndices[j+1]);
				modifyingFactor = (finishingValue - startingValue) / (frameIndices[j+1] - frameIndices[j]);
				currentValue = startingValue;
				j++;
			}
			currentValue = currentValue + modifyingFactor;
		}
		else
		{
			if(i < frameIndices[j])
				currentValue = currentValue + modifyingFactor;
		}
	}
	return results;
}

