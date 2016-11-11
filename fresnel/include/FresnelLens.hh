/**
 * FresnelLens.hh
 *
 * @date 19.03.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELLENSE_HH_
#define FRESNELLENSE_HH_

#include <CLHEP/Units/PhysicalConstants.h>

#include "Placeable.hh"
#include "model/FresnelLensModel.hh"

/**
 * Implementation of a plan-convex fresnel lense based on the Geant4 Example "air_shower" coded by
 * Bernado Tome (LIP) for the ULTRA experiment.
 */
class FresnelLens: public Placeable {
private:
	FresnelLensModel* model;
	double rMin;
	double startPhi;
	double dPhi;

private:
	G4LogicalVolume* createLensMother();
	G4LogicalVolume* createGroovePrototype();
	G4LogicalVolume* createAndPlaceGroovesMother(G4LogicalVolume* motherLv);
	G4LogicalVolume* createAndPlaceLensSupport(G4LogicalVolume* motherLv);

public:
	FresnelLens(FresnelLensModel* model, double rMin = 0.0, double startPhi = 0.0, double dPhi = CLHEP::twopi);

	virtual G4LogicalVolume* build();

	double getRMax() const;
	double getRMin() const;
	void setRMin(double rMin);
	double getDPhi() const;
	void setDPhi(double dPhi);
	double getStartPhi() const;
	void setStartPhi(double startPhi);

};

#endif /* FRESNELLENSE_HH_ */
