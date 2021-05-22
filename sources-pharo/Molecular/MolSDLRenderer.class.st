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
	#classInstVars : [
		'form'
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
	self initializeSDLRenderer.
	SDL2 initImage: 1
]

{ #category : #initialization }
MolSDLRenderer >> initializeSDLRenderer [

	sdlRenderer := sdlWindow createAcceleratedRenderer.
	"sdlRenderer drawBlendMode: SDL_BLENDMODE_BLEND"
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

	| filePath surface texture |
	"texture := self
		           sdlTextureFromForm: aMolAnimation form
		           width: aMolAnimation form width
		           height: aMolAnimation form height.
	texture blendMode: SDL_BLENDMODE_BLEND."
	filePath := (MolResourceLocator new molecularResourceFileReferenceFor: aMolAnimation dataPath) pathString.
	surface := SDL2 imageLoadFromPath: filePath.
	surface setColorKeyFlag: 1 key: 0.
	
	texture := sdlRenderer createTextureFromSurface: surface .
	texture blendMode: SDL_BLENDMODE_BLEND.
	aMolAnimation sdlTexture: texture.
	registeredTextures add: texture
]

{ #category : #textures }
MolSDLRenderer >> initializeTextureFor: aMolAnimation blend: blend [

	| filePath surface texture |	
	filePath := (MolResourceLocator new molecularResourceFileReferenceFor: aMolAnimation dataPath) pathString.
	surface := SDL2 imageLoadFromPath: filePath.
	
	texture := sdlRenderer createTextureFromSurface: surface .
	texture blendMode: (blend ifTrue:[SDL_BLENDMODE_ADD] ifFalse:[SDL_BLENDMODE_BLEND]).
	aMolAnimation sdlTexture: texture.
	registeredTextures add: texture
]

{ #category : #rendering }
MolSDLRenderer >> present [
	"sdlRenderer drawColorR: 0 g: 0  b: 0  a: 0. "
	sdlRenderer present
]

{ #category : #rendering }
MolSDLRenderer >> renderAnimationFrame: aMolAnimation atPosition: aSDLRectangle [

	self
		renderTexture: aMolAnimation sdlTexture
		frame: aMolAnimation nextFrame sdlBounds
		atPosition: aSDLRectangle
]

{ #category : #rendering }
MolSDLRenderer >> renderSprite: aSprite [

	aSprite renderOn: self
]

{ #category : #rendering }
MolSDLRenderer >> renderSprite: aSprite atPosition: aSDLRectangle [

	aSprite renderOn: self atPosition: aSDLRectangle
]

{ #category : #rendering }
MolSDLRenderer >> renderTexture: texture frame: frame atPosition: aSDLRectangle [

	sdlRenderer copy: texture srcRect: frame dstRect: aSDLRectangle
]

{ #category : #accessing }
MolSDLRenderer >> sdlRenderer [

	^ sdlRenderer
]

{ #category : #textures }
MolSDLRenderer >> sdlTextureFromForm2: form width: w height: h [

	| surface form2 texture |
	form2 := 	form" unhibernate; asFormOfDepth: 32".
	surface := SDL2 createRGBSurfaceFromPixels: form2 bits
			width: form2 width height: form2 height
			depth: 32 pitch: form2 width *4
			rmask: 16r00ff0000 
			gmask: 16r000ff00
			bmask: 16r00000ff
			amask: 16rff000000.
	
	surface setColorKeyFlag: 1 key: 0.

	texture := SDL2 createTextureFromSurface: sdlRenderer  withRenderer: surface .
	
	^ texture
]

{ #category : #textures }
MolSDLRenderer >> sdlTextureFromForm: form width: w height: h [

	| texture |
	texture := sdlRenderer
		           createTextureFormat: SDL_PIXELFORMAT_XRGB8888
		           access: SDL_TEXTUREACCESS_STREAMING
		           width: w
		           height: h.
	texture updateTexturePixels: form bits pitch: w * 4.
	
	^ texture
]
