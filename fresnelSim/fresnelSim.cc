/*
 * fresnelSim.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <boost/program_options.hpp>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif
#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#ifdef G4UI_USE_QT
#include <G4UIQt.hh>
#endif
#endif

#include <CLHEP/Units/SystemOfUnits.h>

#include <OpticalPhysicsList.hh>
#include <ProgramOptionsUtil.hh>

#include "FresnelPersistVisitor.hh"
#include "FresnelActionInitialization.hh"
#include "FresnelDetectorConstruction.hh"
#include "OpticalPathLengthSteppingAction.hh"

int main(int argc, char** argv) {
	std::string mac, output;
	double d, fn, dz, grooves, thickness, draftAngle, draftAngleFactor;
	// Define and parse the program options.
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()("help", "Produce help message.") //
	("mac", po::value<std::string>(&mac)->default_value(""), "Macro input file.") //
	("output", po::value<std::string>(&output)->default_value("./results"), "Output directory (./results).") //
	("dia", po::value<double>(&d)->default_value(502.1 * CLHEP::mm), "The diameter of the lens in mm.") //
	("fn", po::value<double>(&fn)->default_value(0.9925), "Focal number f/# of the lens.") //
	("grooves", po::value<double>(&grooves)->default_value(2.0 / CLHEP::mm), "Number of grooves per mm.") //
	("dz", po::value<double>(&dz)->default_value(0), "Shift of focal plane in mm.") //
	("thickness", po::value<double>(&thickness)->default_value(2.5 * CLHEP::mm), "Thickness of the lens in mm.") //
	("draft-angle", po::value<double>(&draftAngle)->default_value(3.0), "Lens draft angle in degrees") //
	("draft-angle-factor", po::value<double>(&draftAngleFactor)->default_value(0.0473),
			"Lens draft angle progression factor in degrees per mm.");
	po::variables_map vm = ProgramOptionsUtil::parse(argc, argv, desc, true);
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	// Create fresnel lens model.
	FresnelLensModel* fresnelLensModel = new FresnelLensModel(d / 2., fn * d);
	fresnelLensModel->setThickness(thickness);
	fresnelLensModel->setNumberOfGrooves(grooves);
	fresnelLensModel->setDz(dz);
	fresnelLensModel->setDraftAngle(draftAngle * CLHEP::deg);
	fresnelLensModel->setDraftAngleFactor(draftAngleFactor * CLHEP::deg / CLHEP::mm);
	// Construct the run manager.
	G4RunManager* runManager = new G4RunManager();
	// Set mandatory initialization classes.
	runManager->SetUserInitialization(new FresnelDetectorConstruction(fresnelLensModel));
	runManager->SetUserInitialization(new OpticalPhysicsList);
	runManager->SetUserInitialization(new FresnelActionInitialization(output));
	runManager->SetUserAction(new OpticalPathLengthSteppingAction);
	// Update GPS messenger with Fresnel lens geometry.
	GeneralParticleSourceMessenger::getInstance()->setPlaneShape("circle");
	GeneralParticleSourceMessenger::getInstance()->setPlaneRMin(0);
	GeneralParticleSourceMessenger::getInstance()->setPlaneRMax(fresnelLensModel->getR());
	GeneralParticleSourceMessenger::getInstance()->setPlanePos(
			G4ThreeVector(0, 0, fresnelLensModel->getTotalThickness()));
	GeneralParticleSourceMessenger::getInstance()->setAnglePhiMin(90.0 * CLHEP::deg);
	GeneralParticleSourceMessenger::getInstance()->setAnglePhiMax(90.0 * CLHEP::deg);
	GeneralParticleSourceMessenger::getInstance()->setAngleThetaMin(0.0);
	GeneralParticleSourceMessenger::getInstance()->setAngleThetaMax(0.0);
	// Initialize G4 kernel.
	runManager->Initialize();
	// Create visualization manager.
	G4UImanager* uiManager = G4UImanager::GetUIpointer();
#ifdef G4VIS_USE
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	// Create UI manager.
#ifdef G4UI_USE
	uiManager = G4UImanager::GetUIpointer();
	// Batch mode.
	if (!mac.empty()) {
		uiManager->ApplyCommand((G4String("/control/execute ") + mac));
	} else {
		// Interactive mode.
#ifdef G4UI_USE_QT
		// Create Qt UI.
		G4UIQt* ui = new G4UIQt(argc, argv);
		uiManager->ApplyCommand("/control/execute vis-qt.mac");
		ui->SessionStart();
		delete ui;
#else
		// Create default UI.
		G4UIExecutive * ui = new G4UIExecutive(argc, argv);
		uiManager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete ui;
#endif
	}
#endif
	// Tear down.
	delete runManager;
	return 0;
}
