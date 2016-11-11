/*
 * WinstonCone.hh
 *
 * @date Dec 2, 2010
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef WINSTONCONE_HH_
#define WINSTONCONE_HH_

#include <G4VSolid.hh>

#include "Placeable.hh"
#include "model/WinstonConeModel.hh"

class WinstonCone: public Placeable {
private:
	WinstonConeModel* model;

protected:
	void calculateZPlanes(G4double* z, G4double* rI, G4double* rO, const int n);
	G4VSolid* createSolid();

public:
	WinstonCone(WinstonConeModel* model);
	virtual ~WinstonCone();

	virtual G4LogicalVolume* build();

	WinstonConeModel* getModel() const;
};

#endif /* WINSTONCONE_HH_ */
