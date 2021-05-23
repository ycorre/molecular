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
