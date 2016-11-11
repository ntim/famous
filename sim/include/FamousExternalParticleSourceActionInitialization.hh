/*
 * FamousExternalParticleSourceActionInitialization.hh
 *
 * @date 17.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSEXTERNALPARTICLESOURCEACTIONINITIALIZATION_HH_
#define FAMOUSEXTERNALPARTICLESOURCEACTIONINITIALIZATION_HH_

#include "CoreActionInitialization.hh"

#include <string>

class CoreRunAction;

class FamousExternalParticleSourceActionInitialization: public CoreActionInitialization {
private:
	std::string input;

public:
	FamousExternalParticleSourceActionInitialization(std::string input, std::string path, std::string prefix="");

	virtual G4VUserPrimaryGeneratorAction* createPrimaryGeneratorAction() const;
	virtual CoreRunAction* createRunAction() const;
};

#endif /* FAMOUSEXTERNALPARTICLESOURCEACTIONINITIALIZATION_HH_ */
