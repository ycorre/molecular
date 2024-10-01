"
I read the state of the receiver from a dictionary by calling the corresponding accessors by name.
If those accessors do not exist in the class hierarchy, I create them automatically.
"
Trait {
	#name : 'TMolDictionnaryReader',
	#category : 'Molecular-core',
	#package : 'Molecular',
	#tag : 'core'
}

{ #category : 'reading' }
TMolDictionnaryReader >> readFromDictionnary: aDictionnary [

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
