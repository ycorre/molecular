Class {
	#name : #MolResourceLocatorTest,
	#superclass : #TestCase,
	#instVars : [
		'locator'
	],
	#category : #'Molecular-Tests'
}

{ #category : #running }
MolResourceLocatorTest >> setUp [
	super setUp.
	locator := MolResourceLocator new
]

{ #category : #tests }
MolResourceLocatorTest >> testMolecularConfigurationDirectory [

	| dir |
	self skipOnPharoCITestingEnvironment.
	dir := locator molecularConfigurationDirectory.
	self assert: dir exists.
	self assert: dir isDirectory
]

{ #category : #tests }
MolResourceLocatorTest >> testMolecularResourceFileReferenceFor [
	|resourceFileReference|
	self skipOnPharoCITestingEnvironment.
	resourceFileReference := locator molecularResourceFileReferenceFor: 'res/action/general/abeBkg.png'.
	self assert: resourceFileReference exists.
	self assert: resourceFileReference basename equals: 'abeBkg.png'
]

{ #category : #tests }
MolResourceLocatorTest >> testMolecularResourcePathFor [

	| molecularRepository resourcePath |
	molecularRepository := locator molecularIceRepository.
	resourcePath := locator molecularResourcePathFor:
		                'res/action/general/abeBkg.png'.
	self
		assert: resourcePath
		equals:
		molecularRepository / 'res/action/general/abeBkg.png'
]
