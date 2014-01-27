/*
 * utility.h
 *
 *  Created on: 24 janv. 2014
 *      Author: youenn
 */
#ifndef UTILITY_H_
#define UTILITY_H_
#include <random>
#include <map>
#include <algorithm>

using namespace std;

vector<pair<float, float> > computeInertialLinearMove(float xOrigin, float yOrigin, float xDestination, float yDestination, float speed);
vector<pair<float, float> > computeCircularMove(float xOrigin, float yOrigin, float radius, float startingAngle, float endingAngle, float direction);
vector<float> getNormalDistributionNumbers(int numberOfNumbers, int min, int max, int * minValue);

vector<float> computeLinearValue(vector<float> opacityConfigValue, int animationLength);

#endif /* UTILITY_H_ */
