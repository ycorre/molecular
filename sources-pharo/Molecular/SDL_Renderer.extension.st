Extension { #name : 'SDL_Renderer' }

{ #category : '*Molecular' }
SDL_Renderer >> createTextureFromSurface:surface [

	^ self ffiCall: #( SDL_Texture SDL_CreateTextureFromSurface
		     #( self , SDL_Surface *surface ) )
]
