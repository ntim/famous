/*
 * FresnelRunAction.cc
 *
 * @date 03.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelRunAction.hh"

#include <G4RunManager.hh>

#include "model/FresnelLensModel.hh"
#include "FresnelDetectorConstruction.hh"
#include "FresnelPersistVisitor.hh"

FresnelRunAction::FresnelRunAction(std::string _filename) :
		CoreRunAction(_filename, new FresnelPersistVisitor) {
	//
}

void FresnelRunAction::BeginOfRunAction(const G4Run* run) {
	CoreRunAction::BeginOfRunAction(run);
	FresnelDetectorConstruction* d =
			(FresnelDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	(*((FresnelPersistVisitor*) getPersistencyHandler()->getVisitor()))(d->getModel());
}
