/*
 * FamousPersistVisitor.cc
 *
 * @date Oct 9, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FamousPersistVisitor.hh"

#include <TTree.h>
#include <TFile.h>

void FamousPersistVisitor::operator ()(FamousModel* m) {
	TTree* t = new TTree("famousModel", "famousModel");
	int numberOfPixels = m->getNumberOfPixels();
	double curvatureOfField = m->getCurvatureOfField();
	t->Branch("numberOfPixels", &numberOfPixels);
	t->Branch("curvatureOfField", &curvatureOfField);
	t->Fill();
	getFile()->WriteTObject(t);
	// Pixel coordinates.
	t = new TTree("famousModelPixelCoordinates", "famousModelPixelCoordinates");
	double x, y;
	t->Branch("x", &x);
	t->Branch("y", &y);
	std::vector<hex::CartesianCoordinate> coords = m->getPixelCoordinates();
	for (size_t i = 0; i < coords.size(); i++) {
		x = coords[i].x;
		y = coords[i].y;
		t->Fill();
	}
	getFile()->WriteTObject(t);
	// Add contained models.
	(*(CoreRootPersistVisitor*) this)(m->getSipmModel());
	(*this)(m->getFresnelLensModel());
	(*this)(m->getWicoModel());
}

void FamousPersistVisitor::operator ()(FresnelLensModel* m) {
	TTree* t = new TTree("fresnelLensModel", "fresnelLensModel");
	double r = m->getR();
	double f = m->getF();
	double k = m->getK();
	double rIndex = m->getRIndex();
	double rIndexAir = m->getRIndexAir();
	double numberOfGrooves = m->getNumberOfGrooves();
	double thickness = m->getThickness();
	double totalThickness = m->getTotalThickness();
	double dz = m->getDz();
	double draftAngle = m->getDraftAngle();
	double draftAngleFactor = m->getDraftAngleFactor();
	t->Branch("r", &r);
	t->Branch("f", &f);
	t->Branch("k", &k);
	t->Branch("rIndex", &rIndex);
	t->Branch("rIndexAir", &rIndexAir);
	t->Branch("numberOfGrooves", &numberOfGrooves);
	t->Branch("thickness", &thickness);
	t->Branch("totalThickness", &totalThickness);
	t->Branch("dz", &dz);
	t->Branch("draftAngle", &draftAngle);
	t->Branch("draftAngleFactor", &draftAngleFactor);
	t->Fill();
	getFile()->WriteTObject(t);
}

void FamousPersistVisitor::operator ()(WinstonConeModel* m) {
	TTree* t = new TTree("winstonConeModel", "winstonConeModel");
	double r1 = m->getR1();
	double r2 = m->getR2();
	double thickness = m->getThickness();
	double surfaceSigmaAlpha = m->getSurfaceSigmaAlpha();
	bool solid = m->isSolid();
	double length = m->getLength();
	double thetaMax = m->getThetaMax();
	t->Branch("r1", &r1);
	t->Branch("r2", &r2);
	t->Branch("thickness", &thickness);
	t->Branch("surfaceSigmaAlpha", &surfaceSigmaAlpha);
	t->Branch("solid", &solid);
	t->Branch("length", &length);
	t->Branch("thetaMax", &thetaMax);
	t->Fill();
	getFile()->WriteTObject(t);
}
