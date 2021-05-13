"
I model a spritesheet animation.


	currentFrame:		<MolSpriteFrame> the frame to be display from the spritesheet.
	frames:				<OrderedCollection>
	height:				<SmallInteger>			The individual height of each frame
	maxHeight:			<SmallInteger>			The individual height of each frame
	maxWidth:				<SmallInteger>			The individual height of each frame
	name:					<ByteString>				The individual height of each frame
	numberOfFrames:		<SmallInteger>			The individual height of each frame
	width:					<SmallInteger>			The individual width of each frame



"
Class {
	#name : #MolAnimation,
	#superclass : #Object,
	#traits : 'TMolMaterializer',
	#classTraits : 'TMolMaterializer classTrait',
	#instVars : [
		'name',
		'width',
		'height',
		'numberOfFrames',
		'frames',
		'currentFrame',
		'maxWidth',
		'maxHeight'
	],
	#category : #'Molecular-model'
}

{ #category : #default }
MolAnimation class >> defaultName [
	^'default'
]

{ #category : #examples }
MolAnimation class >> exampleAnimation [

	<script>
	^ self readFromString:
		  '{"name" : "example", "width" : 64, "height" : 64, "maxWidth" : 320, "maxHeight" : 320, "numberOfFrames" : 10}'
]

{ #category : #building }
MolAnimation >> build [
	self buildFrames.
	^ self
]

{ #category : #building }
MolAnimation >> buildFrames [

	| i j |
	frames := OrderedCollection new.
	i := 0.
	j := 0.
	numberOfFrames timesRepeat: [ 
		frames add: (i @ j extent: width @ height) asMolSpriteFrame.
		i := i + width.
		i = maxWidth ifTrue: [ 
			i := 0.
			j := j + height ].
		j = maxHeight ifTrue: [ 
			i := 0.
			j := 0 ] ].
	currentFrame := frames last.
	currentFrame nextLink: frames first
]

{ #category : #animation }
MolAnimation >> currentFrame [

	^ currentFrame
]

{ #category : #accessing }
MolAnimation >> frames [

	^ frames
]

{ #category : #accessing }
MolAnimation >> height [

	^ height
]

{ #category : #accessing }
MolAnimation >> height: anObject [

	height := anObject
]

{ #category : #initialization }
MolAnimation >> initialize [
	super initialize.
	name := self class defaultName
]

{ #category : #accessing }
MolAnimation >> maxHeight: anObject [

	maxHeight := anObject
]

{ #category : #accessing }
MolAnimation >> maxWidth: anObject [

	maxWidth := anObject
]

{ #category : #accessing }
MolAnimation >> name [

	^ name
]

{ #category : #accessing }
MolAnimation >> name: anObject [

	name := anObject
]

{ #category : #animation }
MolAnimation >> nextFrame [

	^ currentFrame nextLink
]

{ #category : #accessing }
MolAnimation >> numberOfFrames [

	^ numberOfFrames
]

{ #category : #accessing }
MolAnimation >> numberOfFrames: anObject [

	numberOfFrames := anObject
]

{ #category : #accessing }
MolAnimation >> width [

	^ width
]

{ #category : #accessing }
MolAnimation >> width: anObject [

	width := anObject
]
