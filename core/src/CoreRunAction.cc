/*
 * CoreRunAction.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CoreRunAction.hh"

#include "CorePersistencyHandler.hh"
#include "CoreRootPersistVisitor.hh"

#include "GeneralParticleSourceMessenger.hh"

CoreRunAction::CoreRunAction(std::string _filename, CoreRootPersistVisitor* visitor) :
		G4UserRunAction(), filename(_filename) {
	persistencyHandler = new CorePersistencyHandler(visitor);
}

CoreRunAction::~CoreRunAction() {
	delete persistencyHandler;
}

void CoreRunAction::BeginOfRunAction(const G4Run*) {
	timer.Start();
	persistencyHandler->open(filename);
	// Persist run settings.
	persistencyHandler->persist(GeneralParticleSourceMessenger::getInstance());
}

void CoreRunAction::EndOfRunAction(const G4Run*) {
	persistencyHandler->close();
	timer.Stop();
	std::cout << "RunAction::EndOfRunAction(): time (" << timer << ")." << std::endl;
}

std::string CoreRunAction::getFilename() const {
	return filename;
}

CorePersistencyHandler* CoreRunAction::getPersistencyHandler() const {
	return persistencyHandler;
}

void CoreRunAction::setPersistencyHandler(CorePersistencyHandler* _persistencyHandler) {
	persistencyHandler = _persistencyHandler;
}
