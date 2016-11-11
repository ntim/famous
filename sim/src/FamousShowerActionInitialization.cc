/*
 * FamousShowerActionInitialization.cc
 *
 * @date Aug 6, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FamousShowerActionInitialization.hh"

#include <G4RunManager.hh>

#include "FamousDetectorConstruction.hh"
#include "model/FamousModel.hh"

FamousShowerActionInitialization::FamousShowerActionInitialization(std::string path, std::string _input,
		int _inputIndex, bool _addBg, unsigned int _bgThin, std::string prefix) :
		CoreActionInitialization(path, "famous" + prefix), input(_input), inputIndex(_inputIndex), addBg(_addBg), bgThin(
				_bgThin) {
}

G4VUserPrimaryGeneratorAction* FamousShowerActionInitialization::createPrimaryGeneratorAction() const {
	ShowerLightParticleSource* source = new ShowerLightParticleSource(input);
	source->setAddNightSkyPhotons(addBg);
	source->setNightSkyPhotonsThinning(bgThin);
	// Set telescope geomentry.
	FamousModel* model =
			((FamousDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->getModel();
	source->setPosition(
			G4ThreeVector(0, 0,
					model->getFresnelLensModel()->getF() + model->getFresnelLensModel()->getDz()
							+ model->getFresnelLensModel()->getTotalThickness()));
	source->setRMax(model->getFresnelLensModel()->getR());
	source->setThetaMax(model->getFieldOfView() / 2. + 3 * CLHEP::deg);
	source->initialize(inputIndex);
	//
	return source;
}
