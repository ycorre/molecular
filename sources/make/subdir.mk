################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../animation.cpp \
../asteroid.cpp \
../bomb.cpp \
../bonus.cpp \
../bullet.cpp \
../copper.cpp \
../drawable.cpp \
../effect.cpp \
../enemy.cpp \
../enemyWave.cpp \
../explosion.cpp \
../game.cpp \
../graphicEngine.cpp \
../hero.cpp \
../hud.cpp \
../keyb.cpp \
../laser.cpp \
../level.cpp \
../level1.cpp \
../level2.cpp \
../menu.cpp \
../particleEffect.cpp \
../physicEngine.cpp \
../sound.cpp \
../soundEngine.cpp \
../text.cpp \
../utility.cpp \
../weapon.cpp 

OBJS += \
./animation.o \
./asteroid.o \
./bomb.o \
./bonus.o \
./bullet.o \
./copper.o \
./drawable.o \
./effect.o \
./enemy.o \
./enemyWave.o \
./explosion.o \
./game.o \
./graphicEngine.o \
./hero.o \
./hud.o \
./keyb.o \
./laser.o \
./level.o \
./level1.o \
./level2.o \
./menu.o \
./particleEffect.o \
./physicEngine.o \
./sound.o \
./soundEngine.o \
./text.o \
./utility.o \
./weapon.o 

CPP_DEPS += \
./animation.d \
./asteroid.d \
./bomb.d \
./bonus.d \
./bullet.d \
./copper.d \
./drawable.d \
./effect.d \
./enemy.d \
./enemyWave.d \
./explosion.d \
./game.d \
./graphicEngine.d \
./hero.d \
./hud.d \
./keyb.d \
./laser.d \
./level.d \
./level1.d \
./level2.d \
./menu.d \
./particleEffect.d \
./physicEngine.d \
./sound.d \
./soundEngine.d \
./text.d \
./utility.d \
./weapon.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


