/*
 * sim.cc
 *
 * @date Sep 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Germany License
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

#include <OpticalPhysicsList.hh>
#include <ProgramOptionsUtil.hh>

#include "FamousActionInitialization.hh"
#include "FamousShowerActionInitialization.hh"
#include "FamousExternalParticleSourceActionInitialization.hh"
#include "FamousDetectorConstruction.hh"
#include "model/FamousModel.hh"

int main(int argc, char** argv) {
	std::string mac, output, prefix, input;
	int showerId, bgThin;
	bool addBg;
	double dz, curv, ug11FilterThickness;
	// Define and parse the program options.
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()("help", "Produce help message.") //
	("mac", po::value<std::string>(&mac)->default_value(""), "Macro input file.") //
	("output", po::value<std::string>(&output)->default_value("./results"), "Output directory (./results).") //
	("prefix", po::value<std::string>(&prefix)->default_value(""), "Prefix to be appended to the output filename.") //
	("shower-id", po::value<int>(&showerId)->default_value(-1), "Id of the shower in the supplied shower file.") //
	("input", po::value<std::string>(&input)->default_value(""), "Input file.") //
	("add-bg", po::value<bool>(&addBg)->default_value(false), "Add night-sky background.") //
	("bg-thin", po::value<int>(&bgThin)->default_value(1), "Thinning factor for night-sky background.") //
	("dz", po::value<double>(&dz)->default_value(9.55575), "Focal plane shift.") //
	("curv", po::value<double>(&curv)->default_value(0), "Focal plane curvature factor.") //
	("ug11-filter-thickness", po::value<double>(&ug11FilterThickness)->default_value(1.0),
			"Thickness of the UG11 filter, set to zero to disable it.");
	po::variables_map vm = ProgramOptionsUtil::parse(argc, argv, desc, true);
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	// Construct the run manager.
	G4RunManager* runManager = new G4RunManager();
	// Set mandatory initialization classes.
	FamousModel* model = new FamousModel(61);
	model->getFresnelLensModel()->setDz(dz * CLHEP::mm);
	model->setCurvatureOfField(curv);
	model->setUg11FilterThickness(ug11FilterThickness);
	runManager->SetUserInitialization(new FamousDetectorConstruction(model));
	runManager->SetUserInitialization(new OpticalPhysicsList);
	if (!input.empty()) {
		runManager->SetUserInitialization(new FamousExternalParticleSourceActionInitialization(input, output, prefix));
	} else if (showerId >= 0 && !input.empty()) {
		runManager->SetUserInitialization(
				new FamousShowerActionInitialization(output, input, showerId, addBg, bgThin, prefix));
	} else {
		runManager->SetUserInitialization(new FamousActionInitialization(output, prefix));
	}
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
