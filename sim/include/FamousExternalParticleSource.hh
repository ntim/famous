/*
 * FamousExternalParticleSource.hh
 *
 * @date 17.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSEXTERNALPARTICLESOURCE_HH_
#define FAMOUSEXTERNALPARTICLESOURCE_HH_

#include <string>

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include <G4Event.hh>

#include <TFile.h>

class FamousExternalParticleSource: public G4VUserPrimaryGeneratorAction {
private:
	G4ParticleGun* particleGun;
	TFile* file;
	G4ThreeVector position;

	G4ThreeVector getPolarizationVector(double angle);

public:
	FamousExternalParticleSource(std::string filename, G4ThreeVector position = G4ThreeVector(0, 0, 0));
	virtual ~FamousExternalParticleSource();

	virtual void GeneratePrimaries(G4Event* event);
};

#endif /* FAMOUSEXTERNALPARTICLESOURCE_HH_ */
