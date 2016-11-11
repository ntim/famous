/*
 * FresnelDetectorConstruction.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelDetectorConstruction.hh"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <MaterialFactory.hh>

#include "ColorStore.hh"
#include "CoreSensitiveDetector.hh"
#include "FresnelLens.hh"

FresnelDetectorConstruction::FresnelDetectorConstruction(FresnelLensModel* _model) :
		G4VUserDetectorConstruction(), model(_model) {
	//
}

G4VPhysicalVolume* FresnelDetectorConstruction::Construct() {
	// Create world volume.
	G4Box* world = new G4Box("world", 1. * CLHEP::m, 1. * CLHEP::m, 1. * CLHEP::m);
	G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
	NULL, NULL);
	worldLv->SetVisAttributes(G4VisAttributes::Invisible);
	G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0, true);
	// Build Frensel lens.
	FresnelLens* lens = new FresnelLens(model);
	lens->buildAndPlace(worldPv);
	// Create focal plane.
	const double fpThickness = 0.1 * CLHEP::mm;
	G4Tubs* focalPlane = new G4Tubs("focalPlane", 0, model->getR(), fpThickness, 0, CLHEP::twopi);
	G4LogicalVolume* focalPlaneLV = new G4LogicalVolume(focalPlane, CoreMaterialFactory::getInstance()->getAir(),
			"focalPlaneLV", NULL, new CoreSensitiveDetector("focalPlaneSd", "hits", true), NULL);
	focalPlaneLV->SetVisAttributes(ColorStore::getInstance()->getRed());
	const G4ThreeVector fpPos(0, 0, -(model->getF() + model->getDz()));
	new G4PVPlacement(0, fpPos, focalPlaneLV, "focalPlanePV", worldPv->GetLogicalVolume(), false, 0, false);
	//
	return worldPv;
}

FresnelLensModel* FresnelDetectorConstruction::getModel() const {
	return model;
}
