"
I provide simple naming to Molecular entities.
"
Trait {
	#name : 'TMolNamedEntity',
	#instVars : [
		'name'
	],
	#category : 'Molecular-core',
	#package : 'Molecular',
	#tag : 'core'
}

{ #category : 'accessing' }
TMolNamedEntity >> name [

	^ name
]

{ #category : 'accessing' }
TMolNamedEntity >> name: anObject [

	name := anObject
]
