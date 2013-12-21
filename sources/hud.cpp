/*
 * Contains the menu function
 */

#include "hud.h"

HUD::HUD(GraphicEngine * aGraphicEngine)
{
	int i;
	ge = aGraphicEngine;
	loadHUDElements("conf/hud.json");

	backGround = hudElements.at("hud_bkg");
	backGround.width = SCREEN_WIDTH;
	backGround.height = SCREEN_HEIGHT - GAMEZONE_HEIGHT;
	backGround.setAnimX(0);
	backGround.setAnimY(0);

	health = hudElements.at("hud_health");
	quarkB = hudElements.at("abeSpQB");
	quarkT = hudElements.at("abeSpQT");
	quarkU = hudElements.at("abeSpQU");
	quarkD = hudElements.at("abeSpQD");
	quarkC = hudElements.at("abeSpQC");
	quarkS = hudElements.at("abeSpQS");

	file = hudElements.at("abeDossiers");
	brightDot = hudElements.at("abeTypeLed");

	weaponLoad = hudElements.at("abeSpVumetre");

	for(i = 0; i < 6; i++)
	{
		Drawable anotherLife = hudElements.at("abeVie");
		anotherLife.posX = anotherLife.posX - (i * 30);
		lives.push_back(anotherLife);
	}

	for(i = 0; i < 16; i++)
	{
		Drawable aScoreNumber = hudElements.at("abeNumbers");
		aScoreNumber.posX = aScoreNumber.posX - (i * 15);
		score.push_back(aScoreNumber);
	}

	quarkIndex[QB] = 0;
	quarkIndex[QT] = 0;
	quarkIndex[QU] = 0;
	quarkIndex[QD] = 0;
	quarkIndex[QC] = 0;
	quarkIndex[QS] = 0;

	electron = hudAnimatedElements.at("abeElectr");
	hadron =  hudAnimatedElements.at("abeHadron");
	baryon =  hudAnimatedElements.at("abeBaryon");
	plasma =  hudAnimatedElements.at("abePlasma");

}

void HUD::loadHUDElements(string path)
{
	Json::Value root;
	Json::Reader reader;
	ifstream file;
	file.open(path.c_str());

	int parsingSuccessful = reader.parse(file, root);
	if (!parsingSuccessful)
	{
	    //report to the user the failure and their locations in the document.
	    cout << "Failed to parse configuration:" << endl << reader.getFormattedErrorMessages();
	}

	//instantiate the corresponding drawable elements
	const Json::Value drawable = root["drawable"];
	int index = 0;
	for (index = 0; index < drawable.size(); ++index)
	{
		Drawable tmp = Drawable(drawable[index]);
		tmp.posY = GAMEZONE_HEIGHT + tmp.posY;
		hudElements.insert(make_pair(tmp.name, tmp));
	}

	const Json::Value animDrawable = root["animatedDrawable"];
	for (index = 0; index < animDrawable.size(); ++index)
	{
		AnimatedDrawable tmp = AnimatedDrawable(animDrawable[index]);
		tmp.posY = GAMEZONE_HEIGHT + tmp.posY;
		hudAnimatedElements.insert(make_pair(tmp.name, tmp));
	}
}

void HUD::displayUI(Hero * anHero)
{
	backGround.processDisplay();
	file.processDisplay();
	brightDot.processDisplay();
	displayHealth(anHero->health);
	displayLife(anHero->nbLife);
	displayQuarkLevels(anHero->quarkLevels);
	displayWeapons(anHero);
	displayScore(Score);
}

//Display the current health of the player
void HUD::displayHealth(int healthValue)
{
	static float i = 0.0;

	if(i < healthValue * 3.6)
	{
		i = i + 0.3;
	}
	if (i > healthValue * 3.6)
	{
		i = i - 0.3;
	}
	health.setAnimX((int)(i) * health.width);

	health.processDisplay();
}

void HUD::displayScore(int aScore)
{
	int i = 0;

	while (aScore != 0)
	{
		int currentNumber = aScore % 10;
		aScore = aScore / 10;

		score.at(i).setAnimX(currentNumber * score.at(i).width);
		score.at(i).processDisplay();
		i++;
	}
}

/*
void HUD::displayMassPotential(float massPo)
{
	static float i = 0.0;

	if(i < massPo - 1)
	{
		i = i + 0.3;
	}
	else if (i > massPo)
	{
		i = massPo; //i - 0.3;
	}
	massLoad->setAnimX((int)(i) * massLoad->width);

	massLoad->processDisplay();
}

void HUD::displayRadioPotential(float radioPo)
{
	static float i = 0.0;

	if(i < radioPo)
	{
		i = i + 0.3;
	}
	if (i > radioPo)
	{
		i = i - 0.3;
	}
	radioLoad->setAnimX((int)(i) * radioLoad->width);

	radioLoad->processDisplay();
}
*/

void HUD::displayQuarkLevels(map<int, int> quarkLevels)
{
	for(map<int, int>::iterator aQuark = quarkLevels.begin(); aQuark != quarkLevels.end(); ++aQuark)
	{
		int aQuarkName = (*aQuark).first;
		switch(aQuarkName)
		{
			case QB:
				displayAQuarkLevel(&quarkB, quarkIndex.at(aQuarkName), quarkLevels.at(aQuarkName), aQuarkName);
				break;

			case QT:
				displayAQuarkLevel(&quarkT, quarkIndex.at(aQuarkName), quarkLevels.at(aQuarkName), aQuarkName);
				break;

			case QU:
				displayAQuarkLevel(&quarkU, quarkIndex.at(aQuarkName), quarkLevels.at(aQuarkName), aQuarkName);
				break;

			case QD:
				displayAQuarkLevel(&quarkD, quarkIndex.at(aQuarkName), quarkLevels.at(aQuarkName), aQuarkName);
				break;

			case QC:
				displayAQuarkLevel(&quarkC, quarkIndex.at(aQuarkName), quarkLevels.at(aQuarkName), aQuarkName);
				break;

			case QS:
				displayAQuarkLevel(&quarkS, quarkIndex.at(aQuarkName), quarkLevels.at(aQuarkName), aQuarkName);
				break;

			default:
				break;
		}
	}
}

void HUD::displayAQuarkLevel(Drawable * aQuark, float anIndex, int aLevel, int aQuarkName)
{
	if(anIndex < aLevel)
	{
		quarkIndex.at(aQuarkName) = anIndex + 0.3;
	}
	if (anIndex > aLevel)
	{
		quarkIndex.at(aQuarkName) = anIndex - 0.3;
	}
	aQuark->setAnimX((int)(quarkIndex.at(aQuarkName)) * aQuark->width);
	aQuark->processDisplay();
}


void HUD::displayWeapons(Hero * hero)
{
	//hero->ownedWeapons.at("electron");
	electron.setAnimation("l1");
	if(hero->currentWeapon->name.compare("electronGun") == 0)
		electron.setAnimation("l2");

	electron.processDisplay();
	hadron.setAnimation("l1");

	if(hero->currentWeapon->name.compare("hadronGun") == 0)
		hadron.setAnimation("l2");

	baryon.processDisplay();
	hadron.processDisplay();
	plasma.processDisplay();
	if (hero->currentWeapon->load == -1)
	{
		weaponLoad.setAnimX(0);
	}
	else
	{
		weaponLoad.setAnimX(weaponLoad.width);
	}
	weaponLoad.processDisplay();
}

void HUD::displayLife(int nbLife)
{
	int i;
	if (nbLife > 6)
	{
		nbLife = 6;
	}
	for(i = 0; i < nbLife; i++)
	{
		lives.at(i).processDisplay();
	}
}



