/**
 * WinstonConeModel.cc
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/WinstonConeModel.hh"

#include <CLHEP/Units/PhysicalConstants.h>

#include "CoreMaterialFactory.hh"

WinstonConeModel::WinstonConeModel(double _r1, double _r2) :
		r1(_r1), r2(_r2), solid(false), outerFaceStraight(true) {
	thickness = 0.1 * CLHEP::mm;
	surfaceSigmaAlpha = 0. * CLHEP::deg;
	material = CoreMaterialFactory::getInstance()->getAluminum();
}

WinstonConeModel::~WinstonConeModel() {
	//
}

double WinstonConeModel::getLength() const {
	return (r1 + r2) / tan(getThetaMax());
}

double WinstonConeModel::getThetaMax() const {
	return asin(r2 / r1);
}

double WinstonConeModel::getF() const {
	return r2 * (1. + sin(getThetaMax()));
}

void WinstonConeModel::check(const double theta) const {
	if (theta > CLHEP::halfpi) {
		std::cerr << "WinstonConeModel::check(): theta=" << theta / CLHEP::deg << " deg greater than pi/2."
				<< std::endl;
		throw 1;
	}
	// Double precision safe.
	if (theta - getThetaMax() < -1e-12) {
		std::cerr << "WinstonConeModel::check(): theta=" << theta / CLHEP::deg
				<< " deg smaller than theta_{max}, #Delta=" << theta - getThetaMax() << "." << std::endl;
		throw 1;
	}
}

double WinstonConeModel::getV(double theta) const {
	check(theta);
	return 2. * getF() / (1. - cos(theta + getThetaMax()));
}

double WinstonConeModel::getR(double theta) const {
	return getV(theta) * sin(theta) - r2;
}

double WinstonConeModel::getZ(double theta) const {
	return getV(theta) * cos(theta) - getLength() / 2.;
}

int WinstonConeModel::getNumberOfSides() const {
	return 0;
}

double WinstonConeModel::getR1() const {
	return r1;
}

void WinstonConeModel::setR1(double _r1) {
	r1 = _r1;
}

double WinstonConeModel::getR2() const {
	return r2;
}

void WinstonConeModel::setR2(double _r2) {
	r2 = _r2;
}

double WinstonConeModel::getThickness() const {
	return thickness;
}

void WinstonConeModel::setThickness(double _thickness) {
	thickness = _thickness;
}

double WinstonConeModel::getSurfaceSigmaAlpha() const {
	return surfaceSigmaAlpha;
}

void WinstonConeModel::setSurfaceSigmaAlpha(double _surfaceSigmaAlpha) {
	surfaceSigmaAlpha = _surfaceSigmaAlpha;
}

bool WinstonConeModel::isSolid() const {
	return solid;
}

void WinstonConeModel::setSolid(bool _solid) {
	solid = _solid;
}

G4Material* WinstonConeModel::getMaterial() const {
	return material;
}

void WinstonConeModel::setMaterial(G4Material* _material) {
	material = _material;
}

bool WinstonConeModel::isOuterFaceStraight() const {
	return outerFaceStraight;
}

void WinstonConeModel::setOuterFaceStraight(bool _outerFaceStraight) {
	outerFaceStraight = _outerFaceStraight;
}
