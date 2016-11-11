/*
 * CoreEventAction.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COREEVENTACTION_HH_
#define COREEVENTACTION_HH_

#include <G4UserEventAction.hh>
#include <G4HCofThisEvent.hh>
#include <G4DCofThisEvent.hh>
#include <G4VHitsCollection.hh>
#include <G4VDigiCollection.hh>
#include <G4Event.hh>

class CoreEventAction: public G4UserEventAction {
public:
	CoreEventAction();

	virtual void EndOfEventAction(const G4Event* event);
};

#endif /* COREEVENTACTION_HH_ */
