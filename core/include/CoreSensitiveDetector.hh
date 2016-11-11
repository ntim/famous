/*
 * CoreSensitiveDetector.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef CORESENSITIVEDETECTOR_HH_
#define CORESENSITIVEDETECTOR_HH_

#include <G4VSensitiveDetector.hh>
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4TouchableHistory.hh>

#include "CoreHit.hh"

class CoreSensitiveDetector: public G4VSensitiveDetector {
private:
	CoreHitsCollection* hitsCollection;
	bool recordWorldPosition;

public:
	CoreSensitiveDetector(G4String name, G4String hitsCollectionName = "hits", bool recordWorldPosition = false);

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);
};

#endif /* CORESENSITIVEDETECTOR_HH_ */
