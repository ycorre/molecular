"
I represent an animation frame from a spritesheet.
I hold the bounds of the sprite, and an SDL rectangle with the same bounds to be passed to an SDL function.

I am a link to be used in a linked list. I therefore know the next frame of the spritesheet.
"
Class {
	#name : 'MolSpriteFrame',
	#superclass : 'ValueLink',
	#instVars : [
		'sdlBounds'
	],
	#category : 'Molecular-model - 2D graphics',
	#package : 'Molecular',
	#tag : 'model - 2D graphics'
}

{ #category : 'accessing' }
MolSpriteFrame >> bounds [

	^ self value
]

{ #category : 'accessing' }
MolSpriteFrame >> sdlBounds [

	^ sdlBounds
]

{ #category : 'accessing' }
MolSpriteFrame >> value: anObject [
	super value: anObject.
	sdlBounds := self bounds asSDLRect 
]
