/*
 * FamousDetectorConstruction.cc
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FamousDetectorConstruction.hh"

#include <G4AssemblyVolume.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Polyhedra.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>

#include <G4Sipm.hh>
#include <MaterialFactory.hh>
#include <housing/G4SipmHousing.hh>

#include "Hex.hh"
#include "model/WinstonConeModel.hh"
#include "WinstonCone.hh"
#include "CoreMaterialFactory.hh"
#include "ColorStore.hh"
#include "CoreSensitiveDetector.hh"
#include "FresnelLens.hh"
#include "BackwardPhotonFilter.hh"

FamousDetectorConstruction::FamousDetectorConstruction(FamousModel* _model) :
		G4VUserDetectorConstruction(), model(_model) {
	wicoBackSd = new CoreSensitiveDetector("wicoBackSd", "wicoBackHits");
	wicoBackSd->SetFilter(new BackwardPhotonFilter);
	focalPlaneSd = new CoreSensitiveDetector("focalPlaneSd", "focalPlaneHits");
	focalPlaneSd->SetFilter(new BackwardPhotonFilter);
	fresnelLensBackSd = new CoreSensitiveDetector("fresnelLensBackSd", "fresnelLensBackHits");
	fresnelLensBackSd->SetFilter(new BackwardPhotonFilter);
}

G4LogicalVolume* FamousDetectorConstruction::constructPixel(G4int copyNo) {
	// Create Winston cone.
	WinstonCone* wico = new WinstonCone(model->getWicoModel());
	// Create SiPM.
	G4SipmHousing* sipm = new G4SipmHousing(new G4Sipm(model->getSipmModel()));
	// Gap between Winston cone and UG11 filter.
	double exitDz = 0.2 * CLHEP::mm;
	// Total length of pixel container.
	double pixelDz = wico->getModel()->getLength() + exitDz + model->getUg11FilterThickness() + sipm->getDz();
	double pixelR = wico->getModel()->getR1() + wico->getModel()->getThickness();
	// Create container for the pixel.
	G4Tubs* pixel = new G4Tubs("pixel", 0, pixelR, pixelDz / 2., 0., CLHEP::twopi);
	G4LogicalVolume* pixelLv = new G4LogicalVolume(pixel, CoreMaterialFactory::getInstance()->getAir(), "pixelLv");
	pixelLv->SetVisAttributes(G4VisAttributes::Invisible);
	// Place Winston cone.
	G4LogicalVolume* wicoLv = wico->build();
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, pixelDz / 2. - wico->getModel()->getLength() / 2.), wicoLv, "wicoPv",
			pixelLv, false, copyNo);
	// Create Winston cone exit.
	G4Tubs* exit = new G4Tubs("exit", wico->getModel()->getR2(), pixelR, exitDz / 2.0, 0, CLHEP::twopi);
	G4LogicalVolume* exitLv = new G4LogicalVolume(exit, CoreMaterialFactory::getInstance()->getDummyMatter(), "exitLv");
	exitLv->SetVisAttributes(wicoLv->GetVisAttributes());
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, pixelDz / 2. - wico->getModel()->getLength() - exitDz / 2.), exitLv,
			"exitPv", pixelLv, false, copyNo);
	// Create sensitive element inside the exit area.
	G4Tubs* exitWin = new G4Tubs("exitWin", 0, wico->getModel()->getR1(), exitDz / 2.0, 0, CLHEP::twopi);
	G4LogicalVolume* exitWinLv = new G4LogicalVolume(exitWin, CoreMaterialFactory::getInstance()->getAir(), "exitWinLv",
			NULL, wicoBackSd);
	exitWinLv->SetVisAttributes(G4VisAttributes::Invisible);
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, pixelDz / 2. - wico->getModel()->getLength() - exitDz / 2.), exitWinLv,
			"exitWinPv", pixelLv, false, copyNo);
	// Create light tight container for the SiPM and UG11 filter.
	double contDz = model->getUg11FilterThickness() + sipm->getDz();
	G4Tubs* cont = new G4Tubs("cont", 0, pixelR, contDz / 2.0, 0., CLHEP::twopi);
	G4LogicalVolume* contLv = new G4LogicalVolume(cont, CoreMaterialFactory::getInstance()->getDummyMatter(), "contLv");
	contLv->SetVisAttributes(wicoLv->GetVisAttributes());
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, pixelDz / 2. - wico->getModel()->getLength() - exitDz - contDz / 2.0),
			contLv, "contPv", pixelLv, false, copyNo);
	// Create UG11 filter inside container.
	if (model->getUg11FilterThickness() > 0) {
		G4Box* filter = new G4Box("filter", 9.2 * CLHEP::mm / 2., 8.4 * CLHEP::mm / 2.,
				model->getUg11FilterThickness() / 2.0);
		G4LogicalVolume* filterLv = new G4LogicalVolume(filter, CoreMaterialFactory::getInstance()->getUG11(),
				"filterLv");
		filterLv->SetVisAttributes(new G4VisAttributes(G4Colour(160 / 255., 32 / 255., 240 / 255., 0.3)));
		// Place UG11.
		new G4PVPlacement(NULL, G4ThreeVector(0, 0.4, contDz / 2.0 - model->getUg11FilterThickness() / 2.0), filterLv,
				"filterPv", contLv, false, copyNo);
	}
	// Place SiPM.
	G4LogicalVolume* sipmLv = sipm->build();
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, contDz / 2. - model->getUg11FilterThickness() - sipm->getDz() / 2.0),
			sipmLv, "sipmPv", contLv, false, copyNo);
	//
	return pixelLv;
}

void FamousDetectorConstruction::constructCamera(G4VPhysicalVolume* pv) {
	std::vector<hex::CartesianCoordinate> coords = model->getPixelCoordinates();
	// Determine all offsets and radii.
	double rMax = 0.0;
	for (size_t i = 0; i < coords.size(); i++) {
		const hex::CartesianCoordinate& coord = coords[i];
		double r = sqrt(coord.x * coord.x + coord.y * coord.y);
		// Exact calulation:
		// const double f = model->getFresnelLensModel()->getF() + model->getFresnelLensModel()->getDz();
		// double z = f * (1.0 - cos(asin(r / f)));
		// double alphaR = atan(r / sqrt(f * f - r * r));
		// double alphaX = atan(coord.x / sqrt(f * f - coord.x * coord.x));
		// double alphaY = atan(coord.y / sqrt(f * f - coord.y * coord.y));
		// Petzval field curvature with fitted parameter.
		const double a = model->getCurvatureOfField();
		double z = a * r * r;
		double alphaR = atan(a * r);
		double alphaX = atan(a * coord.x);
		double alphaY = atan(a * coord.y);
		// Compute rotation.
		G4RotationMatrix* rot = new G4RotationMatrix;
		rot->rotateX(-alphaY);
		rot->rotateY(alphaX);
		// Create logical volume and determine its extent.
		G4LogicalVolume* pixelLv = constructPixel(i + 1);
		// Compute pixel shift due to its rotation around the center.
		double dz = ((G4Tubs*) pixelLv->GetSolid())->GetZHalfLength() * cos(alphaR);
		double dx = dz * sin(alphaX);
		double dy = dz * sin(alphaY);
		//
		new G4PVPlacement(rot, G4ThreeVector(coord.x + dx, coord.y + dy, z - dz), "pixelLv", pixelLv, pv, false, i);
		rMax = std::max(r, rMax);
	}
	// Create back plate.
	const double focalPlaneRadius = rMax + model->getWicoModel()->getR1() + model->getWicoModel()->getThickness()
			+ 10. * CLHEP::mm;
	G4Tubs* backPlane = new G4Tubs("backPlane", focalPlaneRadius, model->getFresnelLensModel()->getR(), 0.5 * CLHEP::mm,
			0.0, CLHEP::twopi);
	G4LogicalVolume* backPlaneLv = new G4LogicalVolume(backPlane, CoreMaterialFactory::getInstance()->getDummyMatter(),
			"backPlaneLv");
	backPlaneLv->SetVisAttributes(ColorStore::getInstance()->getBlack());
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -0.5 * CLHEP::mm), "backPlanePv", backPlaneLv, pv, false, 0);
	// Create tube enclosing the focal plane.
	G4Tubs* backTube = new G4Tubs("backTube", focalPlaneRadius, focalPlaneRadius + 1. * CLHEP::mm, 25. * CLHEP::mm, 0.0,
			CLHEP::twopi);
	G4LogicalVolume* backTubeLv = new G4LogicalVolume(backTube, CoreMaterialFactory::getInstance()->getDummyMatter(),
			"backTubeLv");
	backTubeLv->SetVisAttributes(ColorStore::getInstance()->getBlack());
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -25. * CLHEP::mm - 0.5 * CLHEP::mm), "backTubePv", backTubeLv, pv,
			false, 0);
	// Create back plate for the back tube.
	G4Tubs* backTubeBackPlane = new G4Tubs("backTubeBackPlane", 0, focalPlaneRadius, 0.5 * CLHEP::mm, 0.0,
			CLHEP::twopi);
	G4LogicalVolume* backTubeBackPlaneLv = new G4LogicalVolume(backTubeBackPlane,
			CoreMaterialFactory::getInstance()->getDummyMatter(), "backTubeBackPlaneLv");
	backTubeBackPlaneLv->SetVisAttributes(ColorStore::getInstance()->getBlack());
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -50. * CLHEP::mm - 0.5 * CLHEP::mm - 0.5 * CLHEP::mm),
			"backTubeBackPlanePv", backTubeBackPlaneLv, pv, false, 0);
	// Create focal plane sensitive detector.
	G4Tubs* focalPlane = new G4Tubs("focalPlane", 0, focalPlaneRadius, 0.5 * CLHEP::mm, 0.0, CLHEP::twopi);
	G4LogicalVolume* focalPlaneLv = new G4LogicalVolume(focalPlane, CoreMaterialFactory::getInstance()->getAir(),
			"focalPlaneLv");
	focalPlaneLv->SetVisAttributes(new G4VisAttributes(new G4Color(1., 1., 1., 0.1)));
	focalPlaneLv->SetSensitiveDetector(focalPlaneSd);
	focalPlaneLv->SetVisAttributes(G4VisAttributes::Invisible);
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.5 * CLHEP::mm), "focalPlanePv", focalPlaneLv, pv, false, 0);
}

void FamousDetectorConstruction::constructTube(G4VPhysicalVolume* pv) {
	const double length = model->getFresnelLensModel()->getF() + model->getFresnelLensModel()->getDz();
	G4Tubs* tube = new G4Tubs("tube", model->getFresnelLensModel()->getR(),
			model->getFresnelLensModel()->getR() + 1.0 * CLHEP::cm, length / 2.0, 0.0, 2.0 * CLHEP::pi);
	G4LogicalVolume* tubeLv = new G4LogicalVolume(tube, CoreMaterialFactory::getInstance()->getDummyMatter(), "tubeLv");
	tubeLv->SetVisAttributes(ColorStore::getInstance()->getBlack());
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, length / 2.0), "tubePv", tubeLv, pv, false, 0, false);
}

void FamousDetectorConstruction::constructFresnelLens(G4VPhysicalVolume* pv) {
	FresnelLensModel* fresnelLensModel = model->getFresnelLensModel();
	FresnelLens* fresnelLens = new FresnelLens(fresnelLensModel);
	fresnelLens->setPosition(G4ThreeVector(0.0, 0.0, fresnelLensModel->getF() + fresnelLensModel->getDz()));
	fresnelLens->buildAndPlace(pv);
	// Create sensitive detector for photons leaving the lens.
	G4Tubs* fresnelLensBack = new G4Tubs("fresnelLensBack", 0.0, fresnelLensModel->getR(), 0.5 * CLHEP::mm, 0.0,
			CLHEP::twopi);
	G4LogicalVolume* fresnelLensBackLv = new G4LogicalVolume(fresnelLensBack,
			CoreMaterialFactory::getInstance()->getAir(), "fresnelLensBackLv");
	fresnelLensBackLv->SetVisAttributes(new G4VisAttributes(new G4Color(1., 1., 1., 0.3)));
	// Make window sensitive to photons.
	fresnelLensBackLv->SetSensitiveDetector(fresnelLensBackSd);
	fresnelLensBackLv->SetVisAttributes(G4VisAttributes::Invisible);
	new G4PVPlacement(0,
			G4ThreeVector(0.0, 0.0,
					fresnelLensModel->getF() + fresnelLensModel->getDz() - fresnelLensModel->getTotalThickness()
							- 1.0 * CLHEP::mm), "fresnelLensBackPv", fresnelLensBackLv, pv, false, 0);
}

G4VPhysicalVolume* FamousDetectorConstruction::Construct() {
	// Create world volume.
	G4Box* world = new G4Box("world", 1. * CLHEP::m, 1. * CLHEP::m, 1. * CLHEP::m);
	G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
			NULL, NULL);
	worldLv->SetVisAttributes(G4VisAttributes::Invisible);
	G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0, false);
	// Create camera.
	constructCamera(worldPv);
	// Create tube.
	constructTube(worldPv);
	// Create fresnel lens.
	constructFresnelLens(worldPv);
	//
	return worldPv;
}

FamousModel* FamousDetectorConstruction::getModel() const {
	return model;
}
