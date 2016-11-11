/*
 * FresnelRunAction.hh
 *
 * @date 03.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELSIM_INCLUDE_FRESNELRUNACTION_HH_
#define FRESNELSIM_INCLUDE_FRESNELRUNACTION_HH_

#include <CoreRunAction.hh>

class FresnelRunAction: public CoreRunAction {
public:
	FresnelRunAction(std::string filename);
	virtual void BeginOfRunAction(const G4Run* run);
};

#endif /* FRESNELSIM_INCLUDE_FRESNELRUNACTION_HH_ */
