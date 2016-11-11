/*
 * OpticalPathLengthTrackInformation.hh
 *
 * @date Oct 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef OPTICALPATHLENGTHTRACKINFORMATION_HH_
#define OPTICALPATHLENGTHTRACKINFORMATION_HH_

#include <G4VUserTrackInformation.hh>
#include <G4Track.hh>

class OpticalPathLengthTrackInformation: public G4VUserTrackInformation {
private:
	int originalTrackId;
	double opticalPathLength;

public:
	OpticalPathLengthTrackInformation();
	OpticalPathLengthTrackInformation(const G4Track* track);
	OpticalPathLengthTrackInformation(const OpticalPathLengthTrackInformation* trackInfo);
	virtual ~OpticalPathLengthTrackInformation();

	inline void *operator new(size_t);
	inline void operator delete(void *trackInfo);
	inline int operator ==(const OpticalPathLengthTrackInformation& right);

	void Print() const;

	int getOriginalTrackId() const;
	void setOriginalTrackId(int originalTrackId);
	double getOpticalPathLength() const;
	void setOpticalPathLength(double opticalPathLength);
};

extern G4Allocator<OpticalPathLengthTrackInformation> opticalPathLengthTrackInformationAllocator;

inline void* OpticalPathLengthTrackInformation::operator new(size_t) {
	void* aTrackInfo;
	aTrackInfo = (void*) opticalPathLengthTrackInformationAllocator.MallocSingle();
	return aTrackInfo;
}

inline void OpticalPathLengthTrackInformation::operator delete(void *trackInfo) {
	opticalPathLengthTrackInformationAllocator.FreeSingle((OpticalPathLengthTrackInformation*) trackInfo);
}

#endif /* OPTICALPATHLENGTHTRACKINFORMATION_HH_ */
