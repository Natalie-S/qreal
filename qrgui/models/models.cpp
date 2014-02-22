#include "models.h"

//#include "dialogs/preferencesPages/miscellaniousPage.h"

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

    qDebug() << "init role" << SettingsManager::value("role");
}

Models::~Models()
{
	delete mGraphicalModel;
	delete mLogicalModel;
	delete mRepoApi;
}

void Models::roleChanged(int exRole)
{
    switch(exRole) {
    case 1:
    {
        QObject::disconnect(mLogicalModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),mClient, SLOT(onDataChanged()));
        mClient->disconnectFromServer();
        qDebug() << "I'm not a Client any more!";
    }
        break;
    case 2:
    {
        mServer->close();
        qDebug() << "I'm not a Server any more!";
    }
        break;
    default:
        break;
    }
    int role = SettingsManager::value("role").toInt();
    qDebug() << "roleChanged " << role;
    switch(role) {
    case 0:
        break;
    case 1:
        makeItClient();
        break;
    case 2:
        makeItServer();
        break;
    default:
        break;
    }
}

void Models::makeItClient()
{
    qDebug() << "I'm a client!";
    mClient = new Client();
    mClient->connectToServer();
    QObject::connect(mLogicalModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),mClient, SLOT(onDataChanged()));
}

void Models::makeItServer()
{
    qDebug() << "I'm a server!";
    mServer = new Server();
    mServer->listen();
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
