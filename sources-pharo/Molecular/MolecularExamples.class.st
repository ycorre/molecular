Class {
	#name : 'MolecularExamples',
	#superclass : 'Object',
	#instVars : [
		'aHost',
		'aSpace',
		'rock',
		'explosion'
	],
	#category : 'Molecular-examples',
	#package : 'Molecular',
	#tag : 'examples'
}

{ #category : 'examples' }
MolecularExamples class >> exampleRock [
	<example>
	self new exampleRock 
]

{ #category : 'sprites' }
MolecularExamples >> animationFrom: image size: size width: hw [

	| all i j |
	all := OrderedCollection new.
	i := 0.
	j := 0.
	size timesRepeat: [
		size timesRepeat: [
			all add: (image copy: (i @ j extent: hw @ hw)).
			i := i + hw ].
		i := 0.
		j := j + hw ].
	^ all
]

{ #category : 'sprites' }
MolecularExamples >> animationsFor: aPathString size: size hw: hw [

	| img |
	img := Form fromFileNamed:
		       (MolResourceLocator new molecularResourcePathFor: aPathString).
	^ self animationFrom: img size: size width: hw
]

{ #category : 'examples' }
MolecularExamples >> exampleRock [
	|rockElement explosionElement removed|

	rockElement := (BlElement new width: 102; height: 102; yourself).
	explosionElement := (BlElement new width: 204; height: 204; yourself).
	aSpace show.
	
	[aSpace root addChild: rockElement.
	rock do:[:f| 
		rockElement background: f. 
		rockElement position: rockElement position + (3@3). 
		30 milliSecond wait]. 
	
	explosionElement position: rockElement position - (36@36).
	
	aSpace root addChild: explosionElement.
	removed := false.
	explosion do:[:f| 
		explosionElement background: f.  
		removed ifFalse:[
		aSpace root removeChild: rockElement.
		removed := true].
		50 milliSecond wait].
	] fork
]

{ #category : 'initialization' }
MolecularExamples >> initialize [
	aHost := BlOSWindowSDL2Host new.
	aSpace := BlSpace new.
	aSpace host: aHost.
	aSpace root background: Color black.

	rock := self animationsFor: 'res/action/characters/o001Sp.png' size: 10 hw: 102.
	explosion := self animationsFor: 'res/action/characters/expl001Sp_Fire_fixed.png' size: 6 hw: 204.
]
