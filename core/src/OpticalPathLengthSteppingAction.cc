/*
 * OpticalPathLengthSteppingAction.cc
 *
 * @date Oct 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <OpticalPathLengthSteppingAction.hh>
#include <G4StepPoint.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialPropertyVector.hh>
#include <G4Track.hh>
#include <G4RunManagerKernel.hh>
#include <G4TrackingManager.hh>
#include <G4SteppingManager.hh>

#include "OpticalPathLengthTrackInformation.hh"

OpticalPathLengthSteppingAction::OpticalPathLengthSteppingAction() {
	//
}

void OpticalPathLengthSteppingAction::UserSteppingAction(const G4Step* step) {
	double length = step->GetStepLength();
	double eKin = step->GetPreStepPoint()->GetKineticEnergy();
	G4MaterialPropertiesTable* mpt = step->GetPreStepPoint()->GetMaterial()->GetMaterialPropertiesTable();
	if (mpt) {
		G4MaterialPropertyVector* property = mpt->GetProperty("RINDEX");
		if (property) {
			double rIndex = property->Value(eKin);
			OpticalPathLengthTrackInformation* trackInfo =
					dynamic_cast<OpticalPathLengthTrackInformation*>(step->GetTrack()->GetUserInformation());
			if (!trackInfo) {
				trackInfo = new OpticalPathLengthTrackInformation(step->GetTrack());
				step->GetTrack()->SetUserInformation(trackInfo);
			}
			trackInfo->setOpticalPathLength(trackInfo->getOpticalPathLength() + rIndex * length);
			if (G4RunManagerKernel::GetRunManagerKernel()->GetTrackingManager()->GetSteppingManager()->GetverboseLevel()
					> 1) {
				G4String preName = "undefined";
				G4String postName = "undefined";
				if (step->GetPreStepPoint()->GetPhysicalVolume()) {
					preName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
				}
				if (step->GetPostStepPoint()->GetPhysicalVolume()) {
					postName = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
				}
				std::cout << "OpticalPathLengthSteppingAction::UserSteppingAction(): " << step->GetTrack()->GetTrackID()
						<< "\t" << preName << "\tl=" << length << "\tn=" << rIndex << "\t" << postName << std::endl;
			}
		}
	}
}
