#pragma once

#include "models/details/graphicalModel.h"
#include "models/details/logicalModel.h"
#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "models/details/collaborativeDevelopement/client.h"

using namespace qReal;
using namespace models;

namespace qReal {
namespace models {

class Models : public QObject
{
    Q_OBJECT
public:

    explicit Models(QString const &workingCopy, EditorManagerInterface &editorManager, QObject *parent = 0);
    ~Models();

    QAbstractItemModel *graphicalModel() const;
    QAbstractItemModel *logicalModel() const;

    GraphicalModelAssistApi &graphicalModelAssistApi() const;
    LogicalModelAssistApi &logicalModelAssistApi() const;

    qrRepo::RepoControlInterface &repoControlApi() const;

    qrRepo::LogicalRepoApi const &logicalRepoApi() const;
    qrRepo::LogicalRepoApi &mutableLogicalRepoApi() const;

    qrRepo::GraphicalRepoApi const &graphicalRepoApi() const;

    void reinit();
    void makeItClient(QString addr);
    models::details::collaborativeDevelopment::Client *getClient();
    models::details::GraphicalModel *getGraphicalModel();
    models::details::LogicalModel *getLogicalModel();
signals:
    void roleWasSet(int role);

public slots:
    void roleChanged(int exRole, QString addr);

private:
    models::details::GraphicalModel *mGraphicalModel;
    models::details::GraphicalPartModel *mGraphicalPartModel;
    models::details::LogicalModel *mLogicalModel;
    qrRepo::RepoControlInterface *mRepoApi;
    models::details::collaborativeDevelopment::Client *mClient = NULL;
};

}
}
