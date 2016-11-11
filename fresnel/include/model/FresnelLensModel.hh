/**
 * FresnelLensModel.hh
 *
 * @date Mar 7, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELLENSMODEL_HH_
#define FRESNELLENSMODEL_HH_

#include <vector>
#include <G4MaterialPropertiesTable.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "CoreMaterialFactory.hh"

class FresnelLensModel {
private:
	/**
	 * Radius of the lens.
	 */
	double r;
	/**
	 * Focal length.
	 */
	double f;
	/**
	 * Conic constant.
	 */
	double k;
	/**
	 * Refractive index of the lens material.
	 */
	double rIndex;
	double rIndexAir;
	/**
	 * Aspheric constants.
	 */
	std::vector<double> a;
	/**
	 * Number of grooves per mm.
	 */
	double numberOfGrooves;
	/**
	 * Thickness of the support material.
	 */
	double thickness;
	/**
	 * Additional distance which should be added to the focal length to get best focus.
	 */
	double dz;
	G4Material* material;
	/**
	 * Draft angle.
	 */
	double draftAngle;
	/**
	 * Dynamic draft angle factor a * r+ draftAngle.
	 */
	double draftAngleFactor;

public:
	FresnelLensModel(double r, double f);

	/**
	 * @return double - the curvature of the lens surface.
	 */
	double getC() const;
	/**
	 * @return double - the thickness of the highest groove.
	 */
	double getHeighestGrooveThickness() const;
	/**
	 * @return double - the intrinsic thickness of the highest groove + the support material thickness.
	 */
	double getTotalThickness() const;
	/**
	 * The total number of grooves situated on the lens.
	 */
	int getTotalNumberOfGrooves() const;
	/**
	 * Derives the sagitta of the lens at a given radius r.
	 */
	double getSagitta(double ri) const;

	double getR() const;
	void setR(double r);
	double getF() const;
	void setF(double f);
	double getNumberOfGrooves() const;
	void setNumberOfGrooves(double numberOfGrooves);
	double getK() const;
	void setK(double k);
	double getRIndex() const;
	void setRIndex(double rIndex);
	double getRIndexAir() const;
	std::vector<double>& getA();
	double getThickness() const;
	void setThickness(double thickness);
	double getDz() const;
	void setDz(double dz);
	G4Material* getMaterial() const;
	void setMaterial(G4Material* material);
	double getDraftAngle() const;
	void setDraftAngle(double draftAngle);
	double getDraftAngleFactor() const;
	void setDraftAngleFactor(double draftAngleFactor);
};

#endif /* FRESNELLENSMODEL_HH_ */
