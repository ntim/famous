/*
 * FresnelLensParametrisation.hh
 *
 * @date Mar 7, 2012
 * @author Michael Eichler, Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELLENSPARAMETRISATION_HH_
#define FRESNELLENSPARAMETRISATION_HH_

#include <G4VPVParameterisation.hh>
#include <G4Cons.hh>
#include <G4VPhysicalVolume.hh>

#include "model/FresnelLensModel.hh"

class FresnelLensParametrisation: public G4VPVParameterisation {
private:
	FresnelLensModel* model;
	double grooveWidth;
	double heighestGroove;
	void check(const double dz, const int grooveId) const;

public:
	FresnelLensParametrisation(FresnelLensModel* model);

	virtual void ComputeTransformation(const G4int, G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Cons&, const G4int, const G4VPhysicalVolume*) const;

	virtual void ComputeDimensions(G4Box&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Tubs&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Trd&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Trap&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Sphere&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Orb&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Ellipsoid&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Torus&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Para&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Polycone&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Polyhedra&, const G4int, const G4VPhysicalVolume*) const;
	virtual void ComputeDimensions(G4Hype&, const G4int, const G4VPhysicalVolume*) const;
};

#endif /* FRESNELLENSPARAMETRISATION_HH_ */
