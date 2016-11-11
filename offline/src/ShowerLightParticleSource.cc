/*
 * ShowerLightParticleSource.cc
 *
 * @date Apr 21, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "ShowerLightParticleSource.hh"

#include <cmath>
#include <boost/format.hpp>

#include <G4PrimaryVertex.hh>
#include <G4RunManager.hh>
#include <G4OpticalPhoton.hh>

#include <TTree.h>

#include "NightSkySimulator.hh"

ShowerLightParticleSource::ShowerLightParticleSource(G4String _fileName) :
		fileName(_fileName), file(0), rMax(0.255 * CLHEP::m), thetaMax(90 * CLHEP::deg), addNightSkyPhotons(true) {
	particleTable = G4ParticleTable::GetParticleTable();
	position = G4ThreeVector(0, 0, 0);
	rotation = G4RotationMatrix(G4RotationMatrix::IDENTITY);
	nightSkyPhotonsThinning = 1;
}

void ShowerLightParticleSource::readShowerInfo(int eventId) {
	TTree* header = (TTree*) file->Get("header");
	// Sanity check.
	if (header == NULL || header->GetEntries() <= eventId) {
		G4Exception("ShowerLightParticleSource::readShowerInfo", "0", JustWarning,
				"Shower info tree does does not exist in shower root file.");
		return;
	}
	// Read.
	showerInfo.eventId = eventId;
	showerInfo.tMin = 1e9;
	showerInfo.tMax = -1e9;
	header->SetBranchAddress("logE", &showerInfo.logE);
	header->SetBranchAddress("azimuth", &showerInfo.azimuth);
	header->SetBranchAddress("zenith", &showerInfo.zenith);
	header->SetBranchAddress("distance", &showerInfo.distance);
	header->SetBranchAddress("xMax", &showerInfo.xMax);
	header->SetBranchAddress("xFirst", &showerInfo.xFirst);
	header->GetEntry(eventId);
}

void ShowerLightParticleSource::readShowerPhotons(int eventId) {
	// Get tree.
	TTree* t = (TTree*) file->Get(boost::str(boost::format("event%d") % eventId).c_str());
	if (!t) {
		G4Exception("ShowerLightParticleSource::readShowerPhotons", "0", FatalErrorInArgument,
				"Event tree does does not exist in shower root file.");
	}
	// Empty photons array.
	photons.clear();
	// Set branch adresses.
	double x, y, px, py, pz, timeInterval, wavelength, weight;
	t->SetBranchAddress("x", &x);
	t->SetBranchAddress("y", &y);
	t->SetBranchAddress("px", &px);
	t->SetBranchAddress("py", &py);
	t->SetBranchAddress("pz", &pz);
	t->SetBranchAddress("time", &timeInterval);
	t->SetBranchAddress("wavelength", &wavelength);
	t->SetBranchAddress("weight", &weight);
	const int nEntries = t->GetEntries();
	showerInfo.sumOfWeights = 0;
	for (int i = 0; i < nEntries; i++) {
		t->GetEntry(i);
		G4ThreeVector pos(y * CLHEP::m, x * CLHEP::m, 0);
		G4ThreeVector dir(py, px, pz);
		Photon photon(eventId, wavelength * CLHEP::m, pos, dir, timeInterval * CLHEP::ns,
				std::max(1., weight));
		// Consider photon if it meets the defined criterions.
		if (checkPhoton(photon)) {
			// Multiply photon if weight > 1.
			if (weight > 1) {
				for (int j = 0; j < std::floor(weight); j++) {
					// Randomize position on aperture.
					double r = rMax * sqrt(CLHEP::RandFlat::shoot());
					double phi = CLHEP::RandFlat::shoot() * CLHEP::twopi;
					G4ThreeVector newPosition(r * sin(phi), r * cos(phi));
					// Randomize arrival time within the Auger FD time bin.
					double time = CLHEP::RandFlat::shoot(timeInterval * CLHEP::ns - 50. * CLHEP::ns,
							timeInterval * CLHEP::ns + 50. * CLHEP::ns);
					Photon singlePhoton(eventId, wavelength * CLHEP::m, newPosition, dir, time, 1);
					photons.push_back(singlePhoton);
				}
			} else {
				photons.push_back(photon);
			}
			// Log minimum and maximum time.
			showerInfo.tMin = std::min(showerInfo.tMin, photon.getTime());
			showerInfo.tMax = std::max(showerInfo.tMax, photon.getTime());
			//
			showerInfo.sumOfWeights += weight;
		}
	}
	showerInfo.nFluoPhotons = photons.size();
	std::cout << "ShowerLightParticleSource::readShowerPhotons(eventId=" << eventId << "): read logE="
			<< showerInfo.logE << " shower with " << showerInfo.nFluoPhotons << " photons." << std::endl;
}

void ShowerLightParticleSource::generateNightSkyPhotons() {
	NightSkySimulator sim(rMax, thetaMax, showerInfo.tMin - 100. * CLHEP::ns, showerInfo.tMax + 100. * CLHEP::ns);
	sim.setThinningFactor(nightSkyPhotonsThinning);
	std::vector<Photon> nightSkyPhotons = sim.getPhotons();
	showerInfo.nNightSkyPhotons = nightSkyPhotons.size();
	for (std::vector<Photon>::iterator it = nightSkyPhotons.begin(); it != nightSkyPhotons.end(); it++) {
		photons.push_back(*it);
		showerInfo.sumOfWeights += (*it).getWeight();
	}
}

void ShowerLightParticleSource::initialize(int eventId) {
	// Open root file.
	file = new TFile(fileName, "OPEN");
	if (!file->IsOpen()) {
		G4Exception("ShowerLightParticleSource::initialize", "0", FatalException, "Could not read shower root file.");
	}
	// Read shower information.
	readShowerInfo(eventId);
	// Read shower photons.
	readShowerPhotons(eventId);
	// Add nightsky photons.
	if (addNightSkyPhotons && photons.size() > 0) {
		generateNightSkyPhotons();
	}
	currentPhoton = photons.begin();
	// Close file.
	file->Close();
	file = NULL;
}

bool ShowerLightParticleSource::checkPhoton(const Photon& photon) {
	G4double r = sqrt(pow(photon.getPosition().x(), 2) + pow(photon.getPosition().y(), 2));
	G4double theta = acos(-photon.getDirection().z() / photon.getDirection().mag());
	// Check conditions.
	if (photon.getWeight() < 1.) {
		return false;
	}
	if (r <= rMax) {
		if (theta <= thetaMax) {
			return true;
		}
	}
	return false;
}

void ShowerLightParticleSource::GeneratePrimaries(G4Event* event) {
	for (std::vector<Photon>::iterator it = photons.begin(); it != photons.end(); it++) {
		Photon& photon = *it;
		// Create Geant4 particle.
		G4PrimaryParticle* particle = new G4PrimaryParticle();
		particle->SetMomentumDirection(photon.getDirection());
		particle->SetPolarization(1.0, 0.0, 0.0);
		particle->SetKineticEnergy(photon.getEnergy());
		particle->SetParticleDefinition(G4OpticalPhoton::Definition());
		// Adjust position and direction.
		G4ThreeVector start = photon.getPosition() + position;
		start = rotation * start;
		// Create Vertex.
		G4PrimaryVertex * vertex = new G4PrimaryVertex(start, photon.getTime());
		vertex->SetPrimary(particle);
		vertex->SetWeight(photon.getWeight());
		// Add vertex to event.
		event->AddPrimaryVertex(vertex);
	}
}

G4double ShowerLightParticleSource::getNightSkyPhotonsThinning() const {
	return nightSkyPhotonsThinning;
}

void ShowerLightParticleSource::setNightSkyPhotonsThinning(G4double _nightSkyPhotonsThinning) {
	nightSkyPhotonsThinning = _nightSkyPhotonsThinning;
}

G4bool ShowerLightParticleSource::isAddNightSkyPhotons() const {
	return addNightSkyPhotons;
}

G4String ShowerLightParticleSource::getFileName() const {
	return fileName;
}

G4double ShowerLightParticleSource::getRMax() const {
	return rMax;
}

G4ThreeVector ShowerLightParticleSource::getPosition() const {
	return position;
}

G4RotationMatrix ShowerLightParticleSource::getRotation() const {
	return rotation;
}

G4double ShowerLightParticleSource::getThetaMax() const {
	return thetaMax;
}

void ShowerLightParticleSource::setAddNightSkyPhotons(G4bool _addNightSkyPhotons) {
	addNightSkyPhotons = _addNightSkyPhotons;
}

void ShowerLightParticleSource::setFileName(G4String _fileName) {
	fileName = _fileName;
}

void ShowerLightParticleSource::setRMax(G4double _rMax) {
	rMax = _rMax;
}

void ShowerLightParticleSource::setPosition(G4ThreeVector _position) {
	position = _position;
}

void ShowerLightParticleSource::setRotation(G4RotationMatrix _rotation) {
	rotation = _rotation;
}

void ShowerLightParticleSource::setThetaMax(G4double _thetaMax) {
	thetaMax = _thetaMax;
}
