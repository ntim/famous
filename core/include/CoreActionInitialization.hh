/*
 * CoreActionInitialization.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COREACTIONINITIALIZATION_HH_
#define COREACTIONINITIALIZATION_HH_

#include <string>

#include <G4VUserActionInitialization.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

#include "CoreRunAction.hh"
#include "CoreEventAction.hh"

class CoreActionInitialization: public G4VUserActionInitialization {
private:
	std::string filename;

public:
	CoreActionInitialization(std::string path, std::string basename = "famous");

	virtual void Build() const;

	virtual G4VUserPrimaryGeneratorAction* createPrimaryGeneratorAction() const;
	virtual CoreRunAction* createRunAction() const;
	virtual CoreEventAction* createEventAction() const;

	std::string getFilename() const;
};

#endif /* COREACTIONINITIALIZATION_HH_ */
