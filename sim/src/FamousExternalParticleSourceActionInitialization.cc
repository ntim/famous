/*
 * FamousExternalParticleSourceActionInitialization.cc
 *
 * @date 17.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FamousExternalParticleSourceActionInitialization.hh"

#include <G4RunManager.hh>

#include "FamousExternalParticleSource.hh"
#include "FamousDetectorConstruction.hh"
#include "FamousRunAction.hh"

FamousExternalParticleSourceActionInitialization::FamousExternalParticleSourceActionInitialization(std::string _input,
		std::string path, std::string prefix) :
		CoreActionInitialization(path, "famous" + prefix), input(_input) {
	//
}
G4VUserPrimaryGeneratorAction* FamousExternalParticleSourceActionInitialization::createPrimaryGeneratorAction() const {
	// Set telescope geomentry.
	FamousModel* model =
			((FamousDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->getModel();
	G4ThreeVector position = G4ThreeVector(0, 0,
			model->getFresnelLensModel()->getF() + model->getFresnelLensModel()->getDz()
					+ model->getFresnelLensModel()->getTotalThickness());
	return new FamousExternalParticleSource(input, position);
}

CoreRunAction* FamousExternalParticleSourceActionInitialization::createRunAction() const {
	return new FamousRunAction(getFilename());
}
