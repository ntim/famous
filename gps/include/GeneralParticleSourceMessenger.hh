/*
 * GeneralParticleSourceMessenger.hh
 *
 * @date Aug 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef GENERALPARTICLESOURCEMESSENGER_HH_
#define GENERALPARTICLESOURCEMESSENGER_HH_

#include <G4UImessenger.hh>
#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "G4UIcmdWithADouble.hh"
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>

/**
 * A messenger for configuring the GeneralParticleSource.
 *
 * Commands:
 * /gps/verbose
 * /gps/nParticles
 * /gps/polar
 * /gps/tMin
 * /gps/tMax
 * /gps/tProbDistFile
 * /gps/energy/eMin
 * /gps/energy/eMax
 * /gps/angle/phiMin
 * /gps/angle/phiMax
 * /gps/angle/thetaMin
 * /gps/angle/thetaMax
 * /gps/plane/surfaceNormal
 * /gps/plane/a
 * /gps/plane/b
 * /gps/plane/rMin
 * /gps/plane/rMax
 * /gps/plane/pos
 * /gps/plane/shape
 */
class GeneralParticleSourceMessenger: public G4UImessenger {
private:
	static const std::string MACRO_FILE_NAME;

	// Plain values
	int verbose;
	int nParticles;
	double polar;
	double tMin;
	double tMax;
	G4String tProbDistFile;
	double energyEMin;
	double energyEMax;
	double anglePhiMin;
	double anglePhiMax;
	double angleThetaMin;
	double angleThetaMax;
	G4ThreeVector planeSurfaceNormal;
	double planeA;
	double planeB;
	double planeRMin;
	double planeRMax;
	G4ThreeVector planePos;
	G4String planeShape;

	// Commands
	G4UIcmdWithAnInteger* verboseCmd;
	G4UIcmdWithAnInteger* nParticlesCmd;
	G4UIcmdWithADoubleAndUnit* polarCmd;
	G4UIcmdWithADoubleAndUnit* tMinCmd;
	G4UIcmdWithADoubleAndUnit* tMaxCmd;
	G4UIcmdWithAString* tProbDistFileCmd;
	G4UIcmdWithADoubleAndUnit* energyEMinCmd;
	G4UIcmdWithADoubleAndUnit* energyEMaxCmd;
	G4UIcmdWithADoubleAndUnit* anglePhiMinCmd;
	G4UIcmdWithADoubleAndUnit* anglePhiMaxCmd;
	G4UIcmdWithADoubleAndUnit* angleThetaMinCmd;
	G4UIcmdWithADoubleAndUnit* angleThetaMaxCmd;
	G4UIcmdWith3VectorAndUnit* planeSurfaceNormalCmd;
	G4UIcmdWithADoubleAndUnit* planeACmd;
	G4UIcmdWithADoubleAndUnit* planeBCmd;
	G4UIcmdWithADoubleAndUnit* planeRMinCmd;
	G4UIcmdWithADoubleAndUnit* planeRMaxCmd;
	G4UIcmdWith3VectorAndUnit* planePosCmd;
	G4UIcmdWithAString* planeShapeCmd;

	GeneralParticleSourceMessenger();
public:
	virtual ~GeneralParticleSourceMessenger();

	/**
	 * @return GeneralParticleSourceMessenger - the singleton.
	 */
	static GeneralParticleSourceMessenger* getInstance();

	void SetNewValue(G4UIcommand*, G4String);
	void setDefaultValues();

	int getVerbose() const;
	void setVerbose(int verbose);
	int getNParticles() const;
	void setNParticles(int nParticles);
	double getPolar() const;
	void setPolar(double polar);
	double getTMin() const;
	void setTMin(double tMin);
	double getTMax() const;
	void setTMax(double tMax);
	G4String getTProbDistFile() const;
	void setTProbDistFile(G4String tProbDistFile);
	double getEnergyEMin() const;
	void setEnergyEMin(double energyEMin);
	double getEnergyEMax() const;
	void setEnergyEMax(double energyEMax);
	double getAnglePhiMin() const;
	void setAnglePhiMin(double anglePhiMin);
	double getAnglePhiMax() const;
	void setAnglePhiMax(double anglePhiMax);
	double getAngleThetaMin() const;
	void setAngleThetaMin(double angleThetaMin);
	double getAngleThetaMax() const;
	void setAngleThetaMax(double angleThetaMax);
	double getPlaneA() const;
	void setPlaneA(double planeA);
	double getPlaneB() const;
	void setPlaneB(double planeB);
	double getPlaneRMax() const;
	void setPlaneRMax(double planeRMax);
	double getPlaneRMin() const;
	void setPlaneRMin(double planeRMin);
	G4ThreeVector getPlanePos() const;
	void setPlanePos(G4ThreeVector planePos);
	G4String getPlaneShape() const;
	void setPlaneShape(G4String planeShape);
	G4ThreeVector getPlaneSurfaceNormal() const;
	void setPlaneSurfaceNormal(G4ThreeVector planeSurfaceNormal);

};

#endif /* GENERALPARTICLESOURCEMESSENGER_HH_ */
