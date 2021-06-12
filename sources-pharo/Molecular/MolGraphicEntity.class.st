"
I represent a graphic entity displayable on a graphics medium.
"
Class {
	#name : #MolGraphicEntity,
	#superclass : #Object,
	#traits : 'TMolMaterializer + TMolNamedEntity',
	#classTraits : 'TMolMaterializer classTrait + TMolNamedEntity classTrait',
	#instVars : [
		'position',
		'spritesConfigurations',
		'displaySprite',
		'glowSprite'
	],
	#category : #'Molecular-model - 2D graphics'
}

{ #category : #accessing }
MolGraphicEntity >> displaySprite [

	^ displaySprite
]

{ #category : #accessing }
MolGraphicEntity >> displaySprite: anObject [

	displaySprite := anObject
]

{ #category : #accessing }
MolGraphicEntity >> glowSprite [

	^ glowSprite
]

{ #category : #accessing }
MolGraphicEntity >> glowSprite: anObject [

	glowSprite := anObject
]

{ #category : #accessing }
MolGraphicEntity >> position [

	^ position
]

{ #category : #accessing }
MolGraphicEntity >> position: anObject [

	position := anObject
]

{ #category : #accessing }
MolGraphicEntity >> spritesConfigurations [

	^ spritesConfigurations
]

{ #category : #accessing }
MolGraphicEntity >> spritesConfigurations: anObject [

	spritesConfigurations := anObject
]
