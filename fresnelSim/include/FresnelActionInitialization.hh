/*
 * FresnelActionInitialization.hh
 *
 * @date 03.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELSIM_INCLUDE_FRESNELACTIONINITIALIZATION_HH_
#define FRESNELSIM_INCLUDE_FRESNELACTIONINITIALIZATION_HH_

#include "CoreActionInitialization.hh"

class FresnelActionInitialization: public CoreActionInitialization {
public:
	FresnelActionInitialization(std::string path);
	virtual CoreRunAction* createRunAction() const;
};

#endif /* FRESNELSIM_INCLUDE_FRESNELACTIONINITIALIZATION_HH_ */
