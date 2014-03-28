#pragma once

#include "models/details/graphicalModel.h"
#include "models/details/logicalModel.h"
#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "models/details/collaborativeDevelopement/client.h"
#include "models/details/collaborativeDevelopement/server.h"
//#include "models/details/client.h"
//#include "models/details/server.h"

namespace qReal {
namespace models {

class Models
{
public:
    explicit Models(QString const &workingCopy, EditorManagerInterface &editorManager);
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
    void makeItServer();

public slots:
    void roleChanged(int exRole, QString addr);
//    void onElementAdded(Id const &parent, Id const &id,Id const &logicalId, QString const &name
//                        , QPointF const &position);

private:
    models::details::GraphicalModel *mGraphicalModel;
    models::details::GraphicalPartModel *mGraphicalPartModel;
    models::details::LogicalModel *mLogicalModel;
    qrRepo::RepoControlInterface *mRepoApi;
    Client *mClient;
    Server *mServer;
};

}
}
