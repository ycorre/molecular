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
MolResourceLocator >> molecularConfiguration [
	"Return the Molecular game json configuration"
]

{ #category : #repository }
MolResourceLocator >> molecularIceRepository [

	molecularIceRepository := IceRepository repositories
		                 detect: [ :d | 
			                 d location notNil and: [ 
				                 d origin owner = 'ycorre' and: [ 
					                 d origin projectName = 'molecular' ] ] ]
		                 ifNone: [ MolResourceRepositoryNotFound signal ]
]

{ #category : #repository }
MolResourceLocator >> molecularResourceFileReferenceFor: aDataPath [
	^(self molecularResourcePathFor: aDataPath) asFileReference

]

{ #category : #repository }
MolResourceLocator >> molecularResourcePathFor: aDataPath [


]
