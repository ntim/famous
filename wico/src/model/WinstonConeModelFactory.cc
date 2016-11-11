/**
 * WinstonConeModelFactory.cc
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/WinstonConeModelFactory.hh"

#include "model/RectangularWinstonConeModel.hh"
#include "model/HexagonalWinstonConeModel.hh"
#include "model/ParabolicConcentratorModel.hh"

WinstonConeModelFactory::WinstonConeModelFactory() {
	//
}

WinstonConeModel* WinstonConeModelFactory::createRound(double r1, double r2) {
	return new WinstonConeModel(r1, r2);
}

WinstonConeModel* WinstonConeModelFactory::createRectangular(double r1, double r2) {
	return new RectangularWinstonConeModel(r1, r2);
}

WinstonConeModel* WinstonConeModelFactory::createHexagonal(double r1, double r2) {
	return new HexagonalWinstonConeModel(r1, r2);
}

WinstonConeModel* WinstonConeModelFactory::createParabolic(double r1, double r2) {
	return new ParabolicConcentratorModel(r1, r2);
}
