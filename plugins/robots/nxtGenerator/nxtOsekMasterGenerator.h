#pragma once

#include <masterGeneratorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace nxtOsek {

class NxtOsekMasterGenerator : public MasterGeneratorBase
{
public:
	NxtOsekMasterGenerator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);

	void generateOilAndMakeFiles();

protected:
	virtual GeneratorCustomizer *createCustomizer();
	virtual QString targetPath();

	virtual void beforeGeneration();
	virtual void afterGeneration();

private:
	void createProjectDir(QString const &projectDir);

	void generateOilFile(QString const &projectName, QString const &projectDir);
	void generateMakeFile(QString const &projectName, QString const &projectDir);
	void saveImages(QString const &projectDir);
};

}
}
}
}
