Class {
	#name : #BaselineOfMolecular,
	#superclass : #BaselineOf,
	#category : #BaselineOfMolecular
}

{ #category : #baselines }
BaselineOfMolecular >> baseline: spec [

	<baseline>
	spec for: #common do: [ 
		spec package: 'Molecular'.
		spec package: 'Molecular-Tests' ]
]
