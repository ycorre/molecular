Extension { #name : #SDL2 }

{ #category : #'*Molecular' }
SDL2 class >> imageLoadFromPath: file [

	^ self
		  ffiCall: #( SDL_Surface * IMG_Load #( const char * file ) )
		  library: 'libSDL2_image.dylib'
]
