"
I model a 2D sprite.
"
Class {
	#name : #MolSprite,
	#superclass : #Object,
	#traits : 'TMolMaterializer',
	#classTraits : 'TMolMaterializer classTrait',
	#instVars : [
		'width',
		'height',
		'name',
		'dataPath',
		'form',
		'spriteFrame',
		'clampTexture',
		'virtualDepth',
		'blend',
		'opacity',
		'sldTexture'
	],
	#category : #'Molecular-model'
}

{ #category : #accessing }
MolSprite >> blend [

	^ blend
]

{ #category : #accessing }
MolSprite >> blend: anObject [

	blend := anObject
]

{ #category : #building }
MolSprite >> build [

	form := self buildFormFromDataPath: dataPath.
	spriteFrame := ((0 @ 0) extent: (form width) @ (form height))
		               asMolSpriteFrame.
	^ self
]

{ #category : #accessing }
MolSprite >> clampTexture [

	^ clampTexture
]

{ #category : #accessing }
MolSprite >> clampTexture: anObject [

	clampTexture := anObject
]

{ #category : #accessing }
MolSprite >> dataPath [

	^ dataPath
]

{ #category : #accessing }
MolSprite >> dataPath: anObject [

	dataPath := anObject
]

{ #category : #accessing }
MolSprite >> form [

	^ form
]

{ #category : #accessing }
MolSprite >> form: anObject [

	form := anObject
]

{ #category : #accessing }
MolSprite >> height [

	^ height
]

{ #category : #accessing }
MolSprite >> height: anObject [

	height := anObject
]

{ #category : #accessing }
MolSprite >> name [

	^ name
]

{ #category : #accessing }
MolSprite >> name: anObject [

	name := anObject
]

{ #category : #frames }
MolSprite >> nextSpriteFrame [
	^spriteFrame 
]

{ #category : #accessing }
MolSprite >> opacity [

	^ opacity
]

{ #category : #accessing }
MolSprite >> opacity: anObject [

	opacity := anObject
]

{ #category : #rendering }
MolSprite >> renderOn: aRenderer atPosition: aSDLRectangle [
]

{ #category : #accessing }
MolSprite >> sldTexture [

	^ sldTexture
]

{ #category : #accessing }
MolSprite >> sldTexture: anObject [

	sldTexture := anObject
]

{ #category : #accessing }
MolSprite >> spriteFrame [

	^ spriteFrame
]

{ #category : #accessing }
MolSprite >> virtualDepth [

	^ virtualDepth
]

{ #category : #accessing }
MolSprite >> virtualDepth: anObject [

	virtualDepth := anObject
]

{ #category : #accessing }
MolSprite >> width [

	^ width
]

{ #category : #accessing }
MolSprite >> width: anObject [

	width := anObject
]
