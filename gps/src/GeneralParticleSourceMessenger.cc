/*
 * GeneralParticleSourceMessenger.cc
 *
 * @date Aug 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "GeneralParticleSourceMessenger.hh"

#include <CLHEP/Units/SystemOfUnits.h>

#include <G4UiMessengerUtil.hh>

const std::string GeneralParticleSourceMessenger::MACRO_FILE_NAME = "gps.mac";

GeneralParticleSourceMessenger::GeneralParticleSourceMessenger() :
		G4UImessenger() {
	setDefaultValues();
	// Create directories.
	new G4UIdirectory("/gps/");
	new G4UIdirectory("/gps/energy/");
	new G4UIdirectory("/gps/angle/");
	new G4UIdirectory("/gps/plane/");
	// Create basic commands.
	verboseCmd = G4UiMessengerUtil::createCmd(this, "/gps/", "verbose", verbose);
	nParticlesCmd = G4UiMessengerUtil::createCmd(this, "/gps/", "nParticles", nParticles);
	polarCmd = G4UiMessengerUtil::createCmd(this, "/gps/", "polar", polar, "deg");
	tMinCmd = G4UiMessengerUtil::createCmd(this, "/gps/", "tMin", tMin, "s");
	tMaxCmd = G4UiMessengerUtil::createCmd(this, "/gps/", "tMax", tMax, "s");
	tProbDistFileCmd = G4UiMessengerUtil::createCmd(this, "/gps/", "tProbDistFile", tProbDistFile);
	// Create energy commands.
	energyEMinCmd = G4UiMessengerUtil::createCmd(this, "/gps/energy/", "eMin", energyEMin, "eV");
	energyEMaxCmd = G4UiMessengerUtil::createCmd(this, "/gps/energy/", "eMax", energyEMax, "eV");
	// Create angle commands.
	anglePhiMinCmd = G4UiMessengerUtil::createCmd(this, "/gps/angle/", "phiMin", anglePhiMin, "deg");
	anglePhiMaxCmd = G4UiMessengerUtil::createCmd(this, "/gps/angle/", "phiMax", anglePhiMax, "deg");
	angleThetaMinCmd = G4UiMessengerUtil::createCmd(this, "/gps/angle/", "thetaMin", angleThetaMin, "deg");
	angleThetaMaxCmd = G4UiMessengerUtil::createCmd(this, "/gps/angle/", "thetaMax", angleThetaMax, "deg");
	// Create plane commands.
	planeSurfaceNormalCmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "surfaceNormal", planeSurfaceNormal,
			"mm");
	planeACmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "a", planeA, "mm");
	planeBCmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "b", planeB, "mm");
	planeRMinCmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "rMin", planeRMin, "mm");
	planeRMaxCmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "rMax", planeRMax, "mm");
	planePosCmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "pos", planePos, "mm");
	planeShapeCmd = G4UiMessengerUtil::createCmd(this, "/gps/plane/", "shape", planeShape);
	// Set guidance.
	polarCmd->SetGuidance("Set linear polarization angle w.r.t. (k,n) plane. "
			"Negative values will trigger a random polarization.");
	planeShapeCmd->SetGuidance("Choose one of circle, rect, hex.");
	// Set parameter boundaries.
	verboseCmd->SetParameterName("verbose", false);
	verboseCmd->SetRange("verbose >= 0 && verbose <= 2");
	nParticlesCmd->SetParameterName("nParticles", false);
	nParticlesCmd->SetRange("nParticles > 0");
	// Initialize from macro.
	G4UiMessengerUtil::executeMacro(MACRO_FILE_NAME, true);
}

GeneralParticleSourceMessenger::~GeneralParticleSourceMessenger() {
	delete verboseCmd;
	delete nParticlesCmd;
	delete polarCmd;
	delete tMinCmd;
	delete tMaxCmd;
	delete tProbDistFileCmd;
	delete energyEMinCmd;
	delete energyEMaxCmd;
	delete anglePhiMinCmd;
	delete anglePhiMaxCmd;
	delete angleThetaMinCmd;
	delete angleThetaMaxCmd;
	delete planeSurfaceNormalCmd;
	delete planeACmd;
	delete planeBCmd;
	delete planeRMaxCmd;
	delete planeRMinCmd;
	delete planePosCmd;
	delete planeShapeCmd;
}

GeneralParticleSourceMessenger* GeneralParticleSourceMessenger::getInstance() {
	static GeneralParticleSourceMessenger* instance = new GeneralParticleSourceMessenger;
	return instance;
}

void GeneralParticleSourceMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, verboseCmd, value, &verbose);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, nParticlesCmd, value, &nParticles);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, polarCmd, value, &polar);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, tMinCmd, value, &tMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, tMaxCmd, value, &tMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, tProbDistFileCmd, value, &tProbDistFile);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, energyEMinCmd, value, &energyEMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, energyEMaxCmd, value, &energyEMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, anglePhiMinCmd, value, &anglePhiMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, anglePhiMaxCmd, value, &anglePhiMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, angleThetaMinCmd, value, &angleThetaMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, angleThetaMaxCmd, value, &angleThetaMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planeSurfaceNormalCmd, value, &planeSurfaceNormal);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planeACmd, value, &planeA);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planeBCmd, value, &planeB);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planeRMinCmd, value, &planeRMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planeRMaxCmd, value, &planeRMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planePosCmd, value, &planePos);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, planeShapeCmd, value, &planeShape);
}

void GeneralParticleSourceMessenger::setDefaultValues() {
	verbose = 0;
	nParticles = 1;
	polar = -360 * CLHEP::deg;
	tMin = 0;
	tMax = 10 * CLHEP::ns;
	tProbDistFile = "";
	energyEMin = 1 * CLHEP::eV;
	energyEMax = 10 * CLHEP::eV;
	anglePhiMin = 0;
	anglePhiMax = 360 * CLHEP::deg;
	angleThetaMin = 0;
	angleThetaMax = 90 * CLHEP::deg;
	planeSurfaceNormal = G4ThreeVector(0., 0., -1.);
	planeA = 1 * CLHEP::mm;
	planeB = 1 * CLHEP::mm;
	planeRMin = 0;
	planeRMax = 1 * CLHEP::mm;
	planePos = G4ThreeVector(0, 0, 5 * CLHEP::cm);
	planeShape = "circle";
}

double GeneralParticleSourceMessenger::getAnglePhiMax() const {
	return anglePhiMax;
}

void GeneralParticleSourceMessenger::setAnglePhiMax(double _anglePhiMax) {
	anglePhiMax = _anglePhiMax;
}

double GeneralParticleSourceMessenger::getAnglePhiMin() const {
	return anglePhiMin;
}

void GeneralParticleSourceMessenger::setAnglePhiMin(double _anglePhiMin) {
	anglePhiMin = _anglePhiMin;
}

double GeneralParticleSourceMessenger::getAngleThetaMax() const {
	return angleThetaMax;
}

void GeneralParticleSourceMessenger::setAngleThetaMax(double _angleThetaMax) {
	angleThetaMax = _angleThetaMax;
}

double GeneralParticleSourceMessenger::getAngleThetaMin() const {
	return angleThetaMin;
}

void GeneralParticleSourceMessenger::setAngleThetaMin(double _angleThetaMin) {
	angleThetaMin = _angleThetaMin;
}

double GeneralParticleSourceMessenger::getEnergyEMax() const {
	return energyEMax;
}

void GeneralParticleSourceMessenger::setEnergyEMax(double _energyEMax) {
	energyEMax = _energyEMax;
}

double GeneralParticleSourceMessenger::getEnergyEMin() const {
	return energyEMin;
}

void GeneralParticleSourceMessenger::setEnergyEMin(double _energyEMin) {
	energyEMin = _energyEMin;
}

int GeneralParticleSourceMessenger::getNParticles() const {
	return nParticles;
}

void GeneralParticleSourceMessenger::setNParticles(int _nParticles) {
	nParticles = _nParticles;
}

double GeneralParticleSourceMessenger::getPlaneA() const {
	return planeA;
}

void GeneralParticleSourceMessenger::setPlaneA(double _planeA) {
	planeA = _planeA;
}

double GeneralParticleSourceMessenger::getPlaneB() const {
	return planeB;
}

void GeneralParticleSourceMessenger::setPlaneB(double _planeB) {
	planeB = _planeB;
}

G4ThreeVector GeneralParticleSourceMessenger::getPlanePos() const {
	return planePos;
}

void GeneralParticleSourceMessenger::setPlanePos(G4ThreeVector _planePos) {
	planePos = _planePos;
}

G4String GeneralParticleSourceMessenger::getPlaneShape() const {
	return planeShape;
}

void GeneralParticleSourceMessenger::setPlaneShape(G4String _planeShape) {
	planeShape = _planeShape;
}

G4ThreeVector GeneralParticleSourceMessenger::getPlaneSurfaceNormal() const {
	return planeSurfaceNormal;
}

void GeneralParticleSourceMessenger::setPlaneSurfaceNormal(G4ThreeVector _planeSurfaceNormal) {
	planeSurfaceNormal = _planeSurfaceNormal;
}

double GeneralParticleSourceMessenger::getPolar() const {
	return polar;
}

void GeneralParticleSourceMessenger::setPolar(double _polar) {
	polar = _polar;
}

double GeneralParticleSourceMessenger::getTMax() const {
	return tMax;
}

void GeneralParticleSourceMessenger::setTMax(double _tMax) {
	tMax = _tMax;
}

double GeneralParticleSourceMessenger::getTMin() const {
	return tMin;
}

void GeneralParticleSourceMessenger::setTMin(double _tMin) {
	tMin = _tMin;
}

G4String GeneralParticleSourceMessenger::getTProbDistFile() const {
	return tProbDistFile;
}

void GeneralParticleSourceMessenger::setTProbDistFile(G4String _tProbDistFile) {
	tProbDistFile = _tProbDistFile;
}

int GeneralParticleSourceMessenger::getVerbose() const {
	return verbose;
}

void GeneralParticleSourceMessenger::setVerbose(int _verbose) {
	verbose = _verbose;
}

double GeneralParticleSourceMessenger::getPlaneRMax() const {
	return planeRMax;
}

void GeneralParticleSourceMessenger::setPlaneRMax(double _planeRMax) {
	planeRMax = _planeRMax;
}

double GeneralParticleSourceMessenger::getPlaneRMin() const {
	return planeRMin;
}

void GeneralParticleSourceMessenger::setPlaneRMin(double _planeRMin) {
	planeRMin = _planeRMin;
}
