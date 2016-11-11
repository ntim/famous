/*
 * CoreRootPersistVisitor.cc
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "CoreRootPersistVisitor.hh"

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

void CoreRootPersistVisitor::operator ()(CoreHitsCollection* hc) {
	TTree* t = new TTree(hc->GetName(), hc->GetName());
	int parentId, trackId;
	double eKin, time, weight, opticalPathLength;
	double *momentum = new double[3], *position = new double[3], *startMomentum = new double[3], *startPosition =
			new double[3];
	TString particleName, processName, volumeName;
	t->Branch("parentId", &parentId);
	t->Branch("trackId", &trackId);
	t->Branch("particleName", &particleName);
	t->Branch("processName", &processName);
	t->Branch("volumeName", &volumeName);
	t->Branch("eKin", &eKin);
	t->Branch("time", &time);
	t->Branch("weight", &weight);
	t->Branch("opticalPathLength", &opticalPathLength);
	t->Branch("momentum[3]", momentum);
	t->Branch("position[3]", position);
	t->Branch("startMomentum[3]", startMomentum);
	t->Branch("startPosition[3]", startPosition);
	for (size_t i = 0; i < hc->GetSize(); i++) {
		CoreHit* hit = (CoreHit*) hc->GetHit(i);
		parentId = hit->getParentId();
		trackId = hit->getTrackId();
		particleName = hit->getParticleName();
		processName = hit->getProcessName();
		volumeName = hit->getVolumeName();
		eKin = hit->getEKin();
		time = hit->getTime();
		weight = hit->getWeight();
		opticalPathLength = hit->getOpticalPathLength();
		copy(hit->getMomentum(), momentum);
		copy(hit->getPosition(), position);
		copy(hit->getStartMomentum(), startMomentum);
		copy(hit->getStartPosition(), startPosition);
		t->Fill();
	}
	getFile()->WriteTObject(t);
}

void CoreRootPersistVisitor::operator ()(G4SipmHitsCollection* hc) {
	// Delegate.
	RootPersistVisitor::operator ()(hc);
}

void CoreRootPersistVisitor::operator ()(G4SipmDigiCollection* dc) {
	// Delegate.
	RootPersistVisitor::operator ()(dc);
}

void CoreRootPersistVisitor::operator ()(G4SipmVoltageTraceDigiCollection* dc) {
	// Delegate.
	RootPersistVisitor::operator ()(dc);
}

void CoreRootPersistVisitor::operator ()(ParticleSourceMessenger* m) {
	// Delegate.
	RootPersistVisitor::operator ()(m);
}

void CoreRootPersistVisitor::operator ()(G4SipmModel* m) {
	// Delegate.
	RootPersistVisitor::operator ()(m);
}

void CoreRootPersistVisitor::operator ()(G4SipmVoltageTraceModel* m) {
	// Delegate.
	RootPersistVisitor::operator ()(m);
}

void CoreRootPersistVisitor::operator ()(GeneralParticleSourceMessenger* m) {
	TTree* t = new TTree("generalParticleSourceMessenger", "generalParticleSourceMessenger");
	int verbose = m->getVerbose();
	int nParticles = m->getNParticles();
	double polar = m->getPolar();
	double tMin = m->getTMin();
	double tMax = m->getTMax();
	TString* tProbDistFile = new TString(m->getTProbDistFile());
	double eMin = m->getEnergyEMin();
	double eMax = m->getEnergyEMax();
	double phiMin = m->getAnglePhiMax();
	double phiMax = m->getAnglePhiMax();
	double thetaMin = m->getAngleThetaMin();
	double thetaMax = m->getAngleThetaMax();
	double* surfaceNormal = new double[3];
	copy(m->getPlaneSurfaceNormal(), surfaceNormal);
	TString* shape = new TString(m->getPlaneShape());
	double a = m->getPlaneA();
	double b = m->getPlaneB();
	double rMax = m->getPlaneRMax();
	double rMin = m->getPlaneRMin();
	double* pos = new double[3];
	copy(m->getPlanePos(), pos);
	t->Branch("verbose", &verbose);
	t->Branch("nParticles", &nParticles);
	t->Branch("polar", &polar);
	t->Branch("tMin", &tMin);
	t->Branch("tMax", &tMax);
	t->Branch("tProbDistFile", &tProbDistFile);
	t->Branch("energyEMin", &eMin);
	t->Branch("energyEMax", &eMax);
	t->Branch("anglePhiMin", &phiMin);
	t->Branch("anglePhiMax", &phiMax);
	t->Branch("angleThetaMin", &thetaMin);
	t->Branch("angleThetaMax", &thetaMax);
	t->Branch("planeSurfaceNormal[3]", surfaceNormal);
	t->Branch("planeShape", &shape);
	t->Branch("planeA", &a);
	t->Branch("planeB", &b);
	t->Branch("planeRMin", &rMin);
	t->Branch("planeRMax", &rMax);
	t->Branch("planePos[3]", pos);
	t->Fill();
	getFile()->WriteTObject(t);
}
