"
I represent a graphic entity displayable on a graphics medium.
"
Class {
	#name : #MolGraphicEntity,
	#superclass : #Object,
	#traits : 'TMolMaterializer + TMolNamedEntity',
	#classTraits : 'TMolMaterializer classTrait + TMolNamedEntity classTrait',
	#instVars : [
		'position',
		'spritesConfigurations',
		'displaySprite',
		'glowSprite'
	],
	#category : #'Molecular-model - 2D graphics'
}
