/*
 * FastDetectorConstruction.hh
 *
 * @date Feb 6, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FASTDETECTORCONSTRUCTION_HH_
#define FASTDETECTORCONSTRUCTION_HH_

#include <G4VUserDetectorConstruction.hh>

#include "model/FresnelLensModel.hh"

class FastDetectorConstruction: public G4VUserDetectorConstruction {
private:
	FresnelLensModel* model;
	bool createWinstonCone;
	double hemisphereRadius;
	double hemisphereDiameter;
	double winstonConeDiameter;
	bool reversedLens;

	G4Material* createNBK7();
public:
	FastDetectorConstruction(FresnelLensModel* model, bool createWinstonCone = false, bool reversedLens = false);

	virtual G4VPhysicalVolume* Construct();

	FresnelLensModel* getModel() const;
	void setModel(FresnelLensModel* model);

};

#endif /* FASTDETECTORCONSTRUCTION_HH_ */
