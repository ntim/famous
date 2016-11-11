/*
 * SolidWinstonCone.hh
 *
 * @date 04.04.2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef SOLIDWINSTONCONE_HH_
#define SOLIDWINSTONCONE_HH_

#include "WinstonCone.hh"

/**
 * Constructs a solid winston cone filled with UV transparent acryl.
 */
class SolidWinstonCone: public WinstonCone {

protected:
	virtual void calculateZPlanes(G4double* z, G4double* rI, G4double* rO, const int n);

public:
	SolidWinstonCone(WinstonConeModel* model);
	virtual ~SolidWinstonCone();

	virtual G4LogicalVolume* build();
};

#endif /* SOLIDWINSTONCONE_HH_ */
