/*
 * NightSkySimulator.cc
 *
 * @date Jun 29, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "NightSkySimulator.hh"

#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

NightSkySimulator::NightSkySimulator(G4double _apertureRadius, G4double _thetaMax, G4double _tMin, G4double _tMax) :
		apertureRadius(_apertureRadius), thetaMax(_thetaMax), tMin(_tMin), tMax(_tMax) {
	flux = 1.9e12/* 1/(s m^2 sr) */;
	thinningFactor = 1;
	lambdaMin = 300 * CLHEP::nm;
	lambdaMax = 400 * CLHEP::nm;
	maxNPhotons = 1e6;
}

std::vector<Photon> NightSkySimulator::getPhotons() {
	std::vector<Photon> photons;
	const double t = tMax - tMin;
	const double area = CLHEP::pi * apertureRadius * apertureRadius;
	const double solidAngle = 2. * CLHEP::pi * (1. - cos(thetaMax));
	int nPhotons = ceil(flux * t / CLHEP::second * area / CLHEP::m2 * solidAngle / thinningFactor);
	// Limit amount of photons.
	if (nPhotons > maxNPhotons) {
		std::cerr << "NightSkySimulator::getPhotons(): Derived number of night-sky photons was " << nPhotons;
		thinningFactor *= maxNPhotons / nPhotons;
		nPhotons = maxNPhotons;
		std::cerr << ". Limiting to " << maxNPhotons << std::endl;
	}
	std::cout << "NightSkySimulator::getPhotons(): generating " << nPhotons << " photons in t=(" << tMin / CLHEP::ns
			<< ", " << tMax / CLHEP::ns << ") ns." << std::endl;
	// Create photons.
	for (int i = 0; i < nPhotons; i++) {
		int eventId = -1;
		// Wavelength
		double wavelength = CLHEP::RandFlat::shoot(300 * CLHEP::nm, 400 * CLHEP::nm);
		// Dice direction.
		double phi = CLHEP::RandFlat::shoot(0., 360. * CLHEP::deg);
		double cosTheta = CLHEP::RandFlat::shoot(1., cos(thetaMax));
		G4ThreeVector direction(cos(phi) * sin(acos(cosTheta)), sin(phi) * sin(acos(cosTheta)), -cosTheta);
		// Dice position.
		double r = apertureRadius * sqrt(CLHEP::RandFlat::shoot());
		phi = CLHEP::RandFlat::shoot() * CLHEP::twopi;
		G4ThreeVector position(r * sin(phi), r * cos(phi), 0.);
		// Time.
		double time = CLHEP::RandFlat::shoot(tMin, tMax);
		//
		Photon photon(eventId, wavelength, position, direction, time, thinningFactor);
		photons.push_back(photon);
	}
	return photons;
}

G4double NightSkySimulator::getLambdaMax() const {
	return lambdaMax;
}

G4double NightSkySimulator::getLambdaMin() const {
	return lambdaMin;
}

G4double NightSkySimulator::getMax() const {
	return tMax;
}

G4double NightSkySimulator::getMin() const {
	return tMin;
}

void NightSkySimulator::setLambdaMax(G4double _lambdaMax) {
	lambdaMax = _lambdaMax;
}

void NightSkySimulator::setLambdaMin(G4double _lambdaMin) {
	lambdaMin = _lambdaMin;
}

void NightSkySimulator::setMax(G4double _tMax) {
	tMax = _tMax;
}

void NightSkySimulator::setMin(G4double _tMin) {
	tMin = _tMin;
}

G4double NightSkySimulator::getThinningFactor() const {
	return thinningFactor;
}

void NightSkySimulator::setApertureRadius(G4double _apertureRadius) {
	apertureRadius = _apertureRadius;
}

G4double NightSkySimulator::getFlux() const {
	return flux;
}

void NightSkySimulator::setFlux(G4double _flux) {
	flux = _flux;
}

G4double NightSkySimulator::getThetaMax() const {
	return thetaMax;
}

void NightSkySimulator::setThetaMax(G4double _thetaMax) {
	thetaMax = _thetaMax;
}

void NightSkySimulator::setThinningFactor(G4double _thinningFactor) {
	thinningFactor = _thinningFactor;
}

G4double NightSkySimulator::getApertureRadius() const {
	return apertureRadius;
}
