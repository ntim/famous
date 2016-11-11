/*
 * WicoDetectorConstruction.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "WicoDetectorConstruction.hh"

#include <G4Box.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include <MaterialFactory.hh>
#include <housing/G4SipmHousing.hh>

#include "WinstonCone.hh"
#include "SolidWinstonCone.hh"
#include "HexToRoundWinstonCone.hh"
#include "CoreSensitiveDetector.hh"
#include "ColorStore.hh"

WicoDetectorConstruction::WicoDetectorConstruction(WinstonConeModel* _wicoModel, G4SipmModel* _sipmModel) :
		G4VUserDetectorConstruction(), wicoModel(_wicoModel), sipmModel(_sipmModel) {
	//
}

G4VPhysicalVolume* WicoDetectorConstruction::Construct() {
	G4ThreeVector worldDimension = G4ThreeVector(1. * CLHEP::m, 1. * CLHEP::m, 1. * CLHEP::m);
	// Create world volume.
	G4Box* world = new G4Box("world", worldDimension.x(), worldDimension.y(), worldDimension.z());
	G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
	NULL, NULL);
	worldLv->SetVisAttributes(G4VisAttributes::Invisible);
	G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0, true);
	// Create Winston cone.
	WinstonCone* cone = NULL;
	if (wicoModel->isSolid()) {
		cone = new SolidWinstonCone(wicoModel);
	} else {
		cone = new WinstonCone(wicoModel);
	}
	cone->buildAndPlace(worldPv);
	// Determine position of the sensitive component.
	G4ThreeVector sdPos(0, 0, -wicoModel->getLength() / 2.);
	// Create sensitive component.
	if (sipmModel) {
		G4SipmHousing* sipm = new G4SipmHousing(new G4Sipm(sipmModel));
		sipm->setPosition(sdPos - G4ThreeVector(0, 0, sipmModel->getThickness() / 2.));
		sipm->buildAndPlace(worldPv);
	} else {
		G4Box* sd = new G4Box("sd", wicoModel->getR2() * 2.0 / sqrt(3), wicoModel->getR2() * 2.0 / sqrt(3),
				0.5 * CLHEP::mm);
		G4LogicalVolume* sdLv = new G4LogicalVolume(sd, MaterialFactory::getInstance()->getAir(), "sdLv", NULL,
				new CoreSensitiveDetector("g4sipmHits-0"), NULL);
		sdLv->SetVisAttributes(ColorStore::getInstance()->getRed());
		new G4PVPlacement(NULL, sdPos - G4ThreeVector(0, 0, 0.5 * CLHEP::mm), "sdPv", sdLv, worldPv, false, 0, false);
	}
	return worldPv;
}

WinstonConeModel* WicoDetectorConstruction::getWicoModel() const {
	return wicoModel;
}
