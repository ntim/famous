/*
 * NightSkySimulator.hh
 *
 * @date Jun 29, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef NIGHTSKYSIMULATOR_HH_
#define NIGHTSKYSIMULATOR_HH_

#include <vector>

#include "Photon.hh"

class NightSkySimulator {
private:
	G4double flux;
	G4double apertureRadius;
	G4double thetaMax;
	G4double tMin;
	G4double tMax;
	G4double thinningFactor;
	G4double maxNPhotons;
	G4double lambdaMin;
	G4double lambdaMax;

public:
	NightSkySimulator(G4double apertureRadius, G4double thetaMax, G4double tMin, G4double tMax);

	std::vector<Photon> getPhotons();

	G4double getLambdaMax() const;
	G4double getLambdaMin() const;
	G4double getMax() const;
	G4double getMin() const;
	void setLambdaMax(G4double lambdaMax);
	void setLambdaMin(G4double lambdaMin);
	void setMax(G4double tMax);
	void setMin(G4double tMin);
	G4double getThinningFactor() const;
	void setApertureRadius(G4double apertureRadius);
	G4double getFlux() const;
	void setFlux(G4double flux);
	G4double getThetaMax() const;
	void setThetaMax(G4double thetaMax);
	void setThinningFactor(G4double thinningFactor);
	G4double getApertureRadius() const;
};

#endif /* NIGHTSKYSIMULATOR_HH_ */
