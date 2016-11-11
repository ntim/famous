/**
 * FresnelLensParametrisation.cc
 *
 * @date Mar 7, 2012
 * @author Michael Eichler, Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelLensParametrisation.hh"

#include <cmath>

FresnelLensParametrisation::FresnelLensParametrisation(FresnelLensModel* _model) :
		G4VPVParameterisation(), model(_model) {
	grooveWidth = 1.0 / model->getNumberOfGrooves();
	// Get the height of the most outer groove.
	heighestGroove = model->getTotalThickness() - model->getThickness();
}

void FresnelLensParametrisation::check(const double dz, const int grooveId) const {
	if (dz <= 0. || std::isnan(dz)) {
		std::cerr << "FresnelLensParametrisation::check(): groove height is negative or NaN." << std::endl;
		std::cerr << "\t" << "grooveWidth=" << grooveWidth << " mm" << std::endl;
		std::cerr << "\t" << "heighestGroove=" << heighestGroove << " mm" << std::endl;
		std::cerr << "\t" << "grooveId=" << grooveId << std::endl;
		throw 1;
	}
}

void FresnelLensParametrisation::ComputeTransformation(const G4int id, G4VPhysicalVolume* physVol) const {
	// Calculate groove extend on lens surface.
	double rMin = (id + 0) * (grooveWidth);
	double rMax = (id + 1) * (grooveWidth);
	// Calculate groove height.
	double dz = model->getSagitta(rMax) - model->getSagitta(rMin);
	check(dz, id);
	// Compute volume origin.
	G4ThreeVector origin(0.0, 0.0, -heighestGroove / 2.0 + dz / 2.0);
	physVol->SetTranslation(origin);
	physVol->SetRotation(0);
}

void FresnelLensParametrisation::ComputeDimensions(G4Cons& solid, const G4int id, const G4VPhysicalVolume*) const {
	// Calculate groove extend on lense surface.
	double rMin1 = (id + 0) * (grooveWidth);
	double rMax1 = (id + 1) * (grooveWidth);
	double rMin2 = rMin1;
	double rMax2 = rMin2;
	// Calculate groove height.
	double dz = model->getSagitta(rMax1) - model->getSagitta(rMin1);
	check(dz, id);
	// Calculate intersection between y=x*tan(90-xi) and y=h-h/grooveWidth*x
	if (id > 0 && (model->getDraftAngle() > 0.0 || model->getDraftAngleFactor() != 0.0)) {
		// Calculate dynamic draft angle.
		double xi = model->getDraftAngle() + model->getDraftAngleFactor() * rMin1;
		// Shorthand calculation of tangens.
		double tanXi = tan(90.0 * CLHEP::deg - xi);
		if (std::isnan(tanXi)) {
			std::cerr << "FresnelLensParametrisation::check(): draft angle NaN." << std::endl;
			throw 1;
		}
		// Relative coordinates of intersection.
		double xs = dz / (tanXi + dz / grooveWidth);
		double ys = xs * tanXi;
		// Update geometry.
		rMin2 = rMin2 + xs;
		rMax2 = rMin2;
		dz = ys;
	}
	// Set solid properties.
	solid.SetInnerRadiusMinusZ(rMin1);
	solid.SetOuterRadiusMinusZ(rMax1);
	solid.SetInnerRadiusPlusZ(rMin2);
	solid.SetOuterRadiusPlusZ(rMax2);
	solid.SetZHalfLength(dz / 2.0);
}

void FresnelLensParametrisation::ComputeDimensions(G4Box&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Tubs&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Trd&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Trap&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Sphere&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Orb&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Ellipsoid&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Torus&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Para&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Polycone&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Polyhedra&, const G4int, const G4VPhysicalVolume*) const {
	//
}

void FresnelLensParametrisation::ComputeDimensions(G4Hype&, const G4int, const G4VPhysicalVolume*) const {
	//
}
