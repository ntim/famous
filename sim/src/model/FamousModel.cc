/*
 * FamousModel.cc
 *
 * @date Aug 6, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/FamousModel.hh"

#include <model/G4SipmModelFactory.hh>

#include "model/WinstonConeModelFactory.hh"

FamousModel::FamousModel(size_t _numberOfPixels, bool _symmetric) :
		numberOfPixels(_numberOfPixels), symmetric(_symmetric) {
	double r = 502.1 * CLHEP::mm / 2.0;
	double nf = 1.0;
	// Create fresnel lens model.
	double f = 2. * r * nf;
	// Add focal number correction to match measured best focal distance at 513.2 mm.
	fresnelLensModel = new FresnelLensModel(r, f * 0.9925);
	fresnelLensModel->setDz(9.55575 * CLHEP::mm); // Total lens to focal plane distance 507.89 mm
	fresnelLensModel->setNumberOfGrooves(10);
	fresnelLensModel->setThickness(2.5 * CLHEP::mm);
	fresnelLensModel->setDraftAngle(3 * CLHEP::deg);
	fresnelLensModel->setDraftAngleFactor(0.0473 * CLHEP::deg / CLHEP::mm);
	// Find Mppc model.
	sipmModel = G4SipmModelFactory::getInstance()->createHamamatsuS12573100X();
	// Derive Winston cone entrance radius.
	double r1 = sqrt(r * r + f * f) * sipmModel->getPitch() / 2. / r; // 6.71 mm
	wicoModel = WinstonConeModelFactory::createRound(r1, sipmModel->getPitch() / 2.);
	wicoModel->setThickness(0.69 * CLHEP::mm);
	wicoModel->setSurfaceSigmaAlpha(0.45 * CLHEP::deg);
	wicoModel->setMaterial(CoreMaterialFactory::getInstance()->getAluminum());
	wicoModel->setOuterFaceStraight(true);
	//
	ug11FilterThickness = 1.0 * CLHEP::mm;
	//
	curvatureOfField = 0; // Optimum: 5.04e-3 / CLHEP::mm;
}

double FamousModel::getFieldOfView() const {
	const double size = (wicoModel->getR1() + wicoModel->getThickness()) * 2.0 / sqrt(3.0);
	return 2.0 * atan(sqrt(numberOfPixels) * size / (fresnelLensModel->getF() + fresnelLensModel->getDz()));
}

double FamousModel::getPixelFieldOfView() const {
	return 2.0 * atan(wicoModel->getR1() / (fresnelLensModel->getF() + fresnelLensModel->getDz()));
}

std::vector<hex::CartesianCoordinate> FamousModel::getPixelCoordinates() const {
	const double size = (wicoModel->getR1() + wicoModel->getThickness()) * 2.0 / sqrt(3.0);
	// For an non-symmetric camera, we have no central pixel.
	hex::Point offset = hex::Point::fromAxial( { 0, 0.5 }, size);
	if (symmetric) {
		offset = hex::Point::fromAxial( { 0, 0 }, size);
	}
	std::vector<hex::Point> points = hex::grid(getNumberOfPixels(), size, offset);
	std::vector<hex::CartesianCoordinate> coords;
	for (std::vector<hex::Point>::iterator it = points.begin(); it != points.end(); it++) {
		coords.push_back((*it).toCartesian(offset));
	}
	return coords;
}

size_t FamousModel::getNumberOfPixels() const {
	return numberOfPixels;
}

FresnelLensModel* FamousModel::getFresnelLensModel() const {
	return fresnelLensModel;
}

G4SipmModel* FamousModel::getSipmModel() const {
	return sipmModel;
}

WinstonConeModel* FamousModel::getWicoModel() const {
	return wicoModel;
}

double FamousModel::getUg11FilterThickness() const {
	return ug11FilterThickness;
}

double FamousModel::getCurvatureOfField() const {
	return curvatureOfField;
}

void FamousModel::setCurvatureOfField(double _curvatureOfField) {
	curvatureOfField = _curvatureOfField;
}

void FamousModel::setUg11FilterThickness(double _ug11FilterThickness) {
	ug11FilterThickness = _ug11FilterThickness;
}

bool FamousModel::isSymmetric() const {
	return symmetric;
}
