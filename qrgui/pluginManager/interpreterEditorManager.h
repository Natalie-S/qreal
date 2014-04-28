#pragma once

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtGui/QIcon>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>
#include <qrrepo/graphicalRepoApi.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrrepo/repoApi.h>

#include "pluginManager/listenerManager.h"
#include "editorPluginInterface/editorInterface.h"
#include "pluginManager/editorManagerInterface.h"

namespace qReal {

class Element;

class InterpreterEditorManager : public EditorManagerInterface
{
	Q_OBJECT

public:
	explicit InterpreterEditorManager(QString const &fileName, QObject *parent = NULL);
	~InterpreterEditorManager();

	IdList editors() const override;
	IdList diagrams(Id const &editor) const override;
	IdList elements(Id const &diagram) const override;
	bool loadPlugin(QString const &pluginName) override;
	bool unloadPlugin(QString const &pluginName) override;

	QString mouseGesture(Id const &id) const override;
	QString friendlyName(Id const &id) const override;
	QString description(Id const &id) const override;
	QString propertyDescription(Id const &id, QString const &propertyName) const override;
	QString propertyDisplayedName(Id const &id, QString const &propertyName) const override;
	QIcon icon(Id const &id) const override;
	ElementImpl* elementImpl(Id const &id) const override;

	IdList containedTypes(const Id &id) const override;
	QStringList enumValues(Id const &id, const QString &name) const override;
	QString typeName(Id const &id, const QString &name) const override;
	QStringList allChildrenTypesOf(Id const &parent) const override;

	QList<Explosion> explosions(Id const &source) const override;

	bool isEditor(Id const &id) const override;
	bool isDiagram(Id const &id) const override;
	bool isElement(Id const &id) const override;

	QStringList propertyNames(Id const &id) const override;
	QStringList portTypes(Id const &id) const override;
	QString defaultPropertyValue(Id const &id, QString name) const override;
	QStringList propertiesWithDefaultValues(Id const &id) const override;

	IdList checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
			, qrRepo::GraphicalRepoApi const &graphicalApi) const override;
	bool hasElement(Id const &element) const override;

	Id findElementByType(QString const &type) const override;
	QList<ListenerInterface *> listeners() const override;

	bool isDiagramNode(Id const &id) const override;

	bool isParentOf(Id const &child, Id const &parent) const override;
	bool isGraphicalElementNode(const Id &id) const override;

	/// Returns diagram id if only one diagram loaded or Id() otherwise
	Id theOnlyDiagram() const override;
	QString diagramNodeNameString(Id const &editor, Id const &diagram) const override;

	QList<StringPossibleEdge> possibleEdges(QString const &editor, QString const &elementName) const override;
	QStringList elements(QString const &editor, QString const &diagram) const override;
	int isNodeOrEdge(QString const &editor, QString const &element) const override;
	bool isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement
			, QString const &childDiagram, QString const &childElement) const override;
	QString diagramName(QString const &editor, QString const &diagram) const override;
	QString diagramNodeName(QString const &editor, QString const &diagram) const override;
	bool isInterpretationMode() const override;

	bool isParentProperty(Id const &id, QString const &propertyName) const override;
//    void deleteProperty(QString const &propDisplayedName) override;
//    void addProperty(Id const &id, QString const &propDisplayedName) override;
//	void updateProperties(Id const &id, QString const &property, QString const &propertyType
//            , QString const &propertyDefaultValue, QString const &propertyDisplayedName) override;
	QString propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const override;
	IdList children(Id const &parent) const override;
	QString shape(Id const &id) const override;
//    void updateShape(Id const &id, QString const &graphics) override;
//    void deleteElement(qReal::MainWindow *mainWindow, Id const &id) override;
	bool isRootDiagramNode(Id const &id) const override;
//    void addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) override;

//	void addEdgeElement(
//			Id const &diagram
//			, QString const &name
//			, QString const &labelText
//			, QString const &labelType
//			, QString const &lineType
//			, QString const &beginType
//			, QString const &endType
//            ) override;

	void saveMetamodel(QString const &newMetamodelFileName) override;
	QString saveMetamodelFilePath() const override;
	QStringList paletteGroups(Id const &editor, Id const &diagram) const override;
	QStringList paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const override;
	QString paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const override;
	bool shallPaletteBeSorted(Id const &editor, Id const &diagram) const override;
	QStringList referenceProperties(Id const &id) const override;
	IdList groups(Id const &diagram) override;
	Pattern getPatternByName (QString const &str) const override;
	QList<QString> getPatternNames() const override;
    QSize iconSize(Id const &id) const override;

//public slots:
    QPair<Id, Id> createEditorAndDiagram(QString const &name) override;
    QMap<QString, qrRepo::RepoApi*> getEditorRepoApi();


private:
	class CheckPropertyForParent;
	class CompareProperty;
	class GetProperty;
	class HasProperty;

	QMap<QString, qrRepo::RepoApi*> mEditorRepoApi;  // Has ownership.
	QString mMetamodelFile;

	void setProperty(qrRepo::RepoApi* repo, Id const &id, QString const &property, QVariant const &propertyValue) const;
	Id element(Id const &id, qrRepo::RepoApi const * const repo, Id const &diagram) const;
	Id diagramOrElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &editor) const;
	void setStandartConfigurations(qrRepo::RepoApi *repo, Id const &id, Id const &parent, const QString &name) const;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId(Id const &id) const;
	QPair<qrRepo::RepoApi*, Id> repoAndElement(QString const &editor, QString const &element) const;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram(QString const &editor, QString const &diagram) const;
	QPair<Id, Id> editorAndDiagram(qrRepo::RepoApi const * const repo, Id const &element) const;
	QStringList propertiesFromParents(Id const &id, QString const &propertyName
			, CheckPropertyForParent const &checker) const;
	QString valueOfProperty(Id const &id, QString const &propertyName, QString const &value) const;
	void deletePropertyInElement(qrRepo::RepoApi *repo, Id const &editor, Id const &diagram
			, QString const &propDisplayedName) const;
//signals:
//    void nodeAdded(QString const &diagram, QString const &name, bool isDiagramNode);
//    void edgeAdded(
//            QString const &diagram
//            , QString const &name
//            , QString const &labelText
//            , QString const &labelType
//            , QString const &lineType
//            , QString const &beginType
//            , QString const &endType
//            );

//    void propertyDeleted(QString const &propDisplayedName);
//    void propertyAdded(QString const &id, QString const &propDisplayedName);
//    void propertyUpdated(QString const &id, QString const &property, QString const &propertyType
//            , QString const &propertyDefaultValue, QString const &propertyDisplayedName);

//    void shapeUpdated(QString const &id, QString const &graphics);
////	void elementDeleted(qReal::MainWindow *mainWindow, Id const &id);



public slots:
//    void onPropDeleted(QString const &propDisplayedName);
//    void onPropAdded(Id const &id, QString const &propDisplayedName);
//    void onPropUpdated(Id const &id, QString const &property, QString const &propertyType
//            , QString const &propertyDefaultValue, QString const &propertyDisplayedName);
//    void onShapeUpdated(Id const &id, QString const &graphics);
//    //void deleteElement(qReal::MainWindow *mainWindow, Id const &id) override;
//    void onNodeAdded(Id const &diagram, QString const &name, bool isRootDiagramNode);

//    void onEdgeAdded(
//            Id const &diagram
//            , QString const &name
//            , QString const &labelText
//            , QString const &labelType
//            , QString const &lineType
//            , QString const &beginType
//            , QString const &endType
//            );
    void deleteProperty(QString const &propDisplayedName) override;
    void addProperty(Id const &id, QString const &propDisplayedName) override;
    void updateProperties(Id const &id, QString const &property, QString const &propertyType
            , QString const &propertyDefaultValue, QString const &propertyDisplayedName) override;
    void updateShape(Id const &id, QString const &graphics) override;
    void deleteElement(MainWindow *mainWindow, Id const &id) override;
    void addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) override;
    void addEdgeElement(Id const &diagram, QString const &name, QString const &labelText
            , QString const &labelType, QString const &lineType, QString const &beginType
            , QString const &endType) override;
//    void addNodeElementFromClient(Id const &diagram, QString const &name, bool isRootDiagramNode) override;
//    void addEdgeElementFromClient(Id const &diagram, QString const &name, QString const &labelText
//                , QString const &labelType, QString const &lineType
//                , QString const &beginType, QString const &endType) override;
};

}
