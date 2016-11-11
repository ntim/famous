/*
 * CoreActionInitialization.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CoreActionInitialization.hh"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <G4SipmUiMessenger.hh>
#include <persistency/PersistencyHandlerMessenger.hh>

#include "GeneralParticleSourceMessenger.hh"
#include "GeneralParticleSource.hh"

CoreActionInitialization::CoreActionInitialization(std::string path, std::string basename) {
	if (boost::filesystem::is_directory(path)) {
		std::string time = boost::posix_time::to_iso_string(boost::posix_time::microsec_clock::local_time());
		filename = boost::str(boost::format("%s/%s_%s") % path % basename % time);
	} else {
		filename = path;
	}
}

void CoreActionInitialization::Build() const {
	// Load messengers.
	GeneralParticleSourceMessenger::getInstance();
	PersistencyHandlerMessenger::getInstance();
	G4SipmUiMessenger::getInstance();
	// Set user actions.
	SetUserAction(createPrimaryGeneratorAction());
	SetUserAction(createRunAction());
	SetUserAction(createEventAction());
}

G4VUserPrimaryGeneratorAction* CoreActionInitialization::createPrimaryGeneratorAction() const {
	return new GeneralParticleSource();
}

CoreRunAction* CoreActionInitialization::createRunAction() const {
	return new CoreRunAction(getFilename());
}

CoreEventAction* CoreActionInitialization::createEventAction() const {
	return new CoreEventAction();
}

std::string CoreActionInitialization::getFilename() const {
	return filename;
}
