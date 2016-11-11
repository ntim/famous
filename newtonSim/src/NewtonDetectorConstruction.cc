/*
 * NewtonDetectorConstruction.cc
 *
 * @date Aug 1, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "NewtonDetectorConstruction.hh"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <CLHEP/Units/SystemOfUnits.h>

#include <MaterialFactory.hh>

#include "ColorStore.hh"
#include "CoreSensitiveDetector.hh"
#include "model/MessierPN203.hh"

NewtonDetectorConstruction::NewtonDetectorConstruction() :
		G4VUserDetectorConstruction() {
	telescope = new MessierPN203(G4ThreeVector(0, 0, 0), NULL);
}

G4VPhysicalVolume* NewtonDetectorConstruction::Construct() {
	// Create world volume.
	G4Box* world = new G4Box("world", 1. * CLHEP::m, 1. * CLHEP::m, 1. * CLHEP::m);
	G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
			NULL, NULL);
	worldLv->SetVisAttributes(G4VisAttributes::Invisible);
	G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0, true);
	G4VPhysicalVolume* telescopePv = telescope->buildAndPlace(worldPv);
	// Create exit sensitive detector.
	G4VSolid *exitSolid = new G4Tubs("exit", 0, telescope->getEyepieceRadius(), 1e-4 * CLHEP::mm, 0, 360 * CLHEP::deg);
	G4LogicalVolume* exitLv = new G4LogicalVolume(exitSolid, MaterialFactory::getInstance()->getAir(), "exitLv", NULL,
			new CoreSensitiveDetector("exitSd"), NULL);
	exitLv->SetVisAttributes(ColorStore::getInstance()->getRed());
	new G4PVPlacement(telescope->getEyepieceRot(),
			G4ThreeVector(0, telescope->getF() - telescope->getS(), telescope->getEyepiecePos().z()), exitLv, "exitPv",
			telescopePv->GetLogicalVolume(), false, NULL, false);
	//
	return worldPv;
}
