/*
 * OpticalPathLengthSteppingAction.hh
 *
 * @date Oct 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef OPTICALPATHLENGTHSTEPPINGACTION_HH_
#define OPTICALPATHLENGTHSTEPPINGACTION_HH_

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>

class OpticalPathLengthSteppingAction: public G4UserSteppingAction {
public:
	OpticalPathLengthSteppingAction();

	virtual void UserSteppingAction(const G4Step*);
};

#endif /* OPTICALPATHLENGTHSTEPPINGACTION_HH_ */
