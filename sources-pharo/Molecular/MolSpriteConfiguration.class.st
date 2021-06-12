"
I represent a configuration of sprites meant to be displayed together with a common animation:
- a display sprite
- a glow sprite
- an animation to be applied to both those sprites

The display sprites is meant to be displayed first, and the glow sprite on top.
"
Class {
	#name : #MolSpriteConfiguration,
	#superclass : #Object,
	#traits : 'TMolNamedEntity',
	#classTraits : 'TMolNamedEntity classTrait',
	#instVars : [
		'sprite',
		'glowSprite',
		'animation'
	],
	#category : #'Molecular-model - 2D graphics'
}

{ #category : #accessing }
MolSpriteConfiguration >> animation [

	^ animation
]

{ #category : #accessing }
MolSpriteConfiguration >> animation: anObject [

	animation := anObject
]

{ #category : #accessing }
MolSpriteConfiguration >> glowSprite [

	^ glowSprite
]

{ #category : #accessing }
MolSpriteConfiguration >> glowSprite: anObject [

	glowSprite := anObject
]

{ #category : #accessing }
MolSpriteConfiguration >> sprite [

	^ sprite
]

{ #category : #accessing }
MolSpriteConfiguration >> sprite: anObject [

	sprite := anObject
]
