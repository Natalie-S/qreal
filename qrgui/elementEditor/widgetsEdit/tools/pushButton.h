#pragma once

#include <QtGui/QPushButton>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class PushButtonProxy : public AbstractButtonProxy
{
	Q_OBJECT

	Q_PROPERTY(bool flat READ isFlat WRITE setFlat USER true DESIGNABLE true)

public:
	explicit PushButtonProxy(QPushButton *pushButton);

private:
	bool isFlat() const;
	void setFlat(bool flat);

	QPushButton *mPushButton;
};

class PushButton : public AbstractButton
{
	Q_OBJECT

public:
	explicit PushButton(ToolController *controller);

private:
	QPushButton *mPushButton;
};

}
}