/*
 * FamousExternalParticleSource.cc
 *
 * @date 17.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FamousExternalParticleSource.hh"

#include <boost/format.hpp>

#include <G4PrimaryParticle.hh>
#include <G4OpticalPhoton.hh>
#include <G4PrimaryVertex.hh>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Random/RandFlat.h>

#include <TTree.h>

FamousExternalParticleSource::FamousExternalParticleSource(std::string filename, G4ThreeVector _position) :
		G4VUserPrimaryGeneratorAction(), position(_position) {
	particleGun = new G4ParticleGun(1);
	file = new TFile(filename.c_str());
	if (!file->IsOpen()) {
		G4Exception("FamousExternalParticleSource::FamousExternalParticleSource", "0", FatalErrorInArgument,
				"No such file or directory");
		throw 1;
	}
}

FamousExternalParticleSource::~FamousExternalParticleSource() {
	delete file;
}

G4ThreeVector FamousExternalParticleSource::getPolarizationVector(double angle) {
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
	G4ThreeVector polar = std::cos(angle) * e_paralle + std::sin(angle) * e_perpend;
	return polar;
}

void FamousExternalParticleSource::GeneratePrimaries(G4Event* event) {
	TTree* tree = (TTree*) file->Get(boost::str(boost::format("event%d") % event->GetEventID()).c_str());
	if (!tree) {
		G4Exception("FamousExternalParticleSource::GeneratePrimaries", "0", FatalErrorInArgument,
				"Event tree does does not exist in root file.");
	}
	double x, y, px, py, pz, time, wavelength, weight;
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("y", &y);
	tree->SetBranchAddress("px", &px);
	tree->SetBranchAddress("py", &py);
	tree->SetBranchAddress("pz", &pz);
	tree->SetBranchAddress("time", &time);
	tree->SetBranchAddress("wavelength", &wavelength);
	tree->SetBranchAddress("weight", &weight);
	for (long long i = 0; i < tree->GetEntries(); i++) {
		tree->GetEntry(i);
		// Create Geant4 particle.
		G4PrimaryParticle* particle = new G4PrimaryParticle();
		particle->SetMomentumDirection(G4ThreeVector(px, py, -pz));
		particle->SetPolarization(getPolarizationVector(-1));
		particle->SetKineticEnergy(CLHEP::c_light * CLHEP::h_Planck / (wavelength * CLHEP::nm));
		particle->SetParticleDefinition(G4OpticalPhoton::Definition());
		// Create Vertex.
		G4PrimaryVertex * vertex = new G4PrimaryVertex(G4ThreeVector(x * CLHEP::mm, y * CLHEP::mm, 0) + position,
				time * CLHEP::ns);
		vertex->SetPrimary(particle);
		vertex->SetWeight(weight);
		// Add vertex to event.
		event->AddPrimaryVertex(vertex);
	}
}
