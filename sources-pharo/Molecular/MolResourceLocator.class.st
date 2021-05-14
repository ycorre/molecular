"
I locate the resources of the Molecular game and return on-demand file paths to those resources.
"
Class {
	#name : #MolResourceLocator,
	#superclass : #Object,
	#instVars : [
		'molecularIceRepository'
	],
	#category : #'Molecular-core'
}

{ #category : #'resource access' }
MolResourceLocator >> molecularConfigurationDirectory [
	"Return the Molecular game json configuration directory"
		^self molecularResourceFileReferenceFor: 'conf'
]

{ #category : #repository }
MolResourceLocator >> molecularIceRepository [

	^ molecularIceRepository ifNil: [ 
		  molecularIceRepository := IceRepository repositories
			                            detect: [ :d | 
				                            d location notNil and: [ 
					                            d origin owner = 'ycorre' and: [ 
						                            d origin projectName = 'molecular' ] ] ]
			                            ifNone: [ 
			                            MolResourceRepositoryNotFound signal ] ]
]

{ #category : #repository }
MolResourceLocator >> molecularResourceFileReferenceFor: aDataPath [
	^(self molecularResourcePathFor: aDataPath) asFileReference

]

{ #category : #repository }
MolResourceLocator >> molecularResourcePathFor: aDataPath [

	^ self molecularIceRepository location absolutePath / aDataPath
]
