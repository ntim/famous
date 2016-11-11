/*
 * wicoSim.cc
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

#include <CLHEP/Units/PhysicalConstants.h>

#include <OpticalPhysicsList.hh>
#include <ProgramOptionsUtil.hh>
#include <model/G4SipmModelFactory.hh>

#include "CoreMaterialFactory.hh"
#include "WicoActionInitialization.hh"
#include "WicoDetectorConstruction.hh"
#include "model/WinstonConeModelFactory.hh"

void modifyReflectionProperties(double pSpec, double pLobe, double pBs) {
	G4MaterialPropertiesTable* mpt = CoreMaterialFactory::getInstance()->getAluminum()->GetMaterialPropertiesTable();
	//
	const double eMin = CLHEP::h_Planck * CLHEP::c_light / CoreMaterialFactory::LAMBDA_MAX;
	const double eMax = CLHEP::h_Planck * CLHEP::c_light / CoreMaterialFactory::LAMBDA_MIN;
	// Spike reflection.
	G4MaterialPropertyVector* v = new G4MaterialPropertyVector();
	v->InsertValues(eMin, pSpec);
	v->InsertValues(eMax, pSpec);
	mpt->AddProperty("SPECULARSPIKECONSTANT", v);
	// Lobe reflection.
	v = new G4MaterialPropertyVector();
	v->InsertValues(eMin, pLobe);
	v->InsertValues(eMax, pLobe);
	mpt->AddProperty("SPECULARLOBECONSTANT", v);
	// Back scattering.
	v = new G4MaterialPropertyVector();
	v->InsertValues(eMin, pBs);
	v->InsertValues(eMax, pBs);
	mpt->AddProperty("BACKSCATTERCONSTANT", v);
	// Display warning if all probabilities summed up are greater than 1:
	if (pSpec + pLobe + pBs > 1.) {
		std::cerr << "modifyReflectionProperties(): sum of probabilities greater than one." << std::endl;
		throw 1;
	}
}

WinstonConeModel* createWicoModel(std::string name, double r1, double r2) {
	if (name == "round") {
		return WinstonConeModelFactory::createRound(r1, r2);
	} else if (name == "rect") {
		return WinstonConeModelFactory::createRectangular(r1, r2);
	} else if (name == "hex") {
		return WinstonConeModelFactory::createHexagonal(r1, r2);
	} else if (name == "hextoround") {
		return WinstonConeModelFactory::createHexagonal(r1, r2);
	}
	std::cerr << "main(): geometry not recognized." << std::endl;
	throw 1;
}

G4SipmModel* createSipmModel(std::string name) {
	if (name == "null") {
		return NULL;
	}
	if (name == "generic") {
		return G4SipmModelFactory::getInstance()->createGenericSipmModel();
	}
	if (name == "HamamatsuS1036211100") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS1036211100();
	}
	if (name == "HamamatsuS1036233100") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS1036233100();
	}
	if (name == "HamamatsuS10985100") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS10985100();
	}
	if (name == "HamamatsuS12651050") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS12651050();
	}
	if (name == "HamamatsuS1036233050") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS1036233050();
	}
	if (name == "HamamatsuS12573100X") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS12573100X();
	}
	return G4SipmModelFactory::getInstance()->createConfigFileModel(name);
}

int main(int argc, char** argv) {
	std::string mac, output, geometry, sipm;
	double r1, r2, pSpec, pLobe, pBs, sigmaAlpha;
	bool solid;
	// Define and parse the program options.
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()("help", "Produce help message.") //
	("mac", po::value<std::string>(&mac)->default_value(""), "Macro input file.") //
	("output", po::value<std::string>(&output)->default_value("./results"), "Output directory (./results).") //
	("geometry", po::value<std::string>(&geometry)->default_value("round"), "Winston cone geometry (round, hex, rect).") //
	("solid", po::value<bool>(&solid)->default_value(false), "Build solid Winston cone out of acrylic.") //
	("r1", po::value<double>(&r1)->default_value(6.71 * CLHEP::mm), "The radius of the incident area in mm.") //
	("r2", po::value<double>(&r2)->default_value(3. * CLHEP::mm), "The radius of the emergent area in mm.") //
	("p_spec", po::value<double>(&pSpec)->default_value(0.), "Probability of specular spike reflection of the cone.") //
	("p_lobe", po::value<double>(&pLobe)->default_value(1.), "Probability of lobe reflection of the cone.") //
	("p_bs", po::value<double>(&pBs)->default_value(0.), "Probability of back scattering of the cone.") //
	("sigma_alpha", po::value<double>(&sigmaAlpha)->default_value(0.45),
			"Surface roughness stddev of the normal angle in deg.") //
	("sipm", po::value<std::string>(&sipm)->default_value("HamamatsuS10985100"),
			"SiPM model (null, generic, properties file, model name).");
	po::variables_map vm = ProgramOptionsUtil::parse(argc, argv, desc, true);
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	// Modify reflection probabilities.
	modifyReflectionProperties(pSpec, pLobe, pBs);
	// Parse command line arguments and initialize Winston cone model.
	WinstonConeModel* wicoModel = createWicoModel(geometry, r1, r2);
	wicoModel->setSolid(solid);
	wicoModel->setSurfaceSigmaAlpha(sigmaAlpha * CLHEP::deg);
	// Parse command line arguments and initialize Sipm model.
	G4SipmModel* sipmModel = createSipmModel(sipm);
	// Construct the run manager.
	G4RunManager* runManager = new G4RunManager();
	// Set mandatory initialization classes.
	runManager->SetUserInitialization(new WicoDetectorConstruction(wicoModel, sipmModel));
	runManager->SetUserInitialization(new OpticalPhysicsList);
	runManager->SetUserInitialization(new WicoActionInitialization(output));
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
