"
I am a static animation always returning the same sprite frame.
"
Class {
	#name : #MolStaticAnimation,
	#superclass : #Object,
	#instVars : [
		'currentFrame'
	],
	#category : #'Molecular-model'
}

{ #category : #accessing }
MolStaticAnimation class >> defaultName [
	^'static'
]

{ #category : #accessing }
MolStaticAnimation >> currentFrame [

	^ currentFrame
]

{ #category : #accessing }
MolStaticAnimation >> currentFrame: anObject [

	^ currentFrame := anObject
]

{ #category : #accessing }
MolStaticAnimation >> name [

	^ self class defaultName
]

{ #category : #animation }
MolStaticAnimation >> nextFrame [

	^ currentFrame
]

{ #category : #printing }
MolStaticAnimation >> printOn: aStream [

	aStream << 'Molecular animation:'.
	aStream space.
	aStream << self name
]
