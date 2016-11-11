/**
 * ShowerPhoton.cc
 *
 * @date Apr 21, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "Photon.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

Photon::Photon(G4int _eventId, G4double _wavelength, G4ThreeVector _position, G4ThreeVector _direction, G4double _time,
		G4double _weight) :
		eventId(_eventId), position(_position), direction(_direction), time(_time), weight(_weight) {
	setWavelength(_wavelength);
	setSource(FLUORESCENCE);
}

PhotonSource Photon::getSource() const {
	return source;
}

void Photon::setSource(PhotonSource _source) {
	source = _source;
}

G4ThreeVector Photon::getDirection() const {
	return direction;
}

void Photon::setDirection(G4ThreeVector _direction) {
	direction = _direction;
}

G4double Photon::getEnergy() const {
	return energy;
}

void Photon::setEnergy(G4double _energy) {
	energy = _energy;
	wavelength = CLHEP::h_Planck * CLHEP::c_light / energy;
}

G4int Photon::getEventId() const {
	return eventId;
}

void Photon::setEventId(G4int _eventId) {
	eventId = _eventId;
}

G4ThreeVector Photon::getPosition() const {
	return position;
}

void Photon::setPosition(G4ThreeVector _position) {
	position = _position;
}

G4double Photon::getTime() const {
	return time;
}

void Photon::setTime(G4double _time) {
	time = _time;
}

G4double Photon::getWavelength() const {
	return wavelength;
}

void Photon::setWavelength(G4double _wavelength) {
	wavelength = _wavelength;
	energy = CLHEP::h_Planck * CLHEP::c_light / wavelength;
}

G4double Photon::getWeight() const {
	return weight;
}

void Photon::setWeight(G4double _weight) {
	weight = _weight;
}
