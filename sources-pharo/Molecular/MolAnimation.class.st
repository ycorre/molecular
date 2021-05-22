"
I model a spritesheet animation.


	currentFrame:		<MolSpriteFrame> the frame to be display from the spritesheet.
	frames:				<OrderedCollection>	
	height:				<SmallInteger>			The individual height of each frame
	numberOfFrames:		<SmallInteger>			The total number of frames
	width:					<SmallInteger>			The individual width of each frame



"
Class {
	#name : #MolAnimation,
	#superclass : #MolStaticAnimation,
	#instVars : [
		'name',
		'width',
		'height',
		'numberOfFrames',
		'frames'
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
MolAnimation >> buildFramesInBounds: aRectangle [ 

	| i j frame |
	frames := OrderedCollection new.
	i := 0.
	j := 0.
	(width = aRectangle width and: [ height = aRectangle height ]) ifTrue: [ 
		numberOfFrames := 1 ].
	numberOfFrames timesRepeat: [ 
		| newFrame |
		newFrame := (i @ j extent: width @ height) asMolSpriteFrame.
		frame ifNotNil: [ frame nextLink: newFrame ].
		frame := newFrame.
		frames add: frame.
		i := i + width.
		i = aRectangle width ifTrue: [ 
			i := 0.
			j := j + height ].
		j = aRectangle height ifTrue: [ 
			i := 0.
			j := 0 ] ].
	currentFrame := frames last.
	currentFrame nextLink: frames first
]

{ #category : #animation }
MolAnimation >> currentFrame [

	^ currentFrame ifNil: [ currentFrame := frames first ]
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
	name := self class defaultName.
	frames := OrderedCollection new
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

	^ currentFrame := self currentFrame nextLink
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
