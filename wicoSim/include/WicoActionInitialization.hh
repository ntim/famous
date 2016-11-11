/*
 * WicoActionInitialization.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef WICOACTIONINITIALIZATION_HH_
#define WICOACTIONINITIALIZATION_HH_

#include "CoreActionInitialization.hh"

class WicoActionInitialization: public CoreActionInitialization {
public:
	WicoActionInitialization(std::string path);

	virtual G4VUserPrimaryGeneratorAction* createPrimaryGeneratorAction() const;
};

#endif /* WICOACTIONINITIALIZATION_HH_ */
