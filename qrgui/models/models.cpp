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

    qDebug() << "init role" << SettingsManager::value("role").toInt();
    qDebug() << "init addr" << SettingsManager::value("lastServerAddress").toString();
//    roleChanged(0, SettingsManager::value("lastServerAddress").toString());

}

models::details::collaborativeDevelopment::Client *Models::getClient()
{
    return mClient;
}

models::details::collaborativeDevelopment::Server *Models::getServer()
{
    return mServer;
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
    switch(exRole) {
    case 1:
    {
        QObject::disconnect(mClient, 0, 0, 0);
//        QObject::disconnect(mLogicalModel, SIGNAL(smthChanged(QString,QString,QVariant&,int)), mClient, SLOT(onDataChanged(QString,QString,QVariant&,int)));
//        QObject::disconnect(mLogicalModel, SIGNAL(elementAdded(QString,QString,QString,QString,QString,QPointF)), mClient, SLOT(onElementAdded(QString,QString,QString,QString,QString,QPointF)));

//        QObject::disconnect(mGraphicalModel, SIGNAL(smthChanged(QString,QString,QVariant&,int)), mClient, SLOT(onDataChanged(QString,QString,QVariant&,int)));
//        QObject::disconnect(mGraphicalModel, SIGNAL(elementAdded(QString,QString,QString,QString,QString,QPointF)), mClient, SLOT(onElementAdded(QString,QString,QString,QString,QString,QPointF)));
        mClient->disconnectFromServer();
        //qDebug() << "I'm not a Client any more!";
    }
        break;
    case 2:
    {
        QObject::disconnect(mServer, 0, 0, 0);
//        QObject::disconnect(mLogicalModel, SIGNAL(smthChanged(QString,QString,QVariant,int)), mClient, SLOT(onDataChanged(QString,QString,QVariant,int)));
//        QObject::disconnect(mLogicalModel, SIGNAL(elementAdded(QString,QString,QString,QString,QString,QPointF)), mClient, SLOT(onElementAdded(QString,QString,QString,QString,QString,QPointF)));

//        QObject::disconnect(mGraphicalModel, SIGNAL(smthChanged(QString,QString,QVariant,int)), mClient, SLOT(onDataChanged(QString,QString,QVariant,int)));
//        QObject::disconnect(mGraphicalModel, SIGNAL(elementAdded(QString,QString,QString,QString,QString,QPointF)), mClient, SLOT(onElementAdded(QString,QString,QString,QString,QString,QPointF)));
        mServer->close();
        //qDebug() << "I'm not a Server any more!";
    }
        break;
    default:
        break;
    }
    int role = SettingsManager::value("role").toInt();
    qDebug() << "in roleC" << role;
    switch(role) {
    case 0:
        qDebug() << "Forever alone";
        break;
    case 1:
    {
        makeItClient(addr);
        emit roleWasSet(1);
    }
        break;
    case 2:
    {
        makeItServer();
        emit roleWasSet(2);
    }
        break;
    default:
        break;
    }
}

void Models::makeItClient(QString addr)
{
    qDebug() << "I'm a client!";
    mClient = new models::details::collaborativeDevelopment::Client();
    mClient->connectToServer(addr);

//    QObject::connect(mLogicalModel, SIGNAL(smthChanged(QString,QString,QVariant,int)),mClient, SLOT(onDataChanged(QString,QString,QVariant,int)));
//    QObject::connect(mLogicalModel, SIGNAL(elementAdded(QString,QString,QString,QString,QString,QPointF)), mClient, SLOT(onElementAdded(QString,QString,QString,QString,QString,QPointF)));

//    QObject::connect(mGraphicalModel, SIGNAL(smthChanged(QString,QString,QVariant,int)), mClient, SLOT(onDataChanged(QString,QString,QVariant,int)));
//    QObject::connect(mGraphicalModel, SIGNAL(elementAdded(QString,QString,QString,QString,QString,QPointF)), mClient, SLOT(onElementAdded(QString,QString,QString,QString,QString,QPointF)));
}

void Models::makeItServer()
{
    qDebug() << "I'm a server!";
    mServer = new models::details::collaborativeDevelopment::Server();
    mServer->listen();

//    QObject::connect(mServer, SIGNAL(logicalModelChanged(Id,QVariant,int)),mLogicalModel, SLOT(justSetData(Id,QVariant,int)));
//    QObject::connect(mServer, SIGNAL(logicalModelElementAdded(Id,Id,Id,QString,QPointF)), mLogicalModel, SLOT(justAddElementToModel(Id,Id,Id,QString,QPointF)));

//    QObject::connect(mServer, SIGNAL(graphicalModelChanged(Id,QVariant,int)), mGraphicalModel, SLOT(justSetData(Id,QVariant,int)));
//    QObject::connect(mServer, SIGNAL(graphicalModelElementAdded(Id,Id,Id,QString,QPointF)), mGraphicalModel, SLOT(justAddElementToModel(Id,Id,Id,QString,QPointF)));
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
