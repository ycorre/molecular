/*
 * Contains herodescription
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
/*
class Photon: public Shoot
{
   public:
	  AnimatedDrawable trail;

	  Photon();
	  Photon(int x, int y, Weapon * aWeapon);
	  void animate();
	  virtual bool isPhoton() {return true;}
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void setParam(int x, int y, Weapon * aWeapon);
	  void removeEnergy(int anEnergyValue);
};*/

class HadronAmmo: public Shoot
{
   public:
	  float speed;
	  AnimatedDrawable extraPhoton;

	  HadronAmmo(int x, int y, Weapon * aWeapon);
	  void animate();
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void removeEnergy(int anEnergyValue);
	  virtual bool isPhoton() {return true;}
};

class Lazer: public Shoot
{
	public:
		AnimatedDrawable lightning, burningFlames;
		AnimatedDrawable attack, release;

		Lazer();
		Lazer(int x, int y, Weapon * aWeapon);
		virtual bool isLazer() {return true;}
		void animate(float x, float y, Enemy * anHitEnemy, float xImpactPos, float yImpactPos);
		virtual void processCollisionWith(Drawable * aDrawable);
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
	  virtual void animate();
	  virtual void processCollisionWith(Drawable * aDrawable);
};

class CopperAmmo: public Shoot
{
   public:
	  int power;
	  int speed;
	  Drawable halo;

	  CopperAmmo(int x, int y, float anAngle, float aSpeed);
	  virtual void animate();
	  virtual void processCollisionWith(Drawable * aDrawable);
};

#endif

