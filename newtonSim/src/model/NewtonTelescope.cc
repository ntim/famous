/*
 * NewtonTelescope.cc
 *
 * @date 27.01.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/NewtonTelescope.hh"

#include <G4LogicalVolume.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4VSolid.hh>
#include <G4Tubs.hh>
#include <G4PVPlacement.hh>
#include <G4Paraboloid.hh>
#include <G4EllipticalTube.hh>
#include <G4Box.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4VisAttributes.hh>

#include "CoreMaterialFactory.hh"
#include "ColorStore.hh"

NewtonTelescope::NewtonTelescope(G4String _name, G4ThreeVector _position, G4RotationMatrix* _rotation, G4double _rH,
		G4double _rF, G4double _f, G4double _tubusLength, G4double _eyepieceRadius, G4double _eyepieceLength) :
		Placeable(), rH(_rH), rF(_rF), f(_f), s(0.0), a(0.0), b(0.0), delta(0.0), tubusLength(_tubusLength), eyepieceRadius(
				_eyepieceRadius), eyepieceLength(_eyepieceLength), eyepieceRot(0), mirrorLength(0.0) {
	setName(_name);
	setRotation(_rotation);
	setPosition(_position);
	tubusRadius = rH;
	tubusThickness = 3. * CLHEP::mm;
	mirrorThickness = 3. * CLHEP::mm;
	spiderThickness = 1. * CLHEP::mm;
	spiderLength = 1. * CLHEP::cm;
	spiderShift = 1. * CLHEP::cm;
	secondaryMirrorThickness = 1. * CLHEP::mm;
}

void NewtonTelescope::initParameters() {
	G4double tanTheta = rH / f;
	s = f - 2. * rF / rH / (1. / (f + rH) + 1. / (f - rH));
	G4double a1 = (f - s) * rH / (f + rH);
	G4double a2 = (f - s) * rH / (f - rH);
	delta = (a1 + a2) / 2. - a1;
	a = (a1 + a2) / sqrt(2.);
	b = sqrt(pow((f - s + delta) * tanTheta, 2) - delta * delta);
	mirrorLength = rH * rH / (4. * f);
	// Determine eyepiece position and rotation.
	eyepiecePos = G4ThreeVector(0, tubusRadius + eyepieceLength / 2. - tubusThickness, s - tubusLength / 2.);
	eyepieceRot = new G4RotationMatrix();
	eyepieceRot->rotateX(CLHEP::pi / 2.);
	secondaryMirrorPos = G4ThreeVector(0, -delta, s - tubusLength / 2. - delta + secondaryMirrorThickness / 2.);
}

G4LogicalVolume* NewtonTelescope::build() {
	initParameters();
	G4Material* mirrorMaterial = CoreMaterialFactory::getInstance()->getAluminum();
	// Create a bounding volume.
	G4VSolid* body = new G4Box(getName(), tubusRadius + tubusThickness + eyepieceLength,
			tubusRadius + tubusThickness + eyepieceLength, tubusLength / 2. + tubusThickness * 2.);
	G4LogicalVolume* bodyLV = new G4LogicalVolume(body, CoreMaterialFactory::getInstance()->getAir(), getName() + "LV",
			0, 0, 0);
	bodyLV->SetVisAttributes(G4VisAttributes::Invisible);
	//
	// Main tube.
	G4VSolid* fullTube = new G4Tubs("fullTube", tubusRadius, tubusRadius + tubusThickness, tubusLength / 2., 0,
			360 * CLHEP::deg);
	// Hole for eyepiece.
	G4VSolid* eyePieceHole = new G4Tubs("eyePieceHole", 0, eyepieceRadius + tubusThickness, eyepieceLength / 2., 0,
			360 * CLHEP::deg);
	// Eyepiece.
	G4VSolid* eyePiece = new G4Tubs("eyePieceHole", eyepieceRadius, eyepieceRadius + tubusThickness,
			eyepieceLength / 2., 0, 360 * CLHEP::deg);
	//
	// Substract the eyepiece from the tube.
	G4SubtractionSolid *tubeWithHole = new G4SubtractionSolid("tubeWithHole", fullTube, eyePieceHole, eyepieceRot,
			eyepiecePos);
	// Insert the eyepiece and place it.
	G4UnionSolid* tube = new G4UnionSolid("tube", tubeWithHole, eyePiece, eyepieceRot, eyepiecePos);
	G4LogicalVolume* tubeLV = new G4LogicalVolume(tube, CoreMaterialFactory::getInstance()->getDummyMatter(), "tubeLV",
			0, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(0, 0, 0), tubeLV, "tubePV", bodyLV, false, 0, false);
	//
	// Create back plate.
	G4VSolid* basePlate = new G4Tubs("basePlate", 0, tubusRadius, tubusThickness, 0, 360 * CLHEP::deg);
	G4LogicalVolume* basePlateLV = new G4LogicalVolume(basePlate, CoreMaterialFactory::getInstance()->getDummyMatter(),
			"basePlateLogical", 0, 0, 0);
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, -tubusLength / 2. - mirrorLength / 2. - tubusThickness / 2.),
			basePlateLV, "basePlatePhysical", bodyLV, false, 0, false);
	//
	// Create main mirror.
	G4VSolid* mainMirrorInner = new G4Paraboloid("mainMirrorInner", mirrorLength / 2., 0, rH);
	G4VSolid* mainMirrorOuter = new G4Tubs("mainMirrorOuter", 0, rH + mirrorThickness, mirrorLength / 2., 0,
			360. * CLHEP::deg);
	G4SubtractionSolid* mainMirror = new G4SubtractionSolid("mainMirror", mainMirrorOuter, mainMirrorInner);
	G4LogicalVolume* mainMirrorLV = new G4LogicalVolume(mainMirror, mirrorMaterial, "mainMirrorLogical", 0, 0, 0);
	applyMaterialProperties(mainMirrorLV);
	mainMirrorLV->SetVisAttributes(ColorStore::getInstance()->getMetal());
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, -tubusLength / 2. + mirrorLength / 2.), mainMirrorLV,
			"mainMirrorPhysical", bodyLV, false, 0, false);
	//
	// Create secondary mirror.
	G4VSolid* secondaryMirror = new G4EllipticalTube("secondaryMirror", b + 0.5 * CLHEP::mm, a + .5 * CLHEP::mm,
			secondaryMirrorThickness / 2.);
	G4LogicalVolume* secondaryMirrorLV = new G4LogicalVolume(secondaryMirror, mirrorMaterial, "secondaryMirrorLV", 0, 0,
			0);
	applyMaterialProperties(secondaryMirrorLV);
	secondaryMirrorLV->SetVisAttributes(ColorStore::getInstance()->getMetal());
	G4RotationMatrix* secondaryMirrorRot = new G4RotationMatrix();
	secondaryMirrorRot->rotateX(-CLHEP::pi / 4.);
	new G4PVPlacement(secondaryMirrorRot, secondaryMirrorPos, secondaryMirrorLV, "secondaryMirrorPhysical", bodyLV,
			false, 0, false);
	// Create 4 vane spider.
	G4VSolid* spiderVane = new G4Box("spiderVane12", tubusRadius, spiderThickness / 2., spiderLength / 2.);
	G4RotationMatrix* vaneRot = new G4RotationMatrix();
	vaneRot->rotateZ(CLHEP::pi / 2.);
	G4VSolid* spider = new G4UnionSolid("spiderVanes", spiderVane, spiderVane, vaneRot, G4ThreeVector());
	// Create secondary mirror fixation plate.
	G4VSolid* fixationPlate = new G4Tubs("fixationPlate", 0, rF, spiderLength, 0, 360. * CLHEP::deg);
	G4VSolid* fixation = new G4UnionSolid("spider", spider, fixationPlate, NULL,
			G4ThreeVector(0, -delta, -spiderLength));
	// Place the fixation.e
	G4LogicalVolume* fixationLV = new G4LogicalVolume(fixation, CoreMaterialFactory::getInstance()->getDummyMatter(),
			"fixationLV", 0, 0, 0);
	// Set colors for printing.
#ifdef G4UI_USE_QT
	tubeLV->SetVisAttributes(ColorStore::getInstance()->getBlack());
	basePlateLV->SetVisAttributes(ColorStore::getInstance()->getBlack());
	fixationLV->SetVisAttributes(ColorStore::getInstance()->getBlack());
#endif
	new G4PVPlacement(NULL, G4ThreeVector(0, 0, tubusLength / 2. - spiderLength / 2. - spiderShift), fixationLV,
			"fixationPV", bodyLV, false, 0, false);
	return bodyLV;
}

void NewtonTelescope::applyMaterialProperties(G4LogicalVolume* coneLogical) {
	// Set the aluminium skin and its properties.
	G4OpticalSurface* opticalSurface = new G4OpticalSurface("Al-layer");
	/* G4LogicalSkinSurface* skinSurface =*/
	new G4LogicalSkinSurface("Al-skin", coneLogical, opticalSurface);
	opticalSurface->SetType(dielectric_metal);
	opticalSurface->SetFinish(polished);
}

void NewtonTelescope::setEyepieceLength(G4double _eyepieceLength) {
	eyepieceLength = _eyepieceLength;
}

void NewtonTelescope::setEyepieceRadius(G4double _eyepieceRadius) {
	eyepieceRadius = _eyepieceRadius;
}

void NewtonTelescope::setMirrorThickness(G4double _mirrorThickness) {
	mirrorThickness = _mirrorThickness;
}

void NewtonTelescope::setTubusLength(G4double _tubusLength) {
	tubusLength = _tubusLength;
}

void NewtonTelescope::setTubusRadius(G4double _tubusRadius) {
	tubusRadius = _tubusRadius;
}

void NewtonTelescope::setTubusThickness(G4double _tubusThickness) {
	tubusThickness = _tubusThickness;
}

G4ThreeVector NewtonTelescope::getEyepiecePos() const {
	return eyepiecePos;
}

G4RotationMatrix* NewtonTelescope::getEyepieceRot() const {
	return eyepieceRot;
}

G4ThreeVector NewtonTelescope::getSecondaryMirrorPos() const {
	return secondaryMirrorPos;
}

G4double NewtonTelescope::getA() const {
	return a;
}

G4double NewtonTelescope::getB() const {
	return b;
}

G4double NewtonTelescope::getDelta() const {
	return delta;
}

G4double NewtonTelescope::getEyepieceLength() const {
	return eyepieceLength;
}

G4double NewtonTelescope::getEyepieceRadius() const {
	return eyepieceRadius;
}

G4double NewtonTelescope::getF() const {
	return f;
}

G4double NewtonTelescope::getRF() const {
	return rF;
}

G4double NewtonTelescope::getRH() const {
	return rH;
}

G4double NewtonTelescope::getMirrorLength() const {
	return mirrorLength;
}

G4double NewtonTelescope::getMirrorThickness() const {
	return mirrorThickness;
}

G4double NewtonTelescope::getS() const {
	return s;
}

G4double NewtonTelescope::getTubusLength() const {
	return tubusLength;
}

G4double NewtonTelescope::getTubusRadius() const {
	return tubusRadius;
}

G4double NewtonTelescope::getTubusThickness() const {
	return tubusThickness;
}
