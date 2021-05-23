"
Please describe the package using the class comment of the included manifest class. The manifest class also includes other additional metadata for the package. These meta data are used by other tools such as the SmalllintManifestChecker and the critics Browser
"
Class {
	#name : #ManifestMolecularTests,
	#superclass : #PackageManifest,
	#category : #'Molecular-Tests-Manifest'
}

{ #category : #'code-critics' }
ManifestMolecularTests class >> ruleLongMethodsRuleV1FalsePositive [
	^ #(#(#(#RGPackageDefinition #(#'Molecular-Tests')) #'2021-05-23T21:11:02.627768+02:00') )
]
