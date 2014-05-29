#include "models.h"

using namespace qReal;
using namespace models;

Models::Models(QString const &workingCopy, EditorManagerInterface &editorManager, QObject *parent) : QObject(parent)
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
}

models::details::collaborativeDevelopment::Client *Models::getClient()
{
    return mClient;
}

models::details::GraphicalModel *Models::getGraphicalModel()
{
    return mGraphicalModel;
}

models::details::LogicalModel *Models::getLogicalModel()
{
    return mLogicalModel;
}

Models::~Models()
{
    delete mGraphicalModel;
    delete mLogicalModel;
    delete mRepoApi;
}

void Models::roleChanged(int exRole, QString addr)
{
    int role = SettingsManager::value("role").toInt();
    if (exRole == 1 && role == 0) {
        mClient->onDisconnected();
    }
    if (role == 0) {
        qDebug() << "Forever alone";
    } else if (role == 1) {
        makeItClient(addr);
    }
}

void Models::makeItClient(QString addr)
{
    qDebug() << "I'm a client!";
    mClient = new models::details::collaborativeDevelopment::Client();
    mClient->connectToServer(addr);
    emit roleWasSet(1);
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
