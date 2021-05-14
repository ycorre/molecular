"
I am a sprite with animations.
Instead of holding a form with a sprite frame, I hold named animations, each with its own form and a list of sprite frames to animate that form.
"
Class {
	#name : #MolAnimatedSprite,
	#superclass : #MolSprite,
	#instVars : [
		'animations',
		'collision',
		'currentAnimation'
	],
	#category : #'Molecular-model'
}

{ #category : #accessing }
MolAnimatedSprite >> animations [

	^ animations
]

{ #category : #accessing }
MolAnimatedSprite >> animations: anObject [

	animations := anObject
]

{ #category : #building }
MolAnimatedSprite >> build [

	| animationsDictionaries |
	animationsDictionaries := animations.
	animationsDictionaries do: [ :d | 
		d at: 'width' ifAbsentPut: [ self width ].
		d at: 'height' ifAbsentPut: [ self height ] ].
	animations := Dictionary new.
	animationsDictionaries do: [ :d | 
		| animation |
		animation := MolAnimation new readFromDictionnary: d.
		animations at: animation name put: animation ].
	currentAnimation := self defaultAnimation.
	^ self
]

{ #category : #accessing }
MolAnimatedSprite >> collision [

	^ collision
]

{ #category : #accessing }
MolAnimatedSprite >> collision: anObject [

	collision := anObject
]

{ #category : #building }
MolAnimatedSprite >> defaultAnimation [

	^ animations
		  at: 'default'
		  ifAbsent: [ 
		  animations at: animations keys asSortedCollection first ]
]

{ #category : #frames }
MolAnimatedSprite >> nextSpriteFrame [
	^currentAnimation nextFrame
]
