/*
 * NewtonTelescope.hh
 *
 * @date 27.01.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef NEWTONTELESCOPE_HH_
#define NEWTONTELESCOPE_HH_

#include "Placeable.hh"

#include <globals.hh>
#include <G4VPhysicalVolume.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>

/**
 * This class implements a Newton refractor with a planar, elliptical secondary mirror.
 */
class NewtonTelescope: public Placeable {
private:
	// Radius of the main mirror.
	G4double rH;
	// Radius of the secondary mirror.
	G4double rF;
	// Focal length.
	G4double f;
	// Distance of the secondary mirror to the main.
	G4double s;
	// Big half axis of the secondary mirror.
	G4double a;
	// Small half axis of the secondary mirror.
	G4double b;
	// Placement of the secondary mirror.
	G4double delta;
	// Tube parameters.
	G4double tubusLength;
	G4double tubusRadius;
	G4double tubusThickness;
	// Eyepiece parameters.
	G4double eyepieceRadius;
	G4double eyepieceLength;
	G4ThreeVector eyepiecePos;
	G4RotationMatrix* eyepieceRot;
	// Main mirror solid parameters.
	G4double mirrorLength;
	G4double mirrorThickness;
	G4double secondaryMirrorThickness;
	G4ThreeVector secondaryMirrorPos;
	// Spider settings.
	G4double spiderThickness;
	G4double spiderLength;
	G4double spiderShift;

	/**
	 * Initializes all parameters using rH, rF and f.
	 */
	void initParameters();
	void applyMaterialProperties(G4LogicalVolume* coneLogical);

protected:
	void setEyepieceLength(G4double eyepieceLength);
	void setEyepieceRadius(G4double eyepieceRadius);
	void setMirrorThickness(G4double mirrorThickness);
	void setTubusLength(G4double tubusLength);
	void setTubusRadius(G4double tubusRadius);
	void setTubusThickness(G4double tubusThickness);

public:
	NewtonTelescope(G4String name, G4ThreeVector placement, G4RotationMatrix* rotation, G4double rH, G4double rF,
			G4double f, G4double tubusLength, G4double eyepieceRadius, G4double eyepieceLength);

	virtual G4LogicalVolume* build();

	G4ThreeVector getEyepiecePos() const;
	G4RotationMatrix *getEyepieceRot() const;
	G4ThreeVector getSecondaryMirrorPos() const;
	G4double getA() const;
	G4double getB() const;
	G4double getDelta() const;
	G4double getEyepieceLength() const;
	G4double getEyepieceRadius() const;
	G4double getF() const;
	G4double getRF() const;
	G4double getRH() const;
	G4double getMirrorLength() const;
	G4double getMirrorThickness() const;
	G4double getS() const;
	G4double getTubusLength() const;
	G4double getTubusRadius() const;
	G4double getTubusThickness() const;

};

#endif /* NEWTONTELESCOPE_HH_ */
