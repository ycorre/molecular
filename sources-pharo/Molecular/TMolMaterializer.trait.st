"
I provide mechanics to read molecular model objects from json strings and dictionnaries.
I therefore should be used by all molecular objects that needs to be read from json format.

"
Trait {
	#name : #TMolMaterializer,
	#category : #'Molecular-core'
}

{ #category : #reading }
TMolMaterializer classSide >> readFromString: aString [
	|objectDictionary|
	objectDictionary := STONJSON fromString: aString.
	^self new readFromDictionnary: objectDictionary
]

{ #category : #building }
TMolMaterializer >> build [

	^ self
]

{ #category : #reading }
TMolMaterializer >> readFromDictionnary: aDictionnary [

	aDictionnary keysAndValuesDo: [ :selector :value | 
		self perform: selector asSymbol asMutator with: value ].
	self build.
	^ self
]
