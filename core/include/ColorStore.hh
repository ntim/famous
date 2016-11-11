/*
 * ColorStore.hh
 *
 * @date 13.12.2010
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef COLORSTORE_HH_
#define COLORSTORE_HH_

#include <G4VisAttributes.hh>

/**
 * Singleton for storing colors.
 */
class ColorStore {
private:
	G4VisAttributes* green;
	G4VisAttributes* red;
	G4VisAttributes* roux;
	G4VisAttributes* brown;
	G4VisAttributes* metal;
	G4VisAttributes* blue;
	G4VisAttributes* translucentBlue;
	G4VisAttributes* yellow;
	G4VisAttributes* black;
	G4VisAttributes* purple;

	ColorStore();

public:
	/**
	 * Singleton pattern.
	 */
	static ColorStore* getInstance();

	virtual ~ColorStore();

	G4VisAttributes *getGreen() const;
	G4VisAttributes *getBlue() const;
	G4VisAttributes *getTranslucentBlue() const;
	G4VisAttributes *getBrown() const;
	G4VisAttributes *getMetal() const;
	G4VisAttributes *getRed() const;
	G4VisAttributes *getRoux() const;
	G4VisAttributes *getYellow() const;
	G4VisAttributes *getBlack() const;
	G4VisAttributes *getPurple() const;
};

#endif /* COLORSTORE_HH_ */
