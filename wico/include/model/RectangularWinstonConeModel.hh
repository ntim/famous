/**
 * RectangularWinstonConeModel.hh
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef RECTANGULARWINSTONCONEMODEL_HH_
#define RECTANGULARWINSTONCONEMODEL_HH_

#include "WinstonConeModel.hh"

class RectangularWinstonConeModel: public WinstonConeModel {
public:
	RectangularWinstonConeModel(double r1, double r2) ;
	virtual ~RectangularWinstonConeModel();

	virtual int getNumberOfSides() const;
};

#endif /* RECTANGULARWINSTONCONEMODEL_HH_ */
