/*
 * CoreSensitiveDetector.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CoreSensitiveDetector.hh"

#include <iostream>
#include <G4SDManager.hh>
#include <G4Track.hh>
#include <OpticalPathLengthTrackInformation.hh>

CoreSensitiveDetector::CoreSensitiveDetector(G4String name, G4String hitsCollectionName, bool _recordWorldPosition) :
		G4VSensitiveDetector(name), hitsCollection(0), recordWorldPosition(_recordWorldPosition) {
	// Register name to the inherited collection.
	collectionName.push_back(hitsCollectionName);
	// Register to Geant4.
	G4SDManager::GetSDMpointer()->AddNewDetector(this);
}

void CoreSensitiveDetector::Initialize(G4HCofThisEvent*) {
	// Initialize hits collection.
	hitsCollection = new CoreHitsCollection(SensitiveDetectorName, collectionName[0]);
}

G4bool CoreSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
	G4TouchableHistory *touchable = (G4TouchableHistory*) ((step->GetPreStepPoint()->GetTouchable()));
	CoreHit *hit = new CoreHit();
	hit->setParticleName(step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName());
	hit->setTrackId(step->GetTrack()->GetTrackID());
	hit->setParentId(step->GetTrack()->GetParentID());
	hit->setEKin(step->GetPreStepPoint()->GetKineticEnergy());
	hit->setTime(step->GetPreStepPoint()->GetGlobalTime());
	hit->setWeight(step->GetPreStepPoint()->GetWeight());
	// Get local coordinates.
	G4ThreeVector worldPosition = step->GetPreStepPoint()->GetPosition();
	G4ThreeVector localPosition = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
	hit->setPosition(localPosition);
	// Get local momentum.
	G4ThreeVector worldMomentum = step->GetPreStepPoint()->GetMomentumDirection();
	G4ThreeVector localMomentum = touchable->GetHistory()->GetTopTransform().TransformAxis(worldMomentum);
	hit->setMomentum(localMomentum);
	// If it is a copied volume, save the world position.
	if (touchable->GetVolume()->GetCopyNo() > 0 || recordWorldPosition) {
		hit->setPosition(worldPosition);
		hit->setMomentum(worldMomentum);
	}
	// Get start position and momentum.
	hit->setStartPosition(step->GetTrack()->GetVertexPosition());
	hit->setStartMomentum(step->GetTrack()->GetVertexMomentumDirection());
	// Store optical path length.
	if (step->GetTrack()->GetUserInformation()) {
		OpticalPathLengthTrackInformation* trackInfo =
				dynamic_cast<OpticalPathLengthTrackInformation*>(step->GetTrack()->GetUserInformation());
		if (trackInfo) {
			hit->setOpticalPathLength(trackInfo->getOpticalPathLength());
		}
	}
	//
	hitsCollection->insert(hit);
	return true;
}

void CoreSensitiveDetector::EndOfEvent(G4HCofThisEvent* hCof) {
	int collectionId = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hCof->AddHitsCollection(collectionId, hitsCollection);
	std::cout << "CoreSensitiveDetector::EndOfEvent(): added hits collection \"" << collectionName[0]
			<< "\" for sensitive detector \"" << SensitiveDetectorName << "\" with " << hitsCollection->GetSize()
			<< " entries." << std::endl;
}
