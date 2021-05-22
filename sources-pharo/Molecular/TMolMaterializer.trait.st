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

{ #category : #building }
TMolMaterializer >> buildFormFromDataPath: dataPath [

	| form |
	form := ImageReadWriter formFromFileNamed:
		        (MolResourceLocator new molecularResourceFileReferenceFor:
			         dataPath).
	form mapColor: Color black to: Color transparent.
	^ form collectColors: [ :c | 
		  c = Color black
			  ifTrue: [ c alpha: 0 ]
			  ifFalse: [ c ] ]
]

{ #category : #reading }
TMolMaterializer >> readFromDictionnary: aDictionnary [

	aDictionnary keysAndValuesDo: [ :selector :value | 
		(self class withAllSuperclasses anySatisfy: [ :c | 
			 c hasInstVarNamed: selector ]) ifFalse: [ 
			self class addInstVarNamed: selector.
			(RBCreateAccessorsForVariableRefactoring
				 variable: selector
				 class: self class
				 classVariable: false)
				defineGetterMethod;
				defineSetterMethod;
				execute ].
		self perform: selector asSymbol asMutator with: value ].
	self build.
	^ self
]
