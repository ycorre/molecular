/*
 * Contains stuff...
 */
 
#ifndef LASER_H
#define LASER_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"

class Weapon;
class Enemy;

class Shoot: public AnimatedDrawable
{
   public:
	  int power;
	  float angle;
	  float speed;
	  Weapon * firingWeapon;

	  Shoot();
	  Shoot(int x, int y, Weapon * aWeapon);
	  virtual void animate();
	  virtual bool isLaser() {return true;}
	  virtual bool isPhoton() {return false;}
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void createImpact(float x, float y);
};

class ElectronAmmo: public Shoot
{
   public:
	  ElectronAmmo(int x, int y, Weapon * aWeapon);
};

class HadronParticle: public Shoot
{
   public:
	  AnimatedDrawable star;

	  HadronParticle(float x, float y, float aScale, float anAngle, int aLoad, Weapon * aWeapon);
	  void processCollisionWith(Drawable * aDrawable);
	  void removeEnergy(int anEnergyValue);
	  void animate();
	  bool isPhoton() {return true;}
};

class Lazer: public Shoot
{
	public:
		AnimatedDrawable lightning, burningFlames;
		AnimatedDrawable attack, release;

		Lazer();
		Lazer(int x, int y, Weapon * aWeapon);
		bool isLazer() {return true;}
		void animate(float x, float y, Enemy * anHitEnemy, float xImpactPos, float yImpactPos);
		void processCollisionWith(Drawable * aDrawable);
		void upgrade(int aLevel);
};

class Bullet: public Shoot
{
   public:
	  int power;
	  int speed;

	  Bullet();
	  Bullet(int x, int y, float anAngle, int speed);
	  virtual void animate();
	  virtual int isBullet() {return true;}
	  virtual void processCollisionWith(Drawable * aDrawable);
};

class CadmiumAmmo: public Shoot
{
   public:
	  int power;
	  int speed;
	  Drawable halo;

	  CadmiumAmmo(int x, int y, float anAngle, float aSpeed);
	  void animate();
	  void processCollisionWith(Drawable * aDrawable);
};

class CopperAmmo: public Shoot
{
   public:
	  int power;
	  int speed;
	  Drawable halo;

	  CopperAmmo(int x, int y, float anAngle, float aSpeed);
	  void animate();
	  void processCollisionWith(Drawable * aDrawable);
};

#endif

