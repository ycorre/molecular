Extension { #name : #'SDL_Surface' }

{ #category : #'*Molecular' }
SDL_Surface >> setColorKeyFlag: flag key: key [
	^ self ffiCall: #(int SDL_SetColorKey(self, int flag, Uint32 key) )
]
