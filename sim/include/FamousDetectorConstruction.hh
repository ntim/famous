/*
 * FamousDetectorConstruction.hh
 *
 * @date Jul 29, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSDETECTORCONSTRUCTION_HH_
#define FAMOUSDETECTORCONSTRUCTION_HH_

#include <G4VUserDetectorConstruction.hh>

#include <G4LogicalVolume.hh>

#include "CoreSensitiveDetector.hh"
#include "model/FamousModel.hh"

class FamousDetectorConstruction: public G4VUserDetectorConstruction {
private:
	FamousModel* model;
	CoreSensitiveDetector* wicoBackSd;
	CoreSensitiveDetector* focalPlaneSd;
	CoreSensitiveDetector* fresnelLensBackSd;

	G4LogicalVolume* constructPixel(G4int copyNo);
	void constructCamera(G4VPhysicalVolume* pv);
	void constructTube(G4VPhysicalVolume* pv);
	void constructFresnelLens(G4VPhysicalVolume* pv);

public:
	FamousDetectorConstruction(FamousModel* model);

	virtual G4VPhysicalVolume* Construct();

	FamousModel* getModel() const;
};

#endif /* FAMOUSDETECTORCONSTRUCTION_HH_ */
