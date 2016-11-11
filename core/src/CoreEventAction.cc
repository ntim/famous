/*
 * CoreEventAction.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CoreEventAction.hh"

#include <G4RunManager.hh>
#include <G4DigiManager.hh>
#include <G4VDigitizerModule.hh>

#include "CoreRunAction.hh"
#include "CoreHit.hh"
#include "CorePersistencyHandler.hh"

CoreEventAction::CoreEventAction() {
}

void CoreEventAction::EndOfEventAction(const G4Event* event) {
	CorePersistencyHandler* persistency =
			((CoreRunAction*) G4RunManager::GetRunManager()->GetUserRunAction())->getPersistencyHandler();
	// Run all digitizer modules.
	G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
	G4DCtable* dcTable = digiManager->GetDCtable();
	for (int i = 0; i < dcTable->entries(); i++) {
		G4String dmName = dcTable->GetDMname(i);
		G4VDigitizerModule* dm = digiManager->FindDigitizerModule(dmName);
		if (dm) {
			dm->Digitize();
		}
	}
	// Process hits collections.
	G4HCofThisEvent* hCof = event->GetHCofThisEvent();
	if (hCof != NULL) {
		for (int i = 0; i < hCof->GetCapacity(); ++i) {
			G4VHitsCollection* hc = hCof->GetHC(i);
			if (hc != NULL) {
				if (dynamic_cast<G4SipmHitsCollection*>(hc)) {
					persistency->persist((G4SipmHitsCollection*) hc);
				}
				else if (dynamic_cast<CoreHitsCollection*>(hc)) {
					persistency->persist(CorePersistable((CoreHitsCollection*) hc));
				}
			}
		}
	}
	// Process digi collections.
	G4DCofThisEvent* dCof = event->GetDCofThisEvent();
	if (dCof != NULL) {
		for (int i = 0; i < dCof->GetCapacity(); ++i) {
			G4VDigiCollection* dc = dCof->GetDC(i);
			if (dc != NULL) {
				if (dynamic_cast<G4SipmDigiCollection*>(dc)) {
					persistency->persist((G4SipmDigiCollection*) dc);
				}
				if (dynamic_cast<G4SipmVoltageTraceDigiCollection*>(dc)) {
					persistency->persist((G4SipmVoltageTraceDigiCollection*) dc);
				}
			}
		}
	}
}
