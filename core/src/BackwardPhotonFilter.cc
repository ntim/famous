/*
 * BackwardPhotonFilter.cc
 *
 * @date Sep 16, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "BackwardPhotonFilter.hh"

BackwardPhotonFilter::BackwardPhotonFilter() :
		G4VSDFilter("backwardPhotonFilter") {
	//
}

G4bool BackwardPhotonFilter::Accept(const G4Step* step) const {
	return step->GetPreStepPoint()->GetMomentumDirection().z() <= 0;
}
