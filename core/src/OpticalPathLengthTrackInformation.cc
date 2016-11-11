/*
 * OpticalPathLengthTrackInformation.cc
 *
 * @date Oct 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <OpticalPathLengthTrackInformation.hh>

G4Allocator<OpticalPathLengthTrackInformation> opticalPathLengthTrackInformationAllocator;

OpticalPathLengthTrackInformation::OpticalPathLengthTrackInformation() :
		originalTrackId(0), opticalPathLength(0) {
}

OpticalPathLengthTrackInformation::OpticalPathLengthTrackInformation(const G4Track* track) :
		opticalPathLength(0) {
	originalTrackId = track->GetTrackID();
}

OpticalPathLengthTrackInformation::OpticalPathLengthTrackInformation(
		const OpticalPathLengthTrackInformation* aTrackInfo) {
	originalTrackId = aTrackInfo->getOriginalTrackId();
	opticalPathLength = aTrackInfo->getOpticalPathLength();
}

OpticalPathLengthTrackInformation::~OpticalPathLengthTrackInformation() {
	//
}

inline int OpticalPathLengthTrackInformation::operator ==(const OpticalPathLengthTrackInformation& right) {
	return (this == &right);
}

void OpticalPathLengthTrackInformation::Print() const {
	G4cout << "Original track ID " << originalTrackId << " optical path length " << opticalPathLength << G4endl;
}
int OpticalPathLengthTrackInformation::getOriginalTrackId() const {
	return originalTrackId;
}

void OpticalPathLengthTrackInformation::setOriginalTrackId(int _originalTrackId) {
	originalTrackId = _originalTrackId;
}

double OpticalPathLengthTrackInformation::getOpticalPathLength() const {
	return opticalPathLength;
}

void OpticalPathLengthTrackInformation::setOpticalPathLength(double _opticalPathLength) {
	opticalPathLength = _opticalPathLength;
}
