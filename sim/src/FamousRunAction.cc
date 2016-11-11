/*
 * FamousRunAction.cc
 *
 * @date Oct 9, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FamousRunAction.hh"

#include <G4RunManager.hh>

#include "FamousDetectorConstruction.hh"
#include "FamousPersistVisitor.hh"
#include "model/FamousModel.hh"

FamousRunAction::FamousRunAction(std::string _filename) :
		CoreRunAction(_filename) {
	setPersistencyHandler(new CorePersistencyHandler(new FamousPersistVisitor));
}

void FamousRunAction::BeginOfRunAction(const G4Run* run) {
	CoreRunAction::BeginOfRunAction(run);
	// Persist Famous model.
	FamousDetectorConstruction* d =
			(FamousDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	FamousModel* model = d->getModel();
	(* (FamousPersistVisitor*) getPersistencyHandler()->getVisitor())(model);
}
