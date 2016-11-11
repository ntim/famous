/*
 * FresnelPersistVisitor.cc
 *
 * @date 03.06.2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelPersistVisitor.hh"

#include <TFile.h>
#include <TTree.h>

void FresnelPersistVisitor::operator ()(FresnelLensModel* m) {
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
