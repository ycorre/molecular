Class {
	#name : #MolAnimationTest,
	#superclass : #TestCase,
	#instVars : [
		'animation'
	],
	#category : #'Molecular-Tests'
}

{ #category : #running }
MolAnimationTest >> tearDown [
	animation ifNotNil: [  
		animation frames do:[:f| f sdlBounds free].
		].
	super tearDown.
	
	"Put here a common initialization logic for tests"
]

{ #category : #tests }
MolAnimationTest >> testBuild [
	
	animation := MolAnimation exampleAnimation.
	self assert: animation name equals: 'example'.
	self assert: animation width equals: 64.
	self assert: animation height equals: 64.
	self assert: animation numberOfFrames equals: 10.
	self assert: animation frames size equals: animation numberOfFrames
	
]

{ #category : #tests }
MolAnimationTest >> testBuildFrames [
	|frame|
	animation := MolAnimation basicNew.
	animation width: 64.
	animation height: 64.
	animation maxWidth: 64*2.
	animation maxHeight: 64*2.
	animation numberOfFrames: 4.
	animation buildFrames.	
	self assert: animation frames size equals: 4.	
	self assert: animation currentFrame identicalTo: animation frames last.
	self assert: animation nextFrame identicalTo: animation frames first.
	
	frame := 	animation frames first.
	self assert:	frame bounds origin equals: 0@0.
	self assert:	frame bounds corner equals: 64@64.
	self assert: frame sdlBounds x equals: 0.
	self assert: frame sdlBounds y equals: 0.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64.
	
	frame := 	animation frames second.
	self assert:	frame bounds origin equals: 64@0.
	self assert:	frame bounds corner equals: 128@64.
	self assert: frame sdlBounds x equals: 64.
	self assert: frame sdlBounds y equals: 0.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64.
	
	frame := 	animation frames third.
	self assert:	frame bounds origin equals: 0@64.
	self assert:	frame bounds corner equals: 64@128.
	self assert: frame sdlBounds x equals: 0.
	self assert: frame sdlBounds y equals: 64.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64.
	
	frame := 	animation frames fourth.
	self assert:	frame bounds origin equals: 64@64.
	self assert:	frame bounds corner equals: 128@128.
	self assert: frame sdlBounds x equals: 64.
	self assert: frame sdlBounds y equals: 64.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64
			
		
	
]

{ #category : #tests }
MolAnimationTest >> testDefaultName [
	self assert: MolAnimation new name equals: MolAnimation defaultName
]