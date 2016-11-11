/**
 * SolidWinstonCone.cc
 *
 * @date 04.04.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "SolidWinstonCone.hh"

#include <G4VSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>

#include "ColorStore.hh"
#include "CoreMaterialFactory.hh"

SolidWinstonCone::SolidWinstonCone(WinstonConeModel* _model) :
		WinstonCone(_model) {
	//
}

SolidWinstonCone::~SolidWinstonCone() {
	//
}

void SolidWinstonCone::calculateZPlanes(G4double* z, G4double* rI, G4double* rO, const int n) {
	WinstonCone::calculateZPlanes(z, rI, rO, n);
	for (int i = 0; i < n; ++i) {
		rO[i] = rI[i];
		rI[i] = 0;
	}
}

G4LogicalVolume* SolidWinstonCone::build() {
	// TODO: unify duplicate code with WinstonCone::build().
	G4VSolid* solid = createSolid();
	G4Material* material = CoreMaterialFactory::getInstance()->getAcrylic();
	// Create the logical volume.
	G4LogicalVolume *logical = new G4LogicalVolume(solid, material, "wicoLV", 0, 0, 0);
	logical->SetVisAttributes(ColorStore::getInstance()->getMetal());
	// Create optical, reflective surface.
	G4OpticalSurface* opticalSurface = new G4OpticalSurface("wicoSurface", glisur, polished, dielectric_dielectric);
	opticalSurface->SetMaterialPropertiesTable(material->GetMaterialPropertiesTable());
	new G4LogicalSkinSurface("wicoSkin", logical, opticalSurface);
	//
	return logical;
}
