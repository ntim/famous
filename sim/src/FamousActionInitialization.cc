/*
 * FamousActionInitialization.cc
 *
 * @date Aug 19, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <FamousActionInitialization.hh>

#include <G4RunManager.hh>

#include "FamousDetectorConstruction.hh"
#include "FamousRunAction.hh"
#include "model/FamousModel.hh"
#include "GeneralParticleSource.hh"
#include "GeneralParticleSourceMessenger.hh"

FamousActionInitialization::FamousActionInitialization(std::string path, std::string prefix) :
		CoreActionInitialization(path, "famous" + prefix) {
	//
}

G4VUserPrimaryGeneratorAction* FamousActionInitialization::createPrimaryGeneratorAction() const {
	GeneralParticleSource* gps = new GeneralParticleSource();
	// Set telescope geomentry.
	FamousModel* model =
			((FamousDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->getModel();
	GeneralParticleSourceMessenger* messenger = GeneralParticleSourceMessenger::getInstance();
	messenger->setPlaneShape("circle");
	messenger->setPlaneRMin(0);
	messenger->setPlaneRMax(model->getFresnelLensModel()->getR());
	messenger->setAnglePhiMin(90.0 * CLHEP::deg);
	messenger->setAnglePhiMax(90.0 * CLHEP::deg);
	messenger->setAngleThetaMin(0.0);
	messenger->setAngleThetaMax(0.0);
	messenger->setPlanePos(
			G4ThreeVector(0, 0,
					model->getFresnelLensModel()->getF() + model->getFresnelLensModel()->getDz()
							+ model->getFresnelLensModel()->getTotalThickness()));
	//
	return gps;
}

CoreRunAction* FamousActionInitialization::createRunAction() const {
	return new FamousRunAction(getFilename());
}
