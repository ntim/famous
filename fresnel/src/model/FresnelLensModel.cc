/**
 * FresnelLensModel.cc
 *
 * @date Mar 7, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/FresnelLensModel.hh"

#include <cmath>

FresnelLensModel::FresnelLensModel(double _r, double _f) :
		r(_r), f(_f), dz(0), draftAngle(0), draftAngleFactor(0) {
	k = -1;
	numberOfGrooves = 2.0 / CLHEP::mm;
	material = CoreMaterialFactory::getInstance()->getAcrylic();
	// Reference wavelength for the refractive index calculation.
	double referenceEnergy = CLHEP::h_Planck * CLHEP::c_light / (546. * CLHEP::nm);
	// Get materials.
	G4Material* air = CoreMaterialFactory::getInstance()->getAir();
	// Resolve refractive indices.
	rIndex = material->GetMaterialPropertiesTable()->GetProperty("RINDEX")->Value(referenceEnergy);
	rIndexAir = air->GetMaterialPropertiesTable()->GetProperty("RINDEX")->Value(referenceEnergy);
	// Calculate thickness.
	thickness = 2.5 * CLHEP::mm;
}

double FresnelLensModel::getC() const {
	// Simple calculation using lensmaker's equation.
	// return 1.0 / (f * (rIndex - rIndexAir));
	// Complex calculation respecting the thickness
	return 1.0 / ((f + rIndexAir / rIndex * getTotalThickness()) * (rIndex - rIndexAir) / rIndexAir);
}

double FresnelLensModel::getHeighestGrooveThickness() const {
	return getSagitta(r) - getSagitta(r - 1.0 / numberOfGrooves);
}

double FresnelLensModel::getTotalThickness() const {
	return thickness;
}

int FresnelLensModel::getTotalNumberOfGrooves() const {
	return ceil(r * numberOfGrooves);
}

double FresnelLensModel::getSagitta(double ri) const {
	const double c = getC();
	double sumA = 0;
	int order = 2;
	for (std::vector<double>::const_iterator iter = a.begin(); iter != a.end(); ++iter) {
		sumA += (*iter) * pow(ri, order);
		order *= 2;
	}
	return c * ri * ri / (1.0 + sqrt(1.0 - (1.0 + k) * c * c * ri * ri)) + sumA;
}

double FresnelLensModel::getR() const {
	return r;
}

void FresnelLensModel::setR(double _r) {
	r = _r;
}

double FresnelLensModel::getF() const {
	return f;
}

void FresnelLensModel::setF(double _f) {
	f = _f;
}

double FresnelLensModel::getNumberOfGrooves() const {
	return numberOfGrooves;
}

void FresnelLensModel::setNumberOfGrooves(double _numberOfGrooves) {
	numberOfGrooves = _numberOfGrooves;
}

double FresnelLensModel::getK() const {
	return k;
}

void FresnelLensModel::setK(double _k) {
	k = _k;
}

double FresnelLensModel::getRIndex() const {
	return rIndex;
}

void FresnelLensModel::setRIndex(double _rIndex) {
	rIndex = _rIndex;
}

double FresnelLensModel::getRIndexAir() const {
	return rIndexAir;
}

std::vector<double>& FresnelLensModel::getA() {
	return a;
}

double FresnelLensModel::getThickness() const {
	return thickness - getHeighestGrooveThickness();
}

void FresnelLensModel::setThickness(double _thickness) {
	thickness = _thickness;
}

double FresnelLensModel::getDz() const {
	return dz;
}

void FresnelLensModel::setDz(double _dz) {
	dz = _dz;
}

G4Material* FresnelLensModel::getMaterial() const {
	return material;
}

void FresnelLensModel::setMaterial(G4Material* _material) {
	material = _material;
}

double FresnelLensModel::getDraftAngle() const {
	return draftAngle;
}

void FresnelLensModel::setDraftAngle(double _draftAngle) {
	draftAngle = _draftAngle;
}

double FresnelLensModel::getDraftAngleFactor() const {
	return draftAngleFactor;
}

void FresnelLensModel::setDraftAngleFactor(double _draftAngleFactor) {
	draftAngleFactor = _draftAngleFactor;
}
