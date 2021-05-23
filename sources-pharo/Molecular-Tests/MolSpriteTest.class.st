Class {
	#name : #MolSpriteTest,
	#superclass : #TestCase,
	#instVars : [
		'sprite'
	],
	#category : #'Molecular-Tests-tests'
}

{ #category : #tests }
MolSpriteTest >> testInstantiationFromJSON [
	sprite := MolSprite exampleSprite.
	self assert: sprite name equals: 'example'.
	self assert: sprite width equals: 128.
	self assert: sprite height equals: 128.
	self assert: sprite animation equals: 'example'.
	self assert: sprite dataPath equals: 'path'
]

{ #category : #tests }
MolSpriteTest >> testNextSpriteFrame [
	|animation frame|
	sprite := MolSprite exampleSprite.
	animation := MolStaticAnimation new.
	frame := ((0@0) extent: (sprite width@sprite height)) asMolSpriteFrame.
	animation currentFrame:frame.
	sprite animation: animation.
	self assert: sprite nextSpriteFrame identicalTo: frame
]

{ #category : #tests }
MolSpriteTest >> testUpdateAnimation [
	|animation|
	sprite := MolSprite exampleSprite.
	animation := MolStaticAnimation new.
	sprite updateAnimation: animation.
	self assert: sprite animation identicalTo: animation.
	self assert: animation currentFrame class identicalTo: MolSpriteFrame.
	self assert: animation currentFrame sdlBounds w equals: sprite width.
	self assert: animation currentFrame sdlBounds h equals: sprite height
]
