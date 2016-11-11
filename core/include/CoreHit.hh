/*
 * CoreHit.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COREHIT_HH_
#define COREHIT_HH_

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4ParticleDefinition.hh>
#include <G4ThreeVector.hh>

class CoreHit: public G4VHit {
private:
	G4double eKin;
	G4int parentId;
	G4int trackId;
	G4double time;
	G4String particleName;
	G4String processName;
	G4String volumeName;
	G4ThreeVector position;
	G4ThreeVector momentum;
	G4ThreeVector startPosition;
	G4ThreeVector startMomentum;
	G4double weight;
	G4double opticalPathLength;

public:
	CoreHit();
	void* operator new(size_t);
	void operator delete(void*);

	G4double getEKin() const;
	void setEKin(G4double eKin);
	G4ThreeVector getMomentum() const;
	void setMomentum(G4ThreeVector momentum);
	G4int getParentId() const;
	void setParentId(G4int parentId);
	G4String getParticleName() const;
	void setParticleName(G4String particleName);
	G4ThreeVector getPosition() const;
	void setPosition(G4ThreeVector position);
	G4String getProcessName() const;
	void setProcessName(G4String processName);
	G4ThreeVector getStartMomentum() const;
	void setStartMomentum(G4ThreeVector startMomentum);
	G4ThreeVector getStartPosition() const;
	void setStartPosition(G4ThreeVector startPosition);
	G4double getTime() const;
	void setTime(G4double time);
	G4int getTrackId() const;
	void setTrackId(G4int trackId);
	G4String getVolumeName() const;
	void setVolumeName(G4String volumeName);
	G4double getWeight() const;
	void setWeight(G4double weight);
	G4double getOpticalPathLength() const;
	void setOpticalPathLength(G4double opticalPathLength);
};

typedef G4THitsCollection<CoreHit> CoreHitsCollection;

extern G4ThreadLocal G4Allocator<CoreHit>* CoreHitAllocator;

inline void* CoreHit::operator new(size_t) {
	if (!CoreHitAllocator) {
		CoreHitAllocator = new G4Allocator<CoreHit>;
	}
	return (void*) CoreHitAllocator->MallocSingle();
}

inline void CoreHit::operator delete(void* hit) {
	CoreHitAllocator->FreeSingle((CoreHit*) hit);
}

#endif /* COREHIT_HH_ */
