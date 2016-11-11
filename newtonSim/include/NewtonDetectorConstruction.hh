/*
 * NewtonDetectorConstruction.hh
 *
 * @date Aug 1, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef NEWTONDETECTORCONSTRUCTION_HH_
#define NEWTONDETECTORCONSTRUCTION_HH_

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>

#include "model/NewtonTelescope.hh"

class NewtonDetectorConstruction: public G4VUserDetectorConstruction {
private:
	NewtonTelescope* telescope;

public:
	NewtonDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
};

#endif /* NEWTONDETECTORCONSTRUCTION_HH_ */
