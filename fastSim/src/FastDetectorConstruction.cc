/*
 * FastDetectorConstruction.cc
 *
 * @date Feb 6, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FastDetectorConstruction.hh"

#include <G4Box.hh>
#include <G4Sphere.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialPropertyVector.hh>
#include <G4VisExtent.hh>

#include <CLHEP/Vector/Rotation.h>
#include <CLHEP/Vector/RotationX.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include <MaterialFactory.hh>

#include "FresnelLens.hh"
#include "CoreMaterialFactory.hh"
#include "ColorStore.hh"
#include "CoreSensitiveDetector.hh"
#include "WinstonCone.hh"
#include "model/WinstonConeModelFactory.hh"

FastDetectorConstruction::FastDetectorConstruction(FresnelLensModel* _model, bool _createWinstonCone,
		bool _reversedLens) :
		model(_model), createWinstonCone(_createWinstonCone), reversedLens(_reversedLens) {
	// Start values for a Hamamamatsu R5912 PMT.
	hemisphereRadius = 131. * CLHEP::mm;
	hemisphereDiameter = 202. * CLHEP::mm;
	winstonConeDiameter = 269. * CLHEP::mm;
}

G4Material* FastDetectorConstruction::createNBK7() {
	G4Material* bk7 = new G4Material("bk7", 1., 2.51 * CLHEP::g / CLHEP::mole, 2.51 * CLHEP::g / CLHEP::cm3,
			kStateSolid);
	// Add refractive index.
	G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
	G4MaterialPropertyVector* rIndexVector = new G4MaterialPropertyVector();
	const int nRIndex = 128;
	const double eMin = CLHEP::h_Planck * CLHEP::c_light / CoreMaterialFactory::LAMBDA_MAX;
	const double eMax = CLHEP::h_Planck * CLHEP::c_light / CoreMaterialFactory::LAMBDA_MIN;
	const double dE = (eMax - eMin) / nRIndex;
	for (double e = eMin; e <= eMax; e += dE) {
		double lambda = CLHEP::h_Planck * CLHEP::c_light / e / CLHEP::micrometer;
		// Reference: http://refractiveindex.info/?group=GLASSES&material=BK7
		double rIndex = sqrt(
				1 + 1.03961212 * pow(lambda, 2) / (pow(lambda, 2) - 0.00600069867)
						+ 0.231792344 * pow(lambda, 2) / (pow(lambda, 2) - 0.0200179144)
						+ 1.01046945 * pow(lambda, 2) / (pow(lambda, 2) - 103.560653));
		rIndexVector->InsertValues(e, rIndex);
	}
	mpt->AddProperty("RINDEX", rIndexVector);
	bk7->SetMaterialPropertiesTable(mpt);
	return bk7;
}

G4VPhysicalVolume* FastDetectorConstruction::Construct() {
	// Create world volume.
	G4Box* world = new G4Box("world", 1. * CLHEP::m, 1. * CLHEP::m, 1. * CLHEP::m);
	G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
			NULL, NULL);
	worldLv->SetVisAttributes(G4VisAttributes::Invisible);
	G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0, true);
	// Build Fresnel lens.
	FresnelLens* lens = new FresnelLens(model);
	if (reversedLens) {
		lens->setRotation(new CLHEP::HepRotation(CLHEP::HepRotationX(180. * CLHEP::deg)));
	}
	lens->buildAndPlace(worldPv);
	//
	const double windowThickness = 0.1 * CLHEP::mm;
	const double fpThickness = 0.1 * CLHEP::mm;
	const double thetaMax = asin(hemisphereDiameter / 2. / hemisphereRadius);
	const double hemisphereHeight = hemisphereRadius - sqrt(pow(hemisphereRadius, 2) - pow(hemisphereDiameter / 2., 2));
	G4ThreeVector fpPos(0, 0, -model->getF() - model->getDz() - hemisphereRadius);
	G4Material* nbk7 = createNBK7();
	// Create Winston cone.
	if (createWinstonCone) {
		WinstonCone* wico = new WinstonCone(
				WinstonConeModelFactory::createRound(winstonConeDiameter / 2., hemisphereDiameter / 2.));
		G4ThreeVector wicoPos = G4ThreeVector(0, 0,
				-model->getF() - model->getDz() - wico->getModel()->getLength() / 2.);
		wico->setPosition(wicoPos);
		wico->buildAndPlace(worldPv);
		fpPos += G4ThreeVector(0, 0, -wico->getModel()->getLength() + hemisphereHeight);
	}
	// Create hemispheric window.
	G4Sphere* window = new G4Sphere("window", hemisphereRadius - windowThickness, hemisphereRadius, 0, CLHEP::twopi, 0,
			thetaMax);
	G4LogicalVolume* windowLV = new G4LogicalVolume(window, nbk7, "windowLV", NULL, NULL, NULL);
	windowLV->SetVisAttributes(ColorStore::getInstance()->getBlue());
	new G4PVPlacement(0, fpPos, windowLV, "windowPV", worldPv->GetLogicalVolume(), false, 0, false);
	// Create focal plane.
	G4Sphere* focalPlane = new G4Sphere("focalPlane", hemisphereRadius - windowThickness - fpThickness,
			hemisphereRadius - windowThickness, 0, CLHEP::twopi, 0, thetaMax);
	G4LogicalVolume* focalPlaneLV = new G4LogicalVolume(focalPlane, nbk7, "focalPlaneLV", NULL,
			new CoreSensitiveDetector("fpSd"), NULL);
	focalPlaneLV->SetVisAttributes(ColorStore::getInstance()->getRed());
	new G4PVPlacement(0, fpPos, focalPlaneLV, "focalPlanePV", worldPv->GetLogicalVolume(), false, 0, false);
	return worldPv;
}

FresnelLensModel* FastDetectorConstruction::getModel() const {
	return model;
}

void FastDetectorConstruction::setModel(FresnelLensModel* _model) {
	model = _model;
}
