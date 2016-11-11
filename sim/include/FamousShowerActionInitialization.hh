/*
 * FamousShowerActionInitialization.hh
 *
 * @date Aug 6, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSSHOWERACTIONINITIALIZATION_HH_
#define FAMOUSSHOWERACTIONINITIALIZATION_HH_

#include "CoreActionInitialization.hh"

#include "ShowerLightParticleSource.hh"

class FamousShowerActionInitialization: public CoreActionInitialization {
private:
	std::string input;
	int inputIndex;
	bool addBg;
	unsigned int bgThin;

public:
	FamousShowerActionInitialization(std::string path, std::string input, int inputIndex, bool addBg,
			unsigned int bgThin, std::string prefix = "");

	virtual G4VUserPrimaryGeneratorAction* createPrimaryGeneratorAction() const;
};

#endif /* FAMOUSSHOWERACTIONINITIALIZATION_HH_ */
