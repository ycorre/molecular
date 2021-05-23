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
MolAnimationTest >> testDefaultName [
	self assert: MolAnimation new name equals: MolAnimation defaultName.
	self assert: MolStaticAnimation new name equals: MolStaticAnimation defaultName
]

{ #category : #tests }
MolAnimationTest >> testInstantiationFromJSON [
	animation := MolAnimation exampleAnimation.
	self assert: animation name equals: 'example'.
	self assert: animation width equals: 64.
	self assert: animation height equals: 64.
	self assert: animation numberOfFrames equals: 10
	
]

{ #category : #tests }
MolAnimationTest >> testName [

	self should: [ MolStaticAnimation new name: 'test' ] raise: MessageNotUnderstood.
	self assert: (MolStaticAnimation new name) equals: MolStaticAnimation defaultName.
	self assert: (MolAnimation new name: 'test'; name) equals: 'test'
]

{ #category : #tests }
MolAnimationTest >> testPrintOn [

	| staticAnimation str |
	staticAnimation := MolStaticAnimation new.
	str := WriteStream on: String new.
	staticAnimation printOn: str.
	self assert: str contents equals: 'Molecular animation: static'.
	
	animation := MolAnimation new.
	str := WriteStream on: String new.
	animation name: 'test'.
	animation printOn: str.
	self assert: str contents equals: 'Molecular animation: test'
	
	
]

{ #category : #tests }
MolAnimationTest >> testStaticAnimationNextFrame [
	|staticAnimation|	
	staticAnimation := MolStaticAnimation new.
	staticAnimation currentFrame: Object new.
	self assert: staticAnimation nextFrame identicalTo: staticAnimation currentFrame.
	self assert: staticAnimation nextFrame identicalTo: staticAnimation currentFrame
]

{ #category : #tests }
MolAnimationTest >> testbuildFramesInBounds [
	|frame|
	animation := MolAnimation basicNew.
	animation width: 64.
	animation height: 64.
	animation numberOfFrames: 4.
	animation buildFramesInBounds: (0@0 extent: 128@128).	
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
	self assert: animation nextFrame identicalTo: animation frames second.
	
	frame := 	animation frames second.
	self assert:	frame bounds origin equals: 64@0.
	self assert:	frame bounds corner equals: 128@64.
	self assert: frame sdlBounds x equals: 64.
	self assert: frame sdlBounds y equals: 0.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64.
	self assert: animation nextFrame identicalTo: animation frames third.
	
	frame := 	animation frames third.
	self assert:	frame bounds origin equals: 0@64.
	self assert:	frame bounds corner equals: 64@128.
	self assert: frame sdlBounds x equals: 0.
	self assert: frame sdlBounds y equals: 64.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64.
	self assert: animation nextFrame identicalTo: animation frames fourth.
	
	frame := 	animation frames fourth.
	self assert:	frame bounds origin equals: 64@64.
	self assert:	frame bounds corner equals: 128@128.
	self assert: frame sdlBounds x equals: 64.
	self assert: frame sdlBounds y equals: 64.
	self assert: frame sdlBounds h equals: 64.
	self assert: frame sdlBounds w equals: 64.
	self assert: animation nextFrame identicalTo: animation frames first.
			
		
	
]
