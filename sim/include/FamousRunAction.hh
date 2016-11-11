/*
 * FamousRunAction.hh
 *
 * @date Oct 9, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSRUNACTION_HH_
#define FAMOUSRUNACTION_HH_

#include "CoreRunAction.hh"

class FamousRunAction: public CoreRunAction {
public:
	FamousRunAction(std::string filename);

	virtual void BeginOfRunAction(const G4Run* run);
};

#endif /* FAMOUSRUNACTION_HH_ */
