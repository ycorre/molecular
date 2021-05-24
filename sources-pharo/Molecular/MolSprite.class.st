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
		'animation',
		'sldTexture'
	],
	#category : #'Molecular-model - 2D graphics'
}

{ #category : #examples }
MolSprite class >> exampleSprite [
<script>
	^ self readFromString:
		  '{"name" : "example", "width" : 128, "height" : 128, "animation" : "example", "dataPath" : "path"}'
]

{ #category : #accessing }
MolSprite >> animation [

	^ animation
]

{ #category : #accessing }
MolSprite >> animation: aMolAnimationOrString [
	self flag: 'When the sprite is built, the string should be replaced by the correct animation'.
	animation := aMolAnimationOrString
]

{ #category : #building }
MolSprite >> build [

	| animations animationsDictionaries currentAnimation|
	self deprecated: 'Sprites do not care anymore about their animations. They have one, that decides what to do.'.
	animationsDictionaries := animations.
	animationsDictionaries do: [ :d | 
		d at: 'width' ifAbsentPut: [ self width ].
		d at: 'height' ifAbsentPut: [ self height ] ].
	animations := Dictionary new.
	animationsDictionaries do: [ :d | 
	
		animation := MolAnimation new readFromDictionnary: d.
		animations at: animation name put: animation ].
	currentAnimation := self defaultAnimation.
	^ self
]

{ #category : #textures }
MolSprite >> buildTextureWith: aRenderer [

	animations do: [ :a | aRenderer initializeTextureFor: a blend: self blend ]
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

	^ animation nextFrame
]

{ #category : #rendering }
MolSprite >> renderOn: aRenderer atPosition: aSDLRectangle [
	self flag: 'wont work anymore'.
	aRenderer
		renderAnimationFrame: animation
		atPosition: aSDLRectangle
]

{ #category : #accessing }
MolSprite >> sldTexture [

	^ sldTexture
]

{ #category : #accessing }
MolSprite >> sldTexture: anObject [

	sldTexture := anObject
]

{ #category : #updating }
MolSprite >> updateAnimation: anAnimation [

	anAnimation buildFramesInBounds: (0 @ 0 extent: width @ height).
	self animation: anAnimation
]

{ #category : #accessing }
MolSprite >> width [

	^ width
]

{ #category : #accessing }
MolSprite >> width: anObject [

	width := anObject
]
