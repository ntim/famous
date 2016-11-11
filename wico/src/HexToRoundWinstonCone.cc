/*
 * HexToRoundWinstonCone.cc
 *
 * @date Aug 26, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <HexToRoundWinstonCone.hh>

#include <G4Tubs.hh>
#include <G4VSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4UnionSolid.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>

#include <CLHEP/Units/PhysicalConstants.h>

#include "CoreMaterialFactory.hh"
#include "ColorStore.hh"

HexToRoundWinstonCone::HexToRoundWinstonCone(HexagonalWinstonConeModel* _model) :
		WinstonCone(_model) {
	//
}

G4LogicalVolume* HexToRoundWinstonCone::build() {
	WinstonConeModel* _model = getModel();
	_model->setThickness(1.0 * CLHEP::mm);
	// Create cone solid.
	G4VSolid* wico = createSolid();
	// Create round cut out.
	G4Tubs* cut = new G4Tubs("cut", 0, _model->getR2() * 2.0 / sqrt(3), _model->getLength(), 0, CLHEP::twopi);
	// Subtract.
	G4SubtractionSolid* subtraction = new G4SubtractionSolid("subtraction", wico, cut);
//	// Create additional tubs for the exit.
//	G4Tubs* add = new G4Tubs("cut", _model->getR2() * 2.0 / sqrt(3),
//			_model->getR2() * 2.0 / sqrt(3) + _model->getThickness(), 5 * CLHEP::mm, 0, CLHEP::twopi);
//	// Add solid.
//	G4UnionSolid* uni = new G4UnionSolid("uni", subtraction, add, 0, G4ThreeVector(0, 0, -_model->getLength() / 2.0));
	// Create the logical volume.
	G4Material* material = CoreMaterialFactory::getInstance()->getAluminum();
	G4LogicalVolume *logical = new G4LogicalVolume(subtraction, material, "wicoLV", 0, 0, 0);
	logical->SetVisAttributes(ColorStore::getInstance()->getMetal());
	// Create optical, reflective surface.
	G4OpticalSurface* opticalSurface = new G4OpticalSurface("wicoSurface", unified, ground, dielectric_metal);
	G4MaterialPropertiesTable* mpt = material->GetMaterialPropertiesTable();
	// Retreive surface roughness setting from the model.
	opticalSurface->SetSigmaAlpha(_model->getSurfaceSigmaAlpha());
	opticalSurface->SetMaterialPropertiesTable(mpt);
	new G4LogicalSkinSurface("wicoSkin", logical, opticalSurface);
	//
	return logical;
}
