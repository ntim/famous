/*
 * HexToRoundWinstonCone.hh
 *
 * @date Aug 26, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HEXTOROUNDWINSTONCONE_HH_
#define HEXTOROUNDWINSTONCONE_HH_

#include <WinstonCone.hh>
#include <G4LogicalVolume.hh>

#include "model/HexagonalWinstonConeModel.hh"

class HexToRoundWinstonCone: public WinstonCone {
public:
	HexToRoundWinstonCone(HexagonalWinstonConeModel* model);

	virtual G4LogicalVolume* build();
};

#endif /* HEXTOROUNDWINSTONCONE_HH_ */
