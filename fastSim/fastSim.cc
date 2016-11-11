/*
 * fastSim.cc
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

#include "CoreActionInitialization.hh"
#include "FastDetectorConstruction.hh"

int main(int argc, char** argv) {
	// Define and parse the program options.
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()("help", "Produce help message.") //
	("mac", po::value<std::string>(), "Macro input file.") //
	("output", po::value<std::string>()->default_value("./results"), "Output directory (./results).") //
	("dia", po::value<double>()->default_value(549.7 * CLHEP::mm), "The diameter of the lens.") //
	("fn", po::value<double>()->default_value(0.9134073131), "Focal number f/# of the lens.") //
	("grooves", po::value<double>()->default_value(1.0 / CLHEP::mm), "Number of grooves per mm.") //
	("dz", po::value<double>()->default_value(0), "Shift of focal plane.") //
	("thickness", po::value<double>()->default_value(2.0 * CLHEP::mm), "Thickness of the lens.");
	po::variables_map vm = ProgramOptionsUtil::parse(argc, argv, desc, true);
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	// Create fresnel lens model.
	const double d = vm["dia"].as<double>();
	const double fn = vm["fn"].as<double>();
	const double dz = vm["dz"].as<double>();
	const double grooves = vm["grooves"].as<double>();
	const double thickness = vm["thickness"].as<double>();
	FresnelLensModel* fresnelLensModel = new FresnelLensModel(d / 2., fn * d);
	fresnelLensModel->setThickness(thickness);
	fresnelLensModel->setNumberOfGrooves(grooves);
	fresnelLensModel->setDz(dz);
	// Construct the run manager.
	G4RunManager* runManager = new G4RunManager();
	// Set mandatory initialization classes.
	runManager->SetUserInitialization(new FastDetectorConstruction(fresnelLensModel, false, false));
	runManager->SetUserInitialization(new OpticalPhysicsList);
	runManager->SetUserInitialization(new CoreActionInitialization(vm["output"].as<std::string>(), "fast"));
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
	if (vm.count("mac")) {
		uiManager->ApplyCommand((G4String("/control/execute ") + vm["mac"].as<std::string>()));
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
