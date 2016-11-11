/**
 * FresnelLens.cc
 *
 * @date 19.03.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelLens.hh"

#include <boost/format.hpp>

#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4VisAttributes.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>

#include "ColorStore.hh"
#include "CoreMaterialFactory.hh"
#include "FresnelLensParametrisation.hh"

FresnelLens::FresnelLens(FresnelLensModel* _model, double _rMin, double _startPhi, double _dPhi) :
		Placeable(), model(_model), rMin(_rMin), startPhi(_startPhi), dPhi(_dPhi) {
	setName("fresnelLens");
}

G4LogicalVolume* FresnelLens::createLensMother() {
	G4VSolid* solid = new G4Tubs("motherSolid", rMin, getRMax(), model->getTotalThickness() / 2., startPhi, dPhi);
	G4LogicalVolume *motherLv = new G4LogicalVolume(solid, CoreMaterialFactory::getInstance()->getAir(), "motherLv");
	motherLv->SetVisAttributes(G4VisAttributes::Invisible);
	return motherLv;
}

G4LogicalVolume* FresnelLens::createAndPlaceGroovesMother(G4LogicalVolume* motherLv) {
	const double thickness = model->getThickness();
	const double totalThickness = model->getTotalThickness();
	const double heighestGroove = totalThickness - thickness;
	G4Tubs* grooves = new G4Tubs("grooves", rMin, getRMax(), heighestGroove / 2., startPhi, dPhi);
	G4LogicalVolume* groovesLv = new G4LogicalVolume(grooves, CoreMaterialFactory::getInstance()->getAir(), "groovesLV",
	NULL, NULL, NULL);
	G4ThreeVector groovesPosition(0, 0, totalThickness / 2. - heighestGroove / 2.);
	groovesLv->SetVisAttributes(G4VisAttributes::Invisible);
	new G4PVPlacement(NULL, groovesPosition, groovesLv, "groovesPV", motherLv, false, 0, false);
	return groovesLv;
}

G4LogicalVolume* FresnelLens::createGroovePrototype() {
	G4Cons* groove = new G4Cons("groovePrototype", 0, 0, 0, 0, 0, 0, CLHEP::twopi);
	G4LogicalVolume* grooveLv = new G4LogicalVolume(groove, model->getMaterial(), "groovePrototypeLv");
	grooveLv->SetVisAttributes(ColorStore::getInstance()->getTranslucentBlue());
	return grooveLv;
}

G4LogicalVolume* FresnelLens::createAndPlaceLensSupport(G4LogicalVolume* motherLv) {
	const double thickness = model->getThickness();
	G4Tubs* support = new G4Tubs("support", rMin, getRMax(), thickness / 2., startPhi, dPhi);
	G4LogicalVolume* supportLv = new G4LogicalVolume(support, model->getMaterial(), "supportLv", NULL, NULL, NULL);
	G4ThreeVector supportPosition(0, 0, -model->getTotalThickness() / 2. + thickness / 2.);
	supportLv->SetVisAttributes(ColorStore::getInstance()->getTranslucentBlue());
	new G4PVPlacement(NULL, supportPosition, supportLv, "supportPV", motherLv, false, 0, false);
	return supportLv;
}

G4LogicalVolume* FresnelLens::build() {
	// Create enclosing mother volume.
	G4LogicalVolume* lensMotherLv = createLensMother();
	G4LogicalVolume* groovesMotherLv = createAndPlaceGroovesMother(lensMotherLv);
	//	 Imprint grooves.
	new G4PVParameterised("parameterised", createGroovePrototype(), groovesMotherLv, kZAxis,
			model->getTotalNumberOfGrooves(), new FresnelLensParametrisation(model));
	// Create support marterial.
	if (model->getThickness() > 0) {
		createAndPlaceLensSupport(lensMotherLv);
	}
	//
	return lensMotherLv;
}

double FresnelLens::getRMax() const {
	return model->getR();
}

double FresnelLens::getRMin() const {
	return rMin;
}

void FresnelLens::setRMin(double _rMin) {
	rMin = _rMin;
}

double FresnelLens::getDPhi() const {
	return dPhi;
}

void FresnelLens::setDPhi(double _dPhi) {
	dPhi = _dPhi;
}

double FresnelLens::getStartPhi() const {
	return startPhi;
}

void FresnelLens::setStartPhi(double _startPhi) {
	startPhi = _startPhi;
}
