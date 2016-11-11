/**
 * WinstonConeModelFactory.hh
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef WINSTONCONEMODELFACTORY_HH_
#define WINSTONCONEMODELFACTORY_HH_

#include "WinstonConeModel.hh"

class WinstonConeModelFactory {
private:
	WinstonConeModelFactory();

public:
	static WinstonConeModel* createRound(double r1, double r2);
	static WinstonConeModel* createRectangular(double r1, double r2);
	static WinstonConeModel* createHexagonal(double r1, double r2);
	static WinstonConeModel* createParabolic(double r1, double r2);

};

#endif /* WINSTONCONEMODELFACTORY_HH_ */
