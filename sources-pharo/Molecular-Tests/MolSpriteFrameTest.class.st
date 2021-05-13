Class {
	#name : #MolSpriteFrameTest,
	#superclass : #TestCase,
	#instVars : [
		'spriteFrame'
	],
	#category : #'Molecular-Tests'
}

{ #category : #running }
MolSpriteFrameTest >> tearDown [
	spriteFrame ifNotNil:[spriteFrame sdlBounds free].
	super tearDown
	
]

{ #category : #tests }
MolSpriteFrameTest >> testBounds [
	|sdlRect|
	spriteFrame := (0@0 extent: 10@10) asMolSpriteFrame.
	self assert: spriteFrame bounds equals: (0@0 extent: 10@10).
	sdlRect := spriteFrame sdlBounds.
	self assert: sdlRect class identicalTo: SDL_Rect.
	self assert: sdlRect x equals: 0.
	self assert: sdlRect y equals: 0.
	self assert: sdlRect w equals: 10.
	self assert: sdlRect h equals: 10
]
