/*
 * FamousActionInitialization.hh
 *
 * @date Aug 19, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSACTIONINITIALIZATION_HH_
#define FAMOUSACTIONINITIALIZATION_HH_

#include "CoreActionInitialization.hh"

class CoreRunAction;

class FamousActionInitialization: public CoreActionInitialization {
public:
	FamousActionInitialization(std::string path, std::string prefix = "");

	virtual G4VUserPrimaryGeneratorAction* createPrimaryGeneratorAction() const;
	virtual CoreRunAction* createRunAction() const;
};

#endif /* FAMOUSACTIONINITIALIZATION_HH_ */
