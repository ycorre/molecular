Class {
	#name : #MolGraphicsEntityTest,
	#superclass : #TestCase,
	#instVars : [
		'entity'
	],
	#category : #'Molecular-Tests-tests'
}

{ #category : #running }
MolGraphicsEntityTest >> setUp [
	super setUp.
	
	entity := MolGraphicEntity new
]

{ #category : #tests }
MolGraphicsEntityTest >> testAddSpriteConfiguration [

	| conf |
	conf := MolSpriteConfiguration new.
	conf name: 'test'.
	entity addSpriteConfiguration: conf.
	self
		assert: (entity spritesConfigurations at: conf name)
		identicalTo: conf
]

{ #category : #tests }
MolGraphicsEntityTest >> testDefaultSpriteConfiguration [
	self assert: entity spritesConfigurations isEmpty
]

{ #category : #tests }
MolGraphicsEntityTest >> testSetSpriteConfigurationNamed [
	|conf|
	conf := MolSpriteConfiguration new.
	conf sprite: MolSprite new.
	conf glowSprite: MolSprite new.
	conf animation: MolAnimation new.
	conf name: 'test'.
	
	entity addSpriteConfiguration: conf.
	entity setSpriteConfigurationNamed: 'test'.
	
	self assert: entity displaySprite identicalTo: conf sprite.
	self assert: entity glowSprite identicalTo: conf glowSprite.
	self assert: entity displaySprite animation identicalTo: conf animation.
	self assert: entity displaySprite animation identicalTo: conf animation
	
]
