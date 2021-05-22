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
MolResourceLocator >> configurationFileNamed: aString [
	^self molecularConfigurationDirectory / aString
]

{ #category : #'resource access' }
MolResourceLocator >> jsonConfigurationNamed: aString [

	^ STONJSON fromString:
		  (self configurationFileNamed: aString) contents
]

{ #category : #'resource access' }
MolResourceLocator >> molecularConfigurationDirectory [
	"Return the Molecular game json configuration directory"
		^self molecularResourceFileReferenceFor: 'conf'
]

{ #category : #repository }
MolResourceLocator >> molecularIceRepository [

	^ molecularIceRepository ifNil: [ 
		  molecularIceRepository := FileSystem workingDirectory
		                            / 'pharo-local' / 'iceberg' / 'ycorre'
		                            / 'molecular' ]
]

{ #category : #repository }
MolResourceLocator >> molecularResourceFileReferenceFor: aDataPath [
	^(self molecularResourcePathFor: aDataPath) asFileReference

]

{ #category : #repository }
MolResourceLocator >> molecularResourcePathFor: aDataPath [

	^ self molecularIceRepository / aDataPath
]
