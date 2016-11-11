/*
 * GeneralParticleSource.cc
 *
 * @date Aug 7, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "GeneralParticleSource.hh"

#include <algorithm>

#include <boost/format.hpp>

#include <G4UImanager.hh>
#include <G4OpticalPhoton.hh>
#include <CLHEP/Random/RandFlat.h>

#include <rnd/General.hh>

#include "GeneralParticleSourceMessenger.hh"

GeneralParticleSource::GeneralParticleSource() :
		G4VUserPrimaryGeneratorAction() {
	particleGun = new G4ParticleGun(1);
}

G4ThreeVector GeneralParticleSource::getPolarizationVector(double angle) {
	if (angle < 0) {
		// Randomize polarization.
		return getPolarizationVector(CLHEP::RandFlat::shoot(360. * CLHEP::deg));
	}
	// Calculate polarization.
	G4ThreeVector normal(1., 0., 0.);
	G4ThreeVector kphoton = particleGun->GetParticleMomentumDirection();
	G4ThreeVector product = normal.cross(kphoton);
	G4double modul2 = product * product;
	G4ThreeVector e_perpend(0., 0., 1.);
	if (modul2 > 0.) {
		e_perpend = (1. / std::sqrt(modul2)) * product;
	}
	G4ThreeVector e_paralle = e_perpend.cross(kphoton);
	G4ThreeVector polar = std::cos(angle) * e_paralle
			+ std::sin(angle) * e_perpend;
	return polar;
}

void GeneralParticleSource::RandRect::operator ()(double& x, double& y) {
	x = CLHEP::RandFlat::shoot(-a, a);
	y = CLHEP::RandFlat::shoot(-b, b);
}

void GeneralParticleSource::RandCircle::operator ()(double& x, double& y) {
	double r, phi;
	do {
		r = CLHEP::RandFlat::shoot();
		phi = CLHEP::RandFlat::shoot(0.0, CLHEP::twopi);
		x = sqrt(r) * rMax * cos(phi);
		y = sqrt(r) * rMax * sin(phi);
	} while (sqrt(r) * rMax <= rMin);
}

void GeneralParticleSource::RandHex::operator ()(double& x, double& y) {
	const double ri = sqrt(3.) / 2. * a;
	// Y intercept for a straight line with slope -60 deg.
	double m = tan(CLHEP::pi / 3.) * a;
	x = a;
	y = ri;
	// Check if the point is within the hexagon.
	while (-tan(CLHEP::pi / 3.) * fabs(x) + m < fabs(y)) {
		x = CLHEP::RandFlat::shoot(-a, a);
		y = CLHEP::RandFlat::shoot(-ri, ri);
	}
}

void GeneralParticleSource::GeneratePrimaries(G4Event* event) {
	GeneralParticleSourceMessenger* messenger =
			GeneralParticleSourceMessenger::getInstance();
	const unsigned int nParticles = messenger->getNParticles();
	// Dice particle timings.
	std::vector<double> times(nParticles, 0);
	if (messenger->getTProbDistFile() == "") {
		CLHEP::RandFlat::shootArray(nParticles, &times[0], messenger->getTMin(),
				messenger->getTMax());
	} else {
		rnd::General* rand = new rnd::General(messenger->getTProbDistFile());
		rand->shootArray(nParticles, &times[0]);
		// Set the tMin and tMax (although it has here no effect) to match the values from the file.
		G4UImanager::GetUIpointer()->ApplyCommand(
				boost::str(
						boost::format("/gps/tMin %.02f ns")
								% rand->getXMin()));
		G4UImanager::GetUIpointer()->ApplyCommand(
				boost::str(
						boost::format("/gps/tMax %.02f ns")
								% rand->getXMax()));
	}
	std::sort(times.begin(), times.end());
	Rand* posRand = 0;
	if (messenger->getPlaneShape() == "rect") {
		posRand =
				new RandRect { messenger->getPlaneA(), messenger->getPlaneB() };
	} else if (messenger->getPlaneShape() == "hex") {
		posRand = new RandHex { messenger->getPlaneA() };
	} else if (messenger->getPlaneShape() == "circle") {
		posRand = new RandCircle { messenger->getPlaneRMin(),
				messenger->getPlaneRMax() };
	} else {
		std::cerr
				<< "GeneralParticleSource::GeneratePrimaries(): plane shape name \""
				<< messenger->getPlaneShape() << "\" not defined." << std::endl;
		throw 1;
	}
	// Create particles.
	for (size_t i = 0; i < nParticles; i++) {
		particleGun->SetNumberOfParticles(1);
		particleGun->SetParticleDefinition(G4OpticalPhoton::Definition());
		particleGun->SetParticlePolarization(
				getPolarizationVector(messenger->getPolar()));
		particleGun->SetParticleTime(times[i]);
		// Dice energy.
		particleGun->SetParticleEnergy(
				CLHEP::RandFlat::shoot(messenger->getEnergyEMin(),
						messenger->getEnergyEMax()));
		// Dice position.
		double x, y;
		(*posRand)(x, y);
		G4ThreeVector pos(x, y, 0.0);
		// Determine rotation parameters for given surface normal.
		G4ThreeVector dicingPlaneNormal(0, 0, 1);
		G4ThreeVector sourcePlaneNormal = messenger->getPlaneSurfaceNormal();
		G4ThreeVector rotationAxis = dicingPlaneNormal.cross(sourcePlaneNormal);
		G4double rotationAngle = 0.;
		if (rotationAxis.mag() != 0.) {
			rotationAngle =
					acos(
							dicingPlaneNormal * sourcePlaneNormal
									/ (dicingPlaneNormal.mag()
											* sourcePlaneNormal.mag()));
		} else if (sourcePlaneNormal == -dicingPlaneNormal) {
			rotationAngle = 180. * CLHEP::deg;
		}
		// Rotate particle position.
		if (rotationAngle == 180. * CLHEP::deg) {
			pos *= -1;
		} else if (rotationAngle != 0.) {
			pos.rotate(rotationAxis, rotationAngle);
		}
		particleGun->SetParticlePosition(pos + messenger->getPlanePos());
		// Dice momentum regarding the cosine law.
		G4double phi = CLHEP::RandFlat::shoot(messenger->getAnglePhiMin(),
				messenger->getAnglePhiMax());
		G4double cosTheta = CLHEP::RandFlat::shoot(
				cos(messenger->getAngleThetaMin()),
				cos(messenger->getAngleThetaMax()));
		G4double sinTheta = sqrt(1. - cosTheta * cosTheta);
		G4ThreeVector momentumDirection(sinTheta * cos(phi),
				sinTheta * sin(phi), cosTheta);
		// Rotate momentum direction.
		if (rotationAngle == 180. * CLHEP::deg) {
			momentumDirection *= -1;
		} else if (rotationAngle != 0.) {
			momentumDirection.rotate(rotationAxis, rotationAngle);
		}
		particleGun->SetParticleMomentumDirection(momentumDirection);
		// Fire!
		particleGun->GeneratePrimaryVertex(event);
	}
}
