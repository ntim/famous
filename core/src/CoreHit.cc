/*
 * CoreHit.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CoreHit.hh"

G4ThreadLocal G4Allocator<CoreHit>* CoreHitAllocator = 0;

CoreHit::CoreHit() :
		eKin(0), parentId(-1), trackId(-1), time(0), weight(0), opticalPathLength(0) {
	//
}

G4double CoreHit::getEKin() const {
	return eKin;
}

void CoreHit::setEKin(G4double _eKin) {
	eKin = _eKin;
}

G4ThreeVector CoreHit::getMomentum() const {
	return momentum;
}

void CoreHit::setMomentum(G4ThreeVector _momentum) {
	momentum = _momentum;
}

G4int CoreHit::getParentId() const {
	return parentId;
}

void CoreHit::setParentId(G4int _parentId) {
	parentId = _parentId;
}

G4String CoreHit::getParticleName() const {
	return particleName;
}

void CoreHit::setParticleName(G4String _particleName) {
	particleName = _particleName;
}

G4ThreeVector CoreHit::getPosition() const {
	return position;
}

void CoreHit::setPosition(G4ThreeVector _position) {
	position = _position;
}

G4String CoreHit::getProcessName() const {
	return processName;
}

void CoreHit::setProcessName(G4String _processName) {
	processName = _processName;
}

G4ThreeVector CoreHit::getStartMomentum() const {
	return startMomentum;
}

void CoreHit::setStartMomentum(G4ThreeVector _startMomentum) {
	startMomentum = _startMomentum;
}

G4ThreeVector CoreHit::getStartPosition() const {
	return startPosition;
}

void CoreHit::setStartPosition(G4ThreeVector _startPosition) {
	startPosition = _startPosition;
}

G4double CoreHit::getTime() const {
	return time;
}

void CoreHit::setTime(G4double _time) {
	time = _time;
}

G4int CoreHit::getTrackId() const {
	return trackId;
}

void CoreHit::setTrackId(G4int _trackId) {
	trackId = _trackId;
}

G4String CoreHit::getVolumeName() const {
	return volumeName;
}

void CoreHit::setVolumeName(G4String _volumeName) {
	volumeName = _volumeName;
}

G4double CoreHit::getWeight() const {
	return weight;
}

void CoreHit::setWeight(G4double _weight) {
	weight = _weight;
}

G4double CoreHit::getOpticalPathLength() const {
	return opticalPathLength;
}

void CoreHit::setOpticalPathLength(G4double _opticalPathLength) {
	opticalPathLength = _opticalPathLength;
}
