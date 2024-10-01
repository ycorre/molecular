"
I provide mechanics to read molecular model objects from json strings and dictionnaries.
I therefore should be used by all molecular objects that needs to be read from json format.

"
Trait {
	#name : 'TMolMaterializer',
	#category : 'Molecular-core',
	#package : 'Molecular',
	#tag : 'core'
}

{ #category : 'reading' }
TMolMaterializer classSide >> readFromString: aString [
	|objectDictionary|
	objectDictionary := STONJSON fromString: aString.
	^self new readFromDictionnary: objectDictionary
]

{ #category : 'reading' }
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
	^ self
]
