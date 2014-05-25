#include "proxyEditorManager.h"

using namespace qReal;

ProxyEditorManager::ProxyEditorManager(EditorManagerInterface *editorManagerInterface)
		: mProxiedEditorManager(editorManagerInterface)
{    
}

ProxyEditorManager::~ProxyEditorManager()
{
	delete mProxiedEditorManager;
}

IdList ProxyEditorManager::editors() const
{
	return mProxiedEditorManager->editors();
}

IdList ProxyEditorManager::diagrams(Id const &editor) const
{
	return mProxiedEditorManager->diagrams(editor);
}

IdList ProxyEditorManager::elements(Id const &diagram) const
{
	return mProxiedEditorManager->elements(diagram);
}

bool ProxyEditorManager::loadPlugin(QString const &pluginName)
{
	return mProxiedEditorManager->loadPlugin(pluginName);
}

bool ProxyEditorManager::unloadPlugin(QString const &pluginName)
{
	return mProxiedEditorManager->unloadPlugin(pluginName);
}

QString ProxyEditorManager::mouseGesture(Id const &id) const
{
	return mProxiedEditorManager->mouseGesture(id);
}

QString ProxyEditorManager::friendlyName(Id const &id) const
{
	return mProxiedEditorManager->friendlyName(id);
}

QString ProxyEditorManager::description(Id const &id) const
{
	return mProxiedEditorManager->description(id);
}

QString ProxyEditorManager::propertyDescription(Id const &id, QString const &propertyName) const
{
	return mProxiedEditorManager->propertyDescription(id, propertyName);
}

QString ProxyEditorManager::propertyDisplayedName(Id const &id, QString const &propertyName) const
{
	return mProxiedEditorManager->propertyDisplayedName(id, propertyName);
}

QIcon ProxyEditorManager::icon(Id const &id) const
{
	return mProxiedEditorManager->icon(id);
}

ElementImpl *ProxyEditorManager::elementImpl(Id const &id) const
{
	return mProxiedEditorManager->elementImpl(id);
}

IdList ProxyEditorManager::containedTypes(const Id &id) const
{
	return mProxiedEditorManager->containedTypes(id);
}

QStringList ProxyEditorManager::enumValues(Id const &id, const QString &name) const
{
	return mProxiedEditorManager->enumValues(id, name);
}

QString ProxyEditorManager::typeName(Id const &id, const QString &name) const
{
	return mProxiedEditorManager->typeName(id, name);
}

QStringList ProxyEditorManager::allChildrenTypesOf(Id const &parent) const
{
	return mProxiedEditorManager->allChildrenTypesOf(parent);
}

QList<Explosion> ProxyEditorManager::explosions(Id const &source) const
{
	return mProxiedEditorManager->explosions(source);
}

bool ProxyEditorManager::isEditor(Id const &id) const
{
	return mProxiedEditorManager->isEditor(id);
}

bool ProxyEditorManager::isDiagram(Id const &id) const
{
	return mProxiedEditorManager->isDiagram(id);
}

bool ProxyEditorManager::isElement(Id const &id) const
{
	return mProxiedEditorManager->isElement(id);
}

QStringList ProxyEditorManager::propertyNames(Id const &id) const
{
	return mProxiedEditorManager->propertyNames(id);
}

QStringList ProxyEditorManager::portTypes(Id const &id) const
{
	return mProxiedEditorManager->portTypes(id);
}

QString ProxyEditorManager::defaultPropertyValue(Id const &id, QString name) const
{
	return mProxiedEditorManager->defaultPropertyValue(id, name);
}

QStringList ProxyEditorManager::propertiesWithDefaultValues(Id const &id) const
{
	return mProxiedEditorManager->propertiesWithDefaultValues(id);
}

IdList ProxyEditorManager::checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
		, qrRepo::GraphicalRepoApi const &graphicalApi) const
{
	return mProxiedEditorManager->checkNeededPlugins(logicalApi, graphicalApi);
}

bool ProxyEditorManager::hasElement(Id const &element) const
{
	return mProxiedEditorManager->hasElement(element);
}

Id ProxyEditorManager::findElementByType(QString const &type) const
{
	return mProxiedEditorManager->findElementByType(type);
}

QList<ListenerInterface *> ProxyEditorManager::listeners() const
{
	return mProxiedEditorManager->listeners();
}

bool ProxyEditorManager::isDiagramNode(Id const &id) const
{
	return mProxiedEditorManager->isDiagramNode(id);
}

bool ProxyEditorManager::isParentOf(Id const &child, Id const &parent) const
{
	return mProxiedEditorManager->isParentOf(child, parent);
}

bool ProxyEditorManager::isGraphicalElementNode(const Id &id) const
{
	return mProxiedEditorManager->isGraphicalElementNode(id);
}

Id ProxyEditorManager::theOnlyDiagram() const
{
	return mProxiedEditorManager->theOnlyDiagram();
}

QString ProxyEditorManager::diagramNodeNameString(Id const &editor, Id const &diagram) const
{
	return mProxiedEditorManager->diagramNodeNameString(editor, diagram);
}

QList<StringPossibleEdge> ProxyEditorManager::possibleEdges(QString const &editor, QString const &element) const
{
	return mProxiedEditorManager->possibleEdges(editor, element);
}

QStringList ProxyEditorManager::elements(QString const &editor, QString const &diagram) const
{
	return mProxiedEditorManager->elements(editor, diagram);
}

int ProxyEditorManager::isNodeOrEdge(QString const &editor, QString const &element) const
{
	return mProxiedEditorManager->isNodeOrEdge(editor, element);
}

bool ProxyEditorManager::isParentOf(QString const &editor, QString const &parentDiagram
		, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	return mProxiedEditorManager->isParentOf(editor, parentDiagram, parentElement, childDiagram, childElement);
}

QString ProxyEditorManager::diagramName(QString const &editor, QString const &diagram) const
{
	return mProxiedEditorManager->diagramName(editor, diagram);
}

QString ProxyEditorManager::diagramNodeName(QString const &editor, QString const &diagram) const
{
	return mProxiedEditorManager->diagramNodeName(editor, diagram);
}

void ProxyEditorManager::setProxyManager(EditorManagerInterface *editorManagerInterface)
{
	delete mProxiedEditorManager;
	mProxiedEditorManager = editorManagerInterface;
}

bool ProxyEditorManager::isInterpretationMode() const
{
	return mProxiedEditorManager->isInterpretationMode();
}

bool ProxyEditorManager::isParentProperty(Id const &id, QString const &propertyName) const
{
	return mProxiedEditorManager->isParentProperty(id, propertyName);
}

void ProxyEditorManager::deleteProperty(QString const &propDisplayedName)
{
    QObject::connect(mProxiedEditorManager, SIGNAL(metaModelChanged(QString)), this, SIGNAL(metaModelChanged(QString)));
	mProxiedEditorManager->deleteProperty(propDisplayedName);
//    QStringList params;
//    params << "delProp" << propDisplayedName;
//    emit metaModelChanged(params.join("|") + "|");
//    qDebug() << "proxy:    " << params.join("|") + "|";
}

void ProxyEditorManager::addProperty(Id const &id, QString const &propDisplayedName)
{
    QObject::connect(mProxiedEditorManager, SIGNAL(metaModelChanged(QString)), this, SIGNAL(metaModelChanged(QString)));
	mProxiedEditorManager->addProperty(id, propDisplayedName);
//    QStringList params;
//    params << "addProp" << id.toString() << propDisplayedName;
//    emit metaModelChanged(params.join("|") + "|");
}

void ProxyEditorManager::updateProperties(Id const &id, QString const &property, QString const &propertyType
        , QString const &propertyDefaultValue, QString const &propertyDisplayedName)
{
    QObject::connect(mProxiedEditorManager, SIGNAL(metaModelChanged(QString)), this, SIGNAL(metaModelChanged(QString)));
	mProxiedEditorManager->updateProperties(id, property, propertyType, propertyDefaultValue, propertyDisplayedName);
//    QStringList params;
//    params << "updProp" << id.toString() << property << propertyType << propertyDefaultValue << propertyDisplayedName;
//    emit metaModelChanged(params.join("|") + "|");
}

QString ProxyEditorManager::propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const
{
	return mProxiedEditorManager->propertyNameByDisplayedName(id, displayedPropertyName);
}

IdList ProxyEditorManager::children(Id const &parent) const
{
	return mProxiedEditorManager->children(parent);
}

QString ProxyEditorManager::shape(Id const &id) const
{
	return mProxiedEditorManager->shape(id);
}

void ProxyEditorManager::updateShape(Id const &id, QString const &graphics)
{
    QObject::connect(mProxiedEditorManager, SIGNAL(metaModelChanged(QString)), this, SIGNAL(metaModelChanged(QString)));
	mProxiedEditorManager->updateShape(id, graphics);    
//    QStringList params;
//    params << "updShape" << id.toString() << graphics;
//    emit metaModelChanged(params.join("|") + "|");
//    qDebug() << "proxy" << params.join("|") + "|";
}

void ProxyEditorManager::deleteElement(MainWindow *mainWindow, Id const &id)
{
	mProxiedEditorManager->deleteElement(mainWindow, id);
    QStringList params;
    params << "delElem" << id.toString();
    emit metaModelChanged(params.join("|") + "|");
}

bool ProxyEditorManager::isRootDiagramNode(Id const &id) const
{
	return mProxiedEditorManager->isRootDiagramNode(id);
}

void ProxyEditorManager::addEdgeElementFromClient(Id const &diagram, QString const &name, QString const &labelText
                              , QString const &labelType, QString const &lineType
                              , QString const &beginType, QString const &endType
                              , Id const &edgeId, Id const &associationId)
{
   mProxiedEditorManager->addEdgeElementFromClient(diagram, name, labelText, labelType, lineType, beginType, endType, edgeId, associationId);

}

void ProxyEditorManager::addNodeElementFromClient(Id const &diagram, QString const &name, bool isRootDiagramNode, Id const &nodeId)
{
    mProxiedEditorManager->addNodeElementFromClient(diagram, name, isRootDiagramNode, nodeId);
}

QString ProxyEditorManager::addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode)
{
    QString res = mProxiedEditorManager->addNodeElement(diagram, name, isRootDiagramNode);
    QStringList params;
    params << "addNode" << diagram.toString() << name << (isRootDiagramNode ? "t" : "f") << res;
    emit metaModelChanged(params.join("|") + "|");
    return res;
}

QString ProxyEditorManager::addEdgeElement(Id const &diagram, QString const &name, QString const &labelText
        , QString const &labelType, QString const &lineType, QString const &beginType, QString const &endType)
{
    QString res = mProxiedEditorManager->addEdgeElement(diagram, name, labelText, labelType, lineType, beginType, endType);
    QStringList params;
    params << "addEdge" << diagram.toString() <<  name << labelText << labelType << lineType << beginType << endType << res;
    emit metaModelChanged(params.join("|") + "|");
    return res;
}

QPair<Id, Id> ProxyEditorManager::createEditorAndDiagram(QString const &name)
{
    QObject::connect(mProxiedEditorManager, SIGNAL(metaModelChanged(QString)), this, SIGNAL(metaModelChanged(QString)));
	return mProxiedEditorManager->createEditorAndDiagram(name);
}

QPair<Id, Id> ProxyEditorManager::createEditorAndDiagramFromClient(QString const &name, Id const &editor, Id const &diagram, Id const nodeId, Id const containerLink)
{
    return mProxiedEditorManager->createEditorAndDiagramFromClient(name, editor, diagram, nodeId, containerLink);
}

void ProxyEditorManager::saveMetamodel(QString const &newMetamodelFileName)
{
	mProxiedEditorManager->saveMetamodel(newMetamodelFileName);
}

QString ProxyEditorManager::saveMetamodelFilePath() const
{
	return mProxiedEditorManager->saveMetamodelFilePath();
}

QStringList ProxyEditorManager::paletteGroups(Id const &editor, Id const &diagram) const
{
	return mProxiedEditorManager->paletteGroups(editor, diagram);
}

QStringList ProxyEditorManager::paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const
{
	return mProxiedEditorManager->paletteGroupList(editor, diagram, group);
}

QString ProxyEditorManager::paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const
{
	return mProxiedEditorManager->paletteGroupDescription(editor, diagram, group);
}

bool ProxyEditorManager::shallPaletteBeSorted(Id const &editor, Id const &diagram) const
{
	return mProxiedEditorManager->shallPaletteBeSorted(editor, diagram);
}

QStringList ProxyEditorManager::referenceProperties(Id const &id) const
{
	return mProxiedEditorManager->referenceProperties(id);
}

IdList ProxyEditorManager::groups(Id const &diagram)
{
	return mProxiedEditorManager->groups(diagram);
}

Pattern ProxyEditorManager::getPatternByName (QString const &str) const
{
	return mProxiedEditorManager->getPatternByName(str);
}

QList<QString> ProxyEditorManager::getPatternNames() const
{
	return mProxiedEditorManager->getPatternNames();
}

QSize ProxyEditorManager::iconSize(Id const &id) const
{
	return mProxiedEditorManager->iconSize(id);
}

EditorManagerInterface *ProxyEditorManager::getEditorManager()
{
    return mProxiedEditorManager;
}
