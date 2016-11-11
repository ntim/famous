/*
 * WicoDetectorConstruction.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef WICODETECTORCONSTRUCTION_HH_
#define WICODETECTORCONSTRUCTION_HH_

#include <G4VUserDetectorConstruction.hh>

#include <G4LogicalVolume.hh>

#include <model/G4SipmModel.hh>

#include "model/WinstonConeModel.hh"

class WicoDetectorConstruction: public G4VUserDetectorConstruction {
private:
	WinstonConeModel* wicoModel;
	G4SipmModel* sipmModel;

public:
	WicoDetectorConstruction(WinstonConeModel* wicoModel, G4SipmModel* sipmModel = 0);

	virtual G4VPhysicalVolume* Construct();

	WinstonConeModel* getWicoModel() const;
	G4SipmModel* getSipmModel() const;
};

#endif /* WICODETECTORCONSTRUCTION_HH_ */
