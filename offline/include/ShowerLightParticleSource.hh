/*
 * ShowerLightParticleSource.hh
 *
 * @date Apr 21, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef SHOWERLIGHTPARTICLESOURCE_HH_
#define SHOWERLIGHTPARTICLESOURCE_HH_

#include <vector>

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4RotationMatrix.hh>

#include <TFile.h>
#include <TRandom3.h>

#include "Photon.hh"

/**
 * Generates "opticalphotons" according to the simulated fluorescence yield simulated by
 * Auger offline.
 */
class ShowerLightParticleSource: public G4VUserPrimaryGeneratorAction {
private:
	G4String fileName;
	TFile* file;
	/**
	 * Encapsulated shower information.
	 */
	struct ShowerInfo {
		G4int eventId;
		G4double logE;
		G4double azimuth;
		G4double zenith;
		G4double distance;
		G4double xMax;
		G4double xFirst;
		G4double sumOfWeights;
		G4int nFluoPhotons;
		G4int nNightSkyPhotons;
		G4double tMin;
		G4double tMax;
	} showerInfo;

	std::vector<Photon> photons;
	std::vector<Photon>::iterator currentPhoton;
	G4ParticleTable* particleTable;
	G4ThreeVector position;
	G4RotationMatrix rotation;
	/**
	 * Used to limit the area of which the photons will start.
	 */
	G4double rMax;
	/**
	 * Used to limit the maximum incident angle with respect to the surface normal.
	 */
	G4double thetaMax;
	/**
	 * Add background photons radiated from the night sky.
	 */
	G4bool addNightSkyPhotons;
	G4double nightSkyPhotonsThinning;

protected:
	/**
	 * Reads the shower information.
	 *
	 * @param eventId - the event id.
	 */
	void readShowerInfo(int eventId);
	/**
	 * Reads all shower photons from the ROOT file.
	 *
	 * @param eventId - the event id.
	 */
	void readShowerPhotons(int eventId);
	/**
	 * Generates nightksy photons.
	 */
	void generateNightSkyPhotons();
	/**
	 * @param photon - the photon to check.
	 * @return bool - true if the photon should be shot.
	 */
	bool checkPhoton(const Photon& photon);

public:
	/**
	 * Constructor.
	 *
	 * @param fileName - the shower file file-name.
	 */
	ShowerLightParticleSource(G4String fileName);

	/**
	 * Initializes the readout.
	 *
	 * @param eventId - the event id (default: 0).
	 */
	void initialize(int eventId = 0);

	void GeneratePrimaries(G4Event*);

	G4double getNightSkyPhotonsThinning() const;
	void setNightSkyPhotonsThinning(G4double nightSkyPhotonsThinning);
	G4bool isAddNightSkyPhotons() const;
	G4String getFileName() const;
	G4double getRMax() const;
	G4ThreeVector getPosition() const;
	G4RotationMatrix getRotation() const;
	G4double getThetaMax() const;
	void setAddNightSkyPhotons(G4bool addNightSkyPhotons);
	void setFileName(G4String fileName);
	void setRMax(G4double rMax);
	void setPosition(G4ThreeVector position);
	void setRotation(G4RotationMatrix rotation);
	void setThetaMax(G4double thetaMax);

};

#endif /* SHOWERLIGHTPARTICLESOURCE_HH_ */
