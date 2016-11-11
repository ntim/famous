/*
 * WicoActionInitialization.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "WicoActionInitialization.hh"

#include <G4RunManager.hh>
#include <G4GeometryTolerance.hh>

#include "WicoDetectorConstruction.hh"
#include "GeneralParticleSource.hh"
#include "GeneralParticleSourceMessenger.hh"
#include "model/HexagonalWinstonConeModel.hh"
#include "model/RectangularWinstonConeModel.hh"

WicoActionInitialization::WicoActionInitialization(std::string path) :
		CoreActionInitialization(path, "wico") {
	//
}

G4VUserPrimaryGeneratorAction* WicoActionInitialization::createPrimaryGeneratorAction() const {
	GeneralParticleSource* ps = new GeneralParticleSource();
	// Retreive detector construction.
	const WicoDetectorConstruction* detector =
			(const WicoDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	WinstonConeModel* model = detector->getWicoModel();
	// Set plane extent.
	GeneralParticleSourceMessenger::getInstance()->setPlaneA(model->getR1());
	GeneralParticleSourceMessenger::getInstance()->setPlaneB(
			detector->getWicoModel()->getR1());
	GeneralParticleSourceMessenger::getInstance()->setPlaneRMax(
			detector->getWicoModel()->getR1());
	GeneralParticleSourceMessenger::getInstance()->setPlaneShape("circle");
	// Set plane type.
	if (dynamic_cast<HexagonalWinstonConeModel*>(model)) {
		GeneralParticleSourceMessenger::getInstance()->setPlaneShape("hex");
	}
	if (dynamic_cast<RectangularWinstonConeModel*>(model)) {
		GeneralParticleSourceMessenger::getInstance()->setPlaneShape("rect");
	}
	// Set plane position.
	G4ThreeVector pos(0, 0, detector->getWicoModel()->getLength() / 2.);
	if (model->isSolid()) {
		pos += G4ThreeVector(0, 0,
				G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()
						* 10.);
	}
	GeneralParticleSourceMessenger::getInstance()->setPlanePos(pos);
	// Set maximum incidence angle.
	GeneralParticleSourceMessenger::getInstance()->setAngleThetaMax(
			detector->getWicoModel()->getThetaMax());
	//
	return ps;
}
