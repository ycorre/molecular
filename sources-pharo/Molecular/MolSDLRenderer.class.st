Class {
	#name : #MolSDLRenderer,
	#superclass : #Object,
	#instVars : [
		'registeredTextures',
		'sdlWindow',
		'sdlRenderer'
	],
	#pools : [
		'SDL2Constants',
		'SDL2Types'
	],
	#category : #'Molecular-rendering'
}

{ #category : #textures }
MolSDLRenderer >> buildTextureFor: aMolAnimatedSprite [

	aMolAnimatedSprite buildTextureWith: self
]

{ #category : #rendering }
MolSDLRenderer >> clear [ 
	sdlRenderer clear
]

{ #category : #initialization }
MolSDLRenderer >> drawBlackBackground [

	sdlRenderer drawColorR: 0 g: 0 b: 0 a: 0.

]

{ #category : #'freeing resources' }
MolSDLRenderer >> freeSDLResources [

	registeredTextures do: [ :texture | texture destroy ].
	sdlRenderer destroy.
	sdlWindow destroy
]

{ #category : #initialization }
MolSDLRenderer >> initialize [
	registeredTextures := OrderedCollection new.
	SDL2 initVideo.
	self initializeSDLWindow.
	self initializeSDLRenderer 
]

{ #category : #initialization }
MolSDLRenderer >> initializeSDLRenderer [

	sdlRenderer := sdlWindow createAcceleratedRenderer.
	self drawBlackBackground
	"sdlRenderer clear.
	sdlRenderer present."
]

{ #category : #initialization }
MolSDLRenderer >> initializeSDLWindow [

	sdlWindow := SDL2
		          createWindow: 'Molecular'
		          x: SDL_WINDOWPOS_UNDEFINED
		          y: SDL_WINDOWPOS_UNDEFINED
		          width: 1024
		          height: 1204
		          flags: SDL_WINDOW_SHOWN
]

{ #category : #textures }
MolSDLRenderer >> initializeTextureFor: aMolAnimation [

	| texture |
	texture := sdlRenderer
		           createTextureFormat: SDL_PIXELFORMAT_XRGB8888
		           access: SDL_TEXTUREACCESS_STREAMING
		           width: aMolAnimation form width
		           height: aMolAnimation form height.
	texture updateTexturePixels: aMolAnimation form bits pitch: aMolAnimation form width * 4.
	aMolAnimation sdlTexture: texture.
	registeredTextures add: texture
]

{ #category : #rendering }
MolSDLRenderer >> present [
	sdlRenderer present
]

{ #category : #rendering }
MolSDLRenderer >> renderAnimationFrame: aMolAnimation atPosition: aSDLRectangle [

	sdlRenderer
		copy: aMolAnimation sdlTexture
		srcRect: aMolAnimation nextFrame sdlBounds
		dstRect: aSDLRectangle
]

{ #category : #rendering }
MolSDLRenderer >> renderSprite: aSprite [

	aSprite renderOn: self
]

{ #category : #rendering }
MolSDLRenderer >> renderSprite: aSprite atPosition: aSDLRectangle [

	aSprite renderOn: self atPosition: aSDLRectangle
]
