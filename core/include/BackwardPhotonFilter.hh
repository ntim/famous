/*
 * BackwardPhotonFilter.hh
 *
 * @date Sep 16, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef BACKWARDPHOTONFILTER_HH_
#define BACKWARDPHOTONFILTER_HH_

#include <G4VSDFilter.hh>
#include <G4Step.hh>

class BackwardPhotonFilter: public G4VSDFilter {
public:
	BackwardPhotonFilter();

	virtual G4bool Accept(const G4Step* step) const;
};

#endif /* BACKWARDPHOTONFILTER_HH_ */
