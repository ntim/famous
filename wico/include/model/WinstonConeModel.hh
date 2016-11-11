/**
 * WinstonConeModel.hh
 *
 * @date Mar 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef WINSTONCONEMODEL_HH_
#define WINSTONCONEMODEL_HH_

#include <map>

#include <G4Material.hh>
#include <G4SurfaceProperty.hh>
#include <CLHEP/Units/SystemOfUnits.h>

class WinstonConeModel {
private:
	double r1;
	double r2;
	double thickness;
	double surfaceSigmaAlpha;
	bool solid;
	bool outerFaceStraight;
	G4Material* material;

	void check(const double theta) const;
	double getV(double theta) const;

public:
	WinstonConeModel(double r1, double r2);
	virtual ~WinstonConeModel();

	virtual double getLength() const;
	virtual double getThetaMax() const;
	virtual double getF() const;
	/**
	 * @param theta - in (thetaMax, pi/2)
	 * @return double - the radial distance from the optical axis.
	 */
	virtual double getR(double theta) const;
	/**
	 * @param theta - in (thetaMax, pi/2)
	 * @return double - the height on the optical axis with respect to the center at l/2.
	 */
	virtual double getZ(double theta) const;

	virtual int getNumberOfSides() const;
	double getR1() const;
	void setR1(double r1);
	double getR2() const;
	void setR2(double r2);
	double getThickness() const;
	void setThickness(double thickness);
	double getSurfaceSigmaAlpha() const;
	void setSurfaceSigmaAlpha(double surfaceSigmaAlpha);
	bool isSolid() const;
	void setSolid(bool solid);
	G4Material* getMaterial() const;
	void setMaterial(G4Material* material);
	bool isOuterFaceStraight() const;
	void setOuterFaceStraight(bool outerFaceStraight);
};

#endif /* WINSTONCONEMODEL_HH_ */
