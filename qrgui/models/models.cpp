#include "models.h"

using namespace qReal;
using namespace models;

Models::Models(QString const &workingCopy, EditorManagerInterface &editorManager)
{
	qrRepo::RepoApi *repoApi = new qrRepo::RepoApi(workingCopy);
	mGraphicalModel = new models::details::GraphicalModel(repoApi, editorManager);
	mGraphicalPartModel = new models::details::GraphicalPartModel(*repoApi, *mGraphicalModel);

	GraphicalModelAssistApi * const graphicalAssistApi
			= new GraphicalModelAssistApi(*mGraphicalModel, *mGraphicalPartModel, editorManager);

	mGraphicalModel->setAssistApi(graphicalAssistApi);

	QObject::connect(mGraphicalModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int))
			, mGraphicalPartModel, SLOT(rowsAboutToBeRemovedInGraphicalModel(QModelIndex, int, int)));

	mLogicalModel = new models::details::LogicalModel(repoApi, editorManager);
	mRepoApi = repoApi;

	mLogicalModel->connectToGraphicalModel(mGraphicalModel);
	mGraphicalModel->connectToLogicalModel(mLogicalModel);

    QMessageBox msgBox;
    msgBox.setText("Choose the role");
    msgBox.setInformativeText("By default Server role will be chosen");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
       case QMessageBox::Ok:
    {
        mServer = new Server();
        mServer->listen();
    }
        break;
       case QMessageBox::Cancel:
    {
        mClient = new Client();
        mClient->connectToServer();
        QObject::connect(mLogicalModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),mClient, SLOT(onDataChanged()));
    }
           break;
       default:
           // should never be reached
           break;
     }

    /*mClient = new Client();
    mClient->connectToServer();

    QObject::connect(mLogicalModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),mClient, SLOT(onDataChanged()));*/
}

Models::~Models()
{
	delete mGraphicalModel;
	delete mLogicalModel;
	delete mRepoApi;
}

QAbstractItemModel* Models::graphicalModel() const
{
	return mGraphicalModel;
}

QAbstractItemModel* Models::logicalModel() const
{
	return mLogicalModel;
}

GraphicalModelAssistApi &Models::graphicalModelAssistApi() const
{
	return mGraphicalModel->graphicalModelAssistApi();
}

LogicalModelAssistApi &Models::logicalModelAssistApi() const
{
	return mLogicalModel->logicalModelAssistApi();
}

qrRepo::RepoControlInterface &Models::repoControlApi() const
{
	return *mRepoApi;
}

qrRepo::LogicalRepoApi const &Models::logicalRepoApi() const
{
	return mLogicalModel->api();
}

qrRepo::LogicalRepoApi &Models::mutableLogicalRepoApi() const
{
	return mLogicalModel->mutableApi();
}

qrRepo::GraphicalRepoApi const &Models::graphicalRepoApi() const
{
	return mGraphicalModel->api();
}

void Models::reinit()
{
	mLogicalModel->reinit();
	mGraphicalModel->reinit();
	mGraphicalPartModel->reinit();
}
