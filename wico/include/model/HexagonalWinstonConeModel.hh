/*
 * HexagonalWinstonConeModel.hh
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HEXAGONALWINSTONCONEMODEL_HH_
#define HEXAGONALWINSTONCONEMODEL_HH_

#include "WinstonConeModel.hh"

class HexagonalWinstonConeModel: public WinstonConeModel {
public:
	HexagonalWinstonConeModel(double r1, double r2);
	virtual ~HexagonalWinstonConeModel();

	virtual int getNumberOfSides() const;
};

#endif /* HEXAGONALWINSTONCONEMODEL_HH_ */
