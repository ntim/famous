/**
 * WinstonCone.cc
 *
 * @date Dec 2, 2010
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "WinstonCone.hh"

#include <cmath>

#include <G4VSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <CLHEP/Units/PhysicalConstants.h>

#include "CoreMaterialFactory.hh"
#include "ColorStore.hh"

WinstonCone::WinstonCone(WinstonConeModel* m) :
		Placeable(), model(m) {
}

WinstonCone::~WinstonCone() {
	//
}

void WinstonCone::calculateZPlanes(G4double* z, G4double* rI, G4double* rO, const int n) {
	const double thetaMax = model->getThetaMax();
	const double delta = (CLHEP::halfpi - thetaMax) / (double) n;
	double theta = thetaMax;
	// TODO: dynamically adjust precision with first derivative.
	for (int i = 0; i < n; ++i) {
		z[i] = model->getZ(theta);
		const double r = model->getR(theta);
		rI[i] = r;
		rO[i] = r + model->getThickness();
		theta += delta;
		if (model->isOuterFaceStraight()) {
			rO[i] = model->getR1() + model->getThickness();
		}
	}
}

G4VSolid* WinstonCone::createSolid() {
	const int n = 256;
	G4double* z = new G4double[n];
	G4double* rI = new G4double[n];
	G4double* rO = new G4double[n];
	calculateZPlanes(z, rI, rO, n);
	// Create round one.
	if (model->getNumberOfSides() == 0) {
		return new G4Polycone("wicoPoly", 0, CLHEP::twopi, n, z, rI, rO);
	}
	// Create e.g. hexagonal one.
	return new G4Polyhedra("wicoPoly", 0, CLHEP::twopi, model->getNumberOfSides(), n, z, rI, rO);
}

G4LogicalVolume* WinstonCone::build() {
	G4VSolid* solid = createSolid();
	G4Material* material = model->getMaterial();
	// Create the logical volume.
	G4LogicalVolume *logical = new G4LogicalVolume(solid, material, "wicoLV", 0, 0, 0);
	logical->SetVisAttributes(ColorStore::getInstance()->getMetal());
	// Create optical, reflective surface.
	G4OpticalSurface* opticalSurface = new G4OpticalSurface("wicoSurface", unified, ground, dielectric_metal);
	G4MaterialPropertiesTable* mpt = material->GetMaterialPropertiesTable();
	// Retreive surface roughness setting from the model.
	opticalSurface->SetSigmaAlpha(model->getSurfaceSigmaAlpha());
	opticalSurface->SetMaterialPropertiesTable(mpt);
	new G4LogicalSkinSurface("wicoSkin", logical, opticalSurface);
	//
	return logical;
}

WinstonConeModel* WinstonCone::getModel() const {
	return model;
}
