/*
 * FresnelDetectorConstruction.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELDETECTORCONSTRUCTION_HH_
#define FRESNELDETECTORCONSTRUCTION_HH_

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>

#include "model/FresnelLensModel.hh"

class FresnelDetectorConstruction: public G4VUserDetectorConstruction {
private:
	FresnelLensModel* model;

public:
	FresnelDetectorConstruction(FresnelLensModel* model);

	virtual G4VPhysicalVolume* Construct();

	FresnelLensModel* getModel() const;
};

#endif /* FRESNELDETECTORCONSTRUCTION_HH_ */
