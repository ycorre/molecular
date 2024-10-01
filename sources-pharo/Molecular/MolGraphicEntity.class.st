"
I represent a graphic entity displayable on a graphics medium.
"
Class {
	#name : 'MolGraphicEntity',
	#superclass : 'Object',
	#traits : 'TMolMaterializer + TMolNamedEntity',
	#classTraits : 'TMolMaterializer classTrait + TMolNamedEntity classTrait',
	#instVars : [
		'position',
		'spritesConfigurations',
		'displaySprite',
		'glowSprite'
	],
	#category : 'Molecular-model - 2D graphics',
	#package : 'Molecular',
	#tag : 'model - 2D graphics'
}

{ #category : 'sprites - configuration' }
MolGraphicEntity >> addSpriteConfiguration: aSpriteConfiguration [
	self spritesConfigurations at: aSpriteConfiguration name put: aSpriteConfiguration 
]

{ #category : 'accessing' }
MolGraphicEntity >> displaySprite [

	^ displaySprite
]

{ #category : 'accessing' }
MolGraphicEntity >> displaySprite: anObject [

	displaySprite := anObject
]

{ #category : 'accessing' }
MolGraphicEntity >> glowSprite [

	^ glowSprite
]

{ #category : 'accessing' }
MolGraphicEntity >> glowSprite: anObject [

	glowSprite := anObject
]

{ #category : 'accessing' }
MolGraphicEntity >> position [

	^ position
]

{ #category : 'accessing' }
MolGraphicEntity >> position: anObject [

	position := anObject
]

{ #category : 'sprites - configuration' }
MolGraphicEntity >> setSpriteConfigurationNamed: aString [

	| configuration |
	configuration := self spritesConfigurations at: aString.
	displaySprite := configuration sprite.
	displaySprite animation: configuration animation.
	glowSprite := configuration glowSprite.
	glowSprite animation: configuration animation
]

{ #category : 'accessing' }
MolGraphicEntity >> spritesConfigurations [

	^ spritesConfigurations ifNil:[spritesConfigurations := Dictionary new]
]

{ #category : 'accessing' }
MolGraphicEntity >> spritesConfigurations: anObject [

	spritesConfigurations := anObject
]
