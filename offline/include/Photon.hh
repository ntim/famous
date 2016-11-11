/**
 * Photon.hh
 *
 * @date Apr 21, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PHOTON_HH_
#define PHOTON_HH_

#include <G4ThreeVector.hh>

enum PhotonSource {
	FLUORESCENCE, NIGHTSKY
};

/**
 * Transition container class for handling of Auger offline generated shower photons.
 */
class Photon {
private:
	G4int eventId;
	G4double energy;
	G4double wavelength;
	G4ThreeVector position;
	G4ThreeVector direction;
	G4double time;
	G4double weight;
	PhotonSource source;

public:
	/**
	 * Constructor.
	 */
	Photon(G4int eventId, G4double wavelength, G4ThreeVector position, G4ThreeVector direction, G4double time,
			G4double weight);

	PhotonSource getSource() const;
	void setSource(PhotonSource source);
	G4ThreeVector getDirection() const;
	void setDirection(G4ThreeVector direction);
	G4double getEnergy() const;
	void setEnergy(G4double energy);
	G4int getEventId() const;
	void setEventId(G4int eventId);
	G4ThreeVector getPosition() const;
	void setPosition(G4ThreeVector position);
	G4double getTime() const;
	void setTime(G4double time);
	G4double getWavelength() const;
	void setWavelength(G4double wavelength);
	G4double getWeight() const;
	void setWeight(G4double weight);

};

#endif /* PHOTON_HH_ */
