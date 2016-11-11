/**
 * ParabolicConcentratorModel.cc
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/ParabolicConcentratorModel.hh"

ParabolicConcentratorModel::ParabolicConcentratorModel(double _r1, double _r2) :
		WinstonConeModel(_r1, _r2) {
	//
}

ParabolicConcentratorModel::~ParabolicConcentratorModel() {
	//
}

double ParabolicConcentratorModel::parabola(double x) const {
	return 1. / (4. * getF()) * x * x;
}

double ParabolicConcentratorModel::getF() const {
	return 1. / 4. * getR2();
}

double ParabolicConcentratorModel::getLength() const {
	return parabola(getR1()) - parabola(getR2());
}

double ParabolicConcentratorModel::getR(double) const {
	// FIXME
	return 0;
}

double ParabolicConcentratorModel::getThetaMax() const {
	// FIXME
	return 0;
}

double ParabolicConcentratorModel::getZ(double) const {
	// FIXME
	return 0;
}
