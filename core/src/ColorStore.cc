/**
 * ColorStore.cc
 *
 * @date 13.12.2010
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "ColorStore.hh"

#include <G4Colour.hh>

ColorStore::ColorStore() {
	red = new G4VisAttributes(G4Colour(255 / 255., 0 / 255., 0 / 255.));
	roux = new G4VisAttributes(G4Colour(204 / 255., 0 / 255., 51 / 255.));
	brown = new G4VisAttributes(G4Colour(255 / 255., 153 / 255., 153 / 255.));
	metal = new G4VisAttributes(G4Colour(204 / 255., 204 / 255., 255 / 255.));
	blue = new G4VisAttributes(G4Colour(0 / 255., 204 / 255., 204 / 255.));
	translucentBlue = new G4VisAttributes(G4Colour(0 / 255., 204 / 255., 204 / 255., 0.33));
	green = new G4VisAttributes(G4Colour(153 / 255., 255 / 255., 153 / 255.));
	yellow = new G4VisAttributes(G4Colour(255 / 255., 255 / 255., 0 / 255.));
	black = new G4VisAttributes(G4Colour(.3, .3, .3));
	purple = new G4VisAttributes(G4Colour(160 / 255., 32 / 255., 240 / 255.));
}

ColorStore* ColorStore::getInstance() {
	static ColorStore* instance = new ColorStore();
	return instance;
}

ColorStore::~ColorStore() {
	delete red;
	delete roux;
	delete brown;
	delete metal;
	delete blue;
	delete green;
	delete yellow;
	delete black;
	delete purple;
}

G4VisAttributes* ColorStore::getGreen() const {
	return green;
}

G4VisAttributes *ColorStore::getBlue() const {
	return blue;
}

G4VisAttributes *ColorStore::getTranslucentBlue() const {
	return translucentBlue;
}

G4VisAttributes *ColorStore::getBrown() const {
	return brown;
}

G4VisAttributes *ColorStore::getMetal() const {
	return metal;
}

G4VisAttributes *ColorStore::getRed() const {
	return red;
}

G4VisAttributes *ColorStore::getRoux() const {
	return roux;
}

G4VisAttributes *ColorStore::getYellow() const {
	return yellow;
}

G4VisAttributes *ColorStore::getBlack() const {
	return black;
}

G4VisAttributes *ColorStore::getPurple() const {
	return purple;
}
