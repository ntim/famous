/*
 * FamousModel.hh
 *
 * @date Aug 6, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FAMOUSMODEL_HH_
#define FAMOUSMODEL_HH_

#include <model/G4SipmModel.hh>

#include "model/WinstonConeModel.hh"
#include "model/FresnelLensModel.hh"
#include "Hex.hh"

/**
 * Telescope model.
 */
class FamousModel {
private:
	size_t numberOfPixels;
	bool symmetric;
	G4SipmModel* sipmModel;
	WinstonConeModel* wicoModel;
	FresnelLensModel* fresnelLensModel;
	double ug11FilterThickness;
	double curvatureOfField;

public:
	/**
	 * @param numberOfPixels - the number of pixels.
	 * @param symmetric - true for a symmetric camera (central pixel).
	 */
	FamousModel(size_t numberOfPixels, bool symmetric = true);

	/**
	 * @return double - the field of view of the whole camera.
	 */
	double getFieldOfView() const;
	/**
	 * @return double - the field of view of a single pixel.
	 */
	double getPixelFieldOfView() const;
	/**
	 * @return vector of all cartesian coordinates of the pixels.
	 */
	std::vector<hex::CartesianCoordinate> getPixelCoordinates() const;

	/**
	 * @return unsigned int - the number of pixels.
	 */
	size_t getNumberOfPixels() const;
	/**
	 * @return true if the camera has a central pixel.
	 */
	bool isSymmetric() const;
	/**
	 * @return double - the thickness of the UG11 filter plate which is placed in front of each SiPM.
	 */
	double getUg11FilterThickness() const;
	void setUg11FilterThickness(double ug11FilterThickness);
	/**
	 * @return double - the curvature of the camera
	 */
	double getCurvatureOfField() const;
	void setCurvatureOfField(double curvatureOfField);

	FresnelLensModel* getFresnelLensModel() const;
	G4SipmModel* getSipmModel() const;
	WinstonConeModel* getWicoModel() const;
};

#endif /* FAMOUSMODEL_HH_ */
