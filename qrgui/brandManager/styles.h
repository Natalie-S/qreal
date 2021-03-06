#pragma once

#include <QtWidgets/QApplication>

#include <qrutils/inFile.h>
#include "fonts.h"

namespace qReal {

/// Manages all json stylesheets in system
class Styles
{
public:
	explicit Styles(Fonts const &fonts)
		: mFonts(fonts)
	{
	}

	virtual ~Styles()
	{
	}

	/// Returns json stylesheet for start tab background
	virtual QString startTabBackgroundStyle() const
	{
		return utils::InFile::readAll(startTabBackgroundStylePath());
	}

	/// Returns json stylesheet for start tab substrate background
	virtual QString startTabSubstrateBackgroundStyle() const
	{
		return utils::InFile::readAll(startTabSubstrateBackgroundStylePath());
	}

	/// Returns json stylesheet for start tab header background
	virtual QString startTabHeaderBackgroundStyle() const
	{
		return utils::InFile::readAll(startTabHeaderBackgroundStylePath());
	}

	/// Returns json stylesheet for recent projects section background on start tab
	virtual QString startTabRecentProjectsBackgroundStyle() const
	{
		return utils::InFile::readAll(startTabRecentProjectsBackgroundStylePath());
	}

	/// Returns json stylesheet for projects management section background on start tab
	virtual QString startTabProjectsManagementBackgroundStyle() const
	{
		return utils::InFile::readAll(startTabProjectsManagementBackgroundStylePath());
	}

	/// Returns json stylesheet for command buttons on start tab
	virtual QString startTabButtonStyle() const
	{
		return utils::InFile::readAll(startTabButtonStylePath())
				.replace("@@FONT@@", mFonts.commandButtonsFont());
	}

	/// Returns json stylesheet for styled text on start tab  of level 1 heading
	virtual QString startTabLabelLevel1Style() const
	{
		return utils::InFile::readAll(startTabLabelLevel1StylePath())
				.replace("@@FONT@@", mFonts.styledTextFont());
	}

	/// Returns json stylesheet for styled text on start tab of level 2 heading
	virtual QString startTabLabelLevel2Style() const
	{
		return utils::InFile::readAll(startTabLabelLevel2StylePath())
				.replace("@@FONT@@", mFonts.styledTextFont());
	}

protected:
	/// Returns a path to a file with json stylesheet for start tab background
	virtual QString startTabBackgroundStylePath() const
	{
		return processUrl(":/styles/startTab/background.js");
	}

	/// Returns a path to a file with json stylesheet for start tab substrate background
	virtual QString startTabSubstrateBackgroundStylePath() const
	{
		return processUrl(":/styles/startTab/substrate.js");
	}

	/// Returns a path to a file with json stylesheet for start tab header background
	virtual QString startTabHeaderBackgroundStylePath() const
	{
		return processUrl(":/styles/startTab/header.js");
	}

	/// Returns a path to a file with json stylesheet for recent projects section background on start tab
	virtual QString startTabRecentProjectsBackgroundStylePath() const
	{
		return processUrl(":/styles/startTab/recentProjectsBackground.js");
	}

	/// Returns a path to a file with json stylesheet for projects management section background on start tab
	virtual QString startTabProjectsManagementBackgroundStylePath() const
	{
		return processUrl(":/styles/startTab/projectsManagementBackground.js");
	}

	/// Returns a path to a file with json stylesheet for command buttons on start tab
	virtual QString startTabButtonStylePath() const
	{
		return processUrl(":/styles/startTab/button.js");
	}

	/// Returns a path to a file with json stylesheet for styled text on start tab  of level 1 heading
	virtual QString startTabLabelLevel1StylePath() const
	{
		return processUrl(":/styles/startTab/labelLevel1.js");
	}

	/// Returns a path to a file with json stylesheet for styled text on start tab  of level 2 heading
	virtual QString startTabLabelLevel2StylePath() const
	{
		return processUrl(":/styles/startTab/labelLevel2.js");
	}

	/// Returns either given url without modifications or transforms it into absolute disk location
	/// for more convenient styles debugging (modifications do not need rebuilds then)
	QString processUrl(QString const &resourceUrl) const
	{
		// TODO: uncomment one of the next scenarious

		// Scenario one: use it for releases
		return resourceUrl;

		// Scenario two: use it for debugging
		// QString choppedString = resourceUrl;
		// choppedString.remove(0, 1);
		// return QApplication::applicationDirPath() + "/../qrgui/brandManager" + choppedString;
	}

	Fonts const &mFonts;
};

}

// Implemented in .h file for correct linkage
