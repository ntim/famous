/*
 * CoreRunAction.hh
 *
 * @date Jul 31, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef CORERUNACTION_HH_
#define CORERUNACTION_HH_

#include <string>
#include <G4UserRunAction.hh>
#include <G4Timer.hh>

#include "CorePersistencyHandler.hh"

class CoreRunAction: public G4UserRunAction {
private:
	G4Timer timer;
	std::string filename;
	CorePersistencyHandler* persistencyHandler;

protected:
	void setPersistencyHandler(CorePersistencyHandler* persistencyHandler);

public:
	/**
	 * Constructor.
	 *
	 * @param filename - the filename of the output file.
	 */
	CoreRunAction(std::string filename, CoreRootPersistVisitor* visitor = new CoreRootPersistVisitor);
	virtual ~CoreRunAction();

	virtual void BeginOfRunAction(const G4Run* run);
	virtual void EndOfRunAction(const G4Run* run);

	/**
	 * @return string - the filename.
	 */
	std::string getFilename() const;
	/**
	 * @return - the persistency handler.
	 */
	CorePersistencyHandler* getPersistencyHandler() const;
};

#endif /* CORERUNACTION_HH_ */
