// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#include "composerscene.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMimeData>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "execution/composerscheduler.h"
#include "execution/executorsettings.h"
#include "model/composermodel.h"
#include "model/connection.h"
#include "model/node.h"
#include "processor/processorsfactory.h"
#include "gui/command/editconnectioncommand.h"
#include "gui/command/movenodecommand.h"
#include "gui/command/createnodecommand.h"
#include "gui/command/removenodecommand.h"
#include "gui/command/editvaluecommand.h"
#include "gui/genericnodeitem.h"
#include "gui/customitems.h"
#include "gui/connectionitem.h"
#include "gui/plugitem.h"
#include "plugwidget/abstractplugwidget.h"
#include "global/parser.h"


ComposerScene::ComposerScene(QUndoStack *commandsStack, QObject *parent) :
    QGraphicsScene(parent),
    _commandsStack(commandsStack),
    _model(new ComposerModel(this)),
    _scheduler(new ComposerScheduler(_model, this)),
    _editedConnection(),
    _editedNode(),
    _connections()
{
    init();

    _scheduler->start();
}

ComposerScene::ComposerScene(const QDomDocument &doc,
                             QMainWindow *mainWindow,
                             QUndoStack *commandsStack,
                             QObject *parent) :
    QGraphicsScene(parent),
    _commandsStack(commandsStack),
    _model(new ComposerModel(this)),
    _scheduler(new ComposerScheduler(_model, this)),
    _editedConnection(),
    _editedNode(),
    _connections()
{
    init();

    load(doc, mainWindow);

    _scheduler->start();
}

void ComposerScene::init()
{
    _editedConnection.item = nullptr;
    _editedConnection.plugInput = nullptr;
    _editedConnection.plugOutput = nullptr;

    _editedNode.item = nullptr;

    connect(_model, &ComposerModel::nodeRemoved,       this, &ComposerScene::onNodeRemoved);
    connect(_model, &ComposerModel::connectionAdded,   this, &ComposerScene::onConnectionAdded);
    connect(_model, &ComposerModel::connectionRemoved, this, &ComposerScene::onConnectionRemoved);
    connect(_scheduler, &ComposerScheduler::executorStarted,  this, &ComposerScene::onExecutionStarted);
    connect(_scheduler, &ComposerScheduler::executorProgress, this, &ComposerScene::onExecutionProgress);
    connect(_scheduler, &ComposerScheduler::executorEnded,    this, &ComposerScene::onExecutionEnded);
    connect(_scheduler, &ComposerScheduler::nodeInvalid,      this, &ComposerScene::onNodeInvalid);

    setBackgroundBrush(QColor(0x27, 0x30, 0x35));
}

const QList<GenericNodeItem *> &ComposerScene::getNodes() const
{
    return _nodes;
}

GenericNodeItem *ComposerScene::findNode(const QUuid &uid) const
{
    for(GenericNodeItem *item : _nodes)
    {
        if(item->getNode()->getUid() == uid)
        {
            return item;
        }
    }

    return nullptr;
}

const QList<ConnectionItem *> &ComposerScene::getConnections() const
{
    return _connections;
}

const ComposerScheduler *ComposerScene::getScheduler() const
{
    return _scheduler;
}

ComposerScheduler *ComposerScene::accessScheduler()
{
    return _scheduler;
}

GenericNodeItem *ComposerScene::addNode(const QString &nodeName, const QUuid &uid)
{
    Node *node = new Node(nodeName, ProcessorsFactory::toUserReadableName(nodeName), uid);

    GenericNodeItem *item = new GenericNodeItem(node);
    addItem(item);
    _nodes << item;

    connect(item, &GenericNodeItem::plugValueChanged, this, &ComposerScene::onPlugValueChanged);
    connect(node, &Node::propertyChanged, this, &ComposerScene::onNodePropertyChanged);

    // Give the node to the model only now so that we are ready to receive events
    _model->addNode(node);

    for(PlugItem *plugItem : item->getInputs() + item->getOutputs())
    {
        connect(plugItem, &PlugItem::positionChanged, this, &ComposerScene::onPlugItemPositionChanged);
    }

    return item;
}

void ComposerScene::save(QDomDocument &doc, QMainWindow *mainWindow) const
{
    QDomElement rootNode = doc.createElement(QCoreApplication::applicationName().toLower());

    // Nodes
    for(GenericNodeItem *nodeItem : getNodes())
    {
        QDomElement nodeElement = doc.createElement("node");
        saveNode(doc, nodeElement, nodeItem);
        rootNode.appendChild(nodeElement);
    }

    // Connections
    for(ConnectionItem *connectionItem : getConnections())
    {
        const Connection *connection = connectionItem->getConnection();
        const Plug *outputPlug = connection->getOutput();
        const Plug *inputPlug = connection->getInput();

        Node *outputNode = _model->findOutputPlug(outputPlug);
        Node *inputNode = _model->findInputPlug(inputPlug);

        if(outputNode && inputNode)
        {
            QDomElement connectionElement = doc.createElement("connection");
            connectionElement.setAttribute("output_uid", outputNode->getUid().toString());
            connectionElement.setAttribute("output_plug", outputPlug->getDefinition().name);
            connectionElement.setAttribute("input_uid", inputNode->getUid().toString());
            connectionElement.setAttribute("input_plug", inputPlug->getDefinition().name);
            rootNode.appendChild(connectionElement);
        }
    }

    // GUI
    QDomElement mainWindowNode = doc.createElement("mainwindow");
    mainWindowNode.setAttribute("state", QString::fromUtf8(mainWindow->saveState().toHex()));
    mainWindowNode.setAttribute("geometry", QString::fromUtf8(mainWindow->saveGeometry().toHex()));
    rootNode.appendChild(mainWindowNode);

    // Executor settings
    QDomElement executorSettingsNode = doc.createElement("executor-settings");
    const ExecutorSettings &settings = _scheduler->getSettings();

    executorSettingsNode.setAttribute("cache-data",
                                      Parser::toStringBool(settings.cacheData));
    executorSettingsNode.setAttribute("use-multithreading",
                                      Parser::toStringBool(settings.useMultiThreading));
    executorSettingsNode.setAttribute("optimal-threads-count",
                                      Parser::toStringBool(settings.useOptimalThreadsCount));
    executorSettingsNode.setAttribute("fixed-threads-count",
                                      QString::number(settings.fixedThreadsCount));

    rootNode.appendChild(executorSettingsNode);

    // Finalize
    doc.appendChild(rootNode);
}

void ComposerScene::end()
{
    connect(_scheduler, &ComposerScheduler::ended, this, &ComposerScene::ended);
    _scheduler->end();
}

void ComposerScene::load(const QDomDocument &doc, QMainWindow *mainWindow)
{
    QDomNode mainNode = doc.namedItem(QCoreApplication::applicationName().toLower());
    QDomNodeList childrenNodes = mainNode.childNodes();
    for(int i = 0 ; i < childrenNodes.count() ; i++)
    {
        QDomElement childNode = childrenNodes.at(i).toElement();
        if(childNode.nodeName() == "node")
        {
            loadNode(childNode);
        }
        else if(childNode.nodeName() == "connection")
        {
            QUuid outputId = loadUid(childNode, "output_");
            QUuid inputId = loadUid(childNode, "input_");

            auto outputNode = _model->findNode(outputId);
            auto inputNode = _model->findNode(inputId);

            if(inputNode && outputNode)
            {
                QString outputName = childNode.attribute("output_plug");
                QString inputName = childNode.attribute("input_plug");

                Plug *plugOutput = outputNode->findOutput(outputName);
                Plug *plugInput = inputNode->findInput(inputName);

                if(!plugOutput)
                {
                    qWarning() << "Unable to find output named" << outputName << "on node" << outputId;
                }
                else if(!plugInput)
                {
                    qWarning() << "Unable to find input named" << inputName << "on node" << inputId;
                }
                else
                {
                    _model->addConnection(plugOutput, plugInput);
                    // Item is added by model signal
                }
            }
        }
        else if(childNode.nodeName() == "mainwindow")
        {
            mainWindow->restoreGeometry(QByteArray::fromHex(childNode.attribute("geometry").toUtf8()));
            mainWindow->restoreState(QByteArray::fromHex(childNode.attribute("state").toUtf8()));
        }
        else if(childNode.nodeName() == "executor-settings")
        {
            ExecutorSettings settings;

            settings.cacheData = Parser::parseBool(childNode.attribute("cache-data"));
            settings.useMultiThreading =
                    Parser::parseBool(childNode.attribute("use-multithreading"));
            settings.useOptimalThreadsCount =
                    Parser::parseBool(childNode.attribute("optimal-threads-count"));
            settings.fixedThreadsCount = childNode.attribute("fixed-threads-count").toUInt();

            _scheduler->setSettings(settings);
        }
    }
}

void ComposerScene::loadNode(const QDomElement &node)
{
    QString nodeName = node.attribute("name");
    QUuid uid = loadUid(node);
    GenericNodeItem *item = addNode(nodeName, uid);

    QDomNodeList nodeProperties = node.childNodes();
    QMap<QString, QString> nodeItemProperties;

    for(int j = 0 ; j < nodeProperties.count() ; j++)
    {
        QDomElement nodeProperty = nodeProperties.at(j).toElement();

        if(nodeProperty.nodeName() == "item-property")
        {
            nodeItemProperties.insert(nodeProperty.attribute("name"),
                                      nodeProperty.attribute("value"));
        }
        else if(nodeProperty.nodeName() == "plug")
        {
            QDomElement propertyElement = nodeProperty.toElement();
            QString plugName = propertyElement.attribute("name");
            Plug *plug = item->getNode()->findInput(plugName);
            if(!plug)
            {
                plug = item->getNode()->findOutput(plugName);
            }
            if(plug)
            {
                if(propertyElement.hasAttribute("value"))
                {
                    QVariant value = plug->load(propertyElement.attribute("value"));
                    if(not value.isNull())
                    {
                        // Node and items are updated manually, they are not connected to each
                        // other for performance reasons
                        item->setPlugProperty(plugName, value);
                        item->accessNode()->setProperty(plugName, value);
                    }
                }

                QDomNodeList plugNodeChildren = nodeProperty.childNodes();
                QMap<QString, QString> plugWidgetProperties;

                for(int k = 0 ; k < plugNodeChildren.count() ; k++)
                {
                    QDomElement plugPropertyNode = plugNodeChildren.at(k).toElement();
                    if(plugPropertyNode.nodeName() == "item-property")
                    {
                        plugWidgetProperties.insert(plugPropertyNode.attribute("name"),
                                                    plugPropertyNode.attribute("value"));
                    }
                }

                AbstractPlugWidget *widget = item->accessWidget(plugName);
                if(widget)
                {
                    item->accessWidget(plugName)->load(plugWidgetProperties);
                }
            }
            else
            {
                qWarning() << "No plug named" << plugName << "on node" << nodeName;
            }
        }
    }

    item->load(nodeItemProperties);
}

void ComposerScene::saveNode(QDomDocument &doc, QDomElement &domNode, const GenericNodeItem *nodeItem)
{
    const Node *node = nodeItem->getNode();

    domNode.setAttribute("name", node->getName());
    domNode.setAttribute("uid", node->getUid().toString());

    QMap<QString, QString> nodeItemProperties = nodeItem->save();
    auto iterator = nodeItemProperties.constBegin();
    while(iterator != nodeItemProperties.constEnd())
    {
        QDomElement nodeItemPropertyElement = doc.createElement("item-property");
        nodeItemPropertyElement.setAttribute("name", iterator.key());
        nodeItemPropertyElement.setAttribute("value", iterator.value());
        domNode.appendChild(nodeItemPropertyElement);

        iterator++;
    }

    for(const Plug *plug : (node->getInputs() + node->getOutputs()))
    {
        bool hasValue = false;
        const QString plugName = plug->getDefinition().name;

        QDomElement plugNode = doc.createElement("plug");
        plugNode.setAttribute("name", plugName);

        if(PlugType::isInputSavable(plug->getDefinition().types))
        {
            plugNode.setAttribute("value", plug->save(node->getProperties()[plugName]));
            hasValue = true;
        }

        QMap<QString, QString> plugWidgetProperties;
        const AbstractPlugWidget *plugWidget = nodeItem->getWidget(plugName);
        if(plugWidget)
        {
            plugWidgetProperties = plugWidget->save();

            auto iteratorWidget = plugWidgetProperties.constBegin();
            while(iteratorWidget != plugWidgetProperties.constEnd())
            {
                QDomElement plugWidgetPropertyElement = doc.createElement("item-property");
                plugWidgetPropertyElement.setAttribute("name", iteratorWidget.key());
                plugWidgetPropertyElement.setAttribute("value", iteratorWidget.value());
                plugNode.appendChild(plugWidgetPropertyElement);

                iteratorWidget++;
            }
        }

        if(hasValue || not plugWidgetProperties.isEmpty())
        {
            domNode.appendChild(plugNode);
        }
    }
}

void ComposerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    dragMoveEvent(event);
}

void ComposerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();
    }
}

void ComposerScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();

        const QString nodeName = QString::fromUtf8(event->mimeData()->data("application/x-cvcomposerfilter"));
        _commandsStack->push(new CreateNodeCommand(this, nodeName, event->scenePos()));
    }
}

void ComposerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        event->setModifiers(event->modifiers() & ~Qt::ControlModifier);

        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if(item)
        {
            if(item->type() == QGraphicsProxyWidget::Type)
            {
                // Let the parent do its job fully but disable multi-selection
                return QGraphicsScene::mousePressEvent(event);
            }
            else if(item->type() == static_cast<int>(CustomItems::Plug))
            {
                PlugItem *plug = static_cast<PlugItem *>(item);
                PlugType::PlugTypes types = plug->getPlug()->getDefinition().types;
                Node *nodeInput = _model->findInputPlug(plug->getPlug());
                Node *nodeOutput = _model->findOutputPlug(plug->getPlug());
                bool isInput = nodeInput != nullptr;
                bool isOutput = nodeOutput != nullptr;

                _editedConnection.item = nullptr;
                _editedConnection.plugInput = nullptr;
                _editedConnection.plugOutput = nullptr;
                _editedConnection.initialItem = nullptr;
                _editedConnection.initialConnection = nullptr;
                _editedConnection.item = new ConnectionItem();
                addItem(_editedConnection.item);

                // When editing an input node, edit its actual connection if there is one
                if(isInput)
                {
                    for(ConnectionItem *connectionItem : _connections)
                    {
                        const Connection *connection = connectionItem->getConnection();
                        if(connection->getInput() == plug->getPlug())
                        {
                            // We have found an existing connection, edit it
                            _editedConnection.item->copyColorFrom(connectionItem);
                            _editedConnection.item->setOutput(connectionItem->getOutput());
                            _editedConnection.item->setInput(event->scenePos());
                            _editedConnection.plugOutput = connection->getOutput();
                            _editedConnection.fromOutput = true;
                            _editedConnection.initialItem = connectionItem;
                            _editedConnection.initialItem->hide();
                            _editedConnection.initialConnection = connection;
                            return;
                        }
                    }
                }

                // We are not editing an existing connection
                if(PlugType::isSingleType(types))
                {
                    _editedConnection.item->setCurrentType(PlugType::flagsToEnum(types), true);
                }

                if(isInput)
                {
                    _editedConnection.item->setOutput(event->scenePos());
                    _editedConnection.item->setInput(plug->mapToScene(QPointF(0, 0)));
                    _editedConnection.plugInput = plug->getPlug();
                    _editedConnection.fromOutput = false;
                    _editedConnection.baseNode = nodeInput;
                }
                else if(isOutput)
                {
                    _editedConnection.item->setOutput(plug->mapToScene(QPointF(0, 0)));
                    _editedConnection.item->setInput(event->scenePos());
                    _editedConnection.plugOutput = plug->getPlug();
                    _editedConnection.fromOutput = true;
                    _editedConnection.baseNode = nodeOutput;
                }
                else
                {
                    qCritical("Selected plug not found in node inputs/outputs");
                }
            }
            else if(item->type() == static_cast<int>(CustomItems::Node))
            {
                const QPointF relativePos = item->mapFromScene(event->scenePos());
                GenericNodeItem *nodeItem = static_cast<GenericNodeItem *>(item);

                if(nodeItem->startDragging(relativePos))
                {
                    // Mouse pressed over title : start dragging it
                    event->widget()->setCursor(Qt::ClosedHandCursor);
                    _editedNode.item = nodeItem;
                    _editedNode.initClickPos = event->scenePos();
                    _editedNode.initNodePose = _editedNode.item->pos();
                }
                else
                {
                    QGraphicsScene::mousePressEvent(event);
                }

                // Select node
                QPainterPath path;
                path.addRect(QRectF(event->scenePos(), QSize(1, 1)));
                setSelectionArea(path);

                // Move node on top
                for(GenericNodeItem *node : _nodes)
                {
                    if(node != item)
                    {
                        node->stackBefore(item);
                    }
                }
            }
        }
        else
        {
            clearSelection();
        }
    }
}

void ComposerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    QCursor cursor = Qt::ArrowCursor;

    if(_editedConnection.item)
    {
        bool plugFound = false;

        for(GenericNodeItem *nodeItem : _nodes)
        {
            if(nodeItem->getNode() == _editedConnection.baseNode)
            {
                continue;
            }

            const QList<PlugItem *> &plugItems = _editedConnection.fromOutput ? nodeItem->getInputs() : nodeItem->getOutputs();
            for(PlugItem *plugItem : plugItems)
            {
                PlugType::PlugTypes outputTypes;
                PlugType::PlugTypes inputTypes;

                if(_editedConnection.fromOutput)
                {
                    outputTypes = _editedConnection.plugOutput->getDefinition().types;
                    inputTypes = plugItem->getPlug()->getDefinition().types;
                }
                else
                {
                    outputTypes = plugItem->getPlug()->getDefinition().types;
                    inputTypes = _editedConnection.plugInput->getDefinition().types;
                }

                if(inputTypes & outputTypes)
                {
                    QPointF itemPos = plugItem->mapToScene(QPointF(0, 0));
                    qreal distance = (event->scenePos() - itemPos).manhattanLength();
                    if(distance < PlugItem::magnetRadius)
                    {
                        if(_editedConnection.fromOutput)
                        {
                            _editedConnection.item->setInput(plugItem->mapToScene(QPointF(0, 0)));
                            _editedConnection.plugInput = plugItem->getPlug();
                        }
                        else
                        {
                            _editedConnection.item->setOutput(plugItem->mapToScene(QPointF(0, 0)));
                            _editedConnection.plugOutput = plugItem->getPlug();
                        }

                        cursor = Qt::PointingHandCursor;
                        plugFound = true;
                    }
                }
            }
        }

        if(not plugFound)
        {
            if(_editedConnection.fromOutput)
            {
                _editedConnection.item->setInput(event->scenePos());
                _editedConnection.plugInput = nullptr;
            }
            else
            {
                _editedConnection.item->setOutput(event->scenePos());
                _editedConnection.plugOutput = nullptr;
            }
        }
    }
    else if(_editedNode.item)
    {
        cursor = Qt::ClosedHandCursor;

        QPointF sourcePos = _editedNode.item->pos();
        QPointF targetPos(_editedNode.initNodePose + (event->scenePos() - _editedNode.initClickPos));
        _commandsStack->push(new MoveNodeCommand(this, _editedNode.item->getNode()->getUid(), sourcePos, targetPos));
    }
    else
    {
        // We are not editing anything
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if(item)
        {
            if(item->type() == static_cast<int>(CustomItems::Plug))
            {
                cursor = Qt::PointingHandCursor;
            }
            else if(item->type() == static_cast<int>(CustomItems::Node))
            {
                QPointF relativePos = item->mapFromScene(event->scenePos());
                cursor = ((GenericNodeItem *)item)->overrideMouseCursor(relativePos);
            }
        }
    }

    event->widget()->setCursor(cursor);
}

void ComposerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Disable multi-selection
    event->setModifiers(event->modifiers() & ~Qt::ControlModifier);

    if(_editedConnection.item)
    {
        auto command = EditConnectionCommand::makeMetaCommand(_model,
                                                              _editedConnection.initialConnection,
                                                              _editedConnection.plugOutput,
                                                              _editedConnection.plugInput);

        if(command)
        {
            _commandsStack->push(command);
        }

        if(!command && _editedConnection.initialItem)
        {
            // Restore connection item
            _editedConnection.initialItem->show();
        }

        removeItem(_editedConnection.item);
        delete _editedConnection.item;

        _editedConnection.item = nullptr;
        _editedConnection.plugInput = nullptr;
        _editedConnection.plugOutput = nullptr;
        _editedConnection.initialConnection = nullptr;
        _editedConnection.initialItem = nullptr;
    }
    else if(_editedNode.item)
    {
        _editedNode.item = nullptr;
        event->widget()->setCursor(Qt::OpenHandCursor);
    }
    else
    {
        // Let the parent do its job fully
        return QGraphicsScene::mouseReleaseEvent(event);
    }
}

void ComposerScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)
    {
        for(QGraphicsItem *item : selectedItems())
        {
            if(item->type() == static_cast<int>(CustomItems::Node))
            {
                auto command = RemoveNodeCommand::makeMetaCommand(this, static_cast<GenericNodeItem *>(item));
                if(command)
                {
                    _commandsStack->push(command);
                }
                return;
            }
        }
    }

    QGraphicsScene::keyPressEvent(keyEvent);
}

void ComposerScene::onNodeRemoved(const Node *node)
{
    for(GenericNodeItem *nodeItem : _nodes)
    {
        if(nodeItem->getNode() == node)
        {
            removeItem(nodeItem);
            delete nodeItem;
            _nodes.removeAll(nodeItem);
            break;
        }
    }
}

void ComposerScene::onConnectionAdded(const Connection *connection)
{
    PlugType::PlugTypes outputTypes = connection->getOutput()->getDefinition().types;
    PlugType::PlugTypes inputTypes = connection->getInput()->getDefinition().types;
    QList<PlugType::Enum> compatibleTypes = PlugType::toList(outputTypes & inputTypes);
    PlugType::Enum connectionType = compatibleTypes.value(0);

    ConnectionItem *connectionItem = new ConnectionItem();
    connectionItem->setConnection(connection);

    if(_editedConnection.item)
    {
        connectionItem->copyColorFrom(_editedConnection.item);
    }

    if(compatibleTypes.count() <= PlugItem::maxMultiTypes)
    {
        connectionItem->setCurrentType(connectionType);
    }

    for(const GenericNodeItem *nodeView : _nodes)
    {
        for(PlugItem *plugItem : nodeView->getInputs())
        {
            if(plugItem->getPlug() == connection->getInput())
            {
                plugItem->setCurrentType(connectionType, true);
                connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
        for(PlugItem *plugItem : nodeView->getOutputs())
        {
            if(plugItem->getPlug() == connection->getOutput())
            {
                plugItem->setCurrentType(connectionType, false);
                connectionItem->setOutput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
    }

    addItem(connectionItem);
    _connections << connectionItem;
}

void ComposerScene::onConnectionRemoved(const Connection *connection)
{
    for(ConnectionItem *connectionItem : _connections)
    {
        if(connectionItem->getConnection() == connection)
        {
            removeItem(connectionItem);
            delete connectionItem;
            _connections.removeAll(connectionItem);
            break;
        }
    }
}

void ComposerScene::onPlugItemPositionChanged()
{
    // A plug item has moved, move the associated connections
    PlugItem *plugItem = qobject_cast<PlugItem *>(sender());
    if(plugItem)
    {
        const Plug *plug = plugItem->getPlug();
        for(ConnectionItem *connectionItem : _connections)
        {
            const Connection *connection = connectionItem->getConnection();

            if(connection->getInput() == plug)
            {
                connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
            }
            else if(connection->getOutput() == plug)
            {
                connectionItem->setOutput(plugItem->mapToScene(QPointF(0, 0)));
            }
        }
    }
    else
    {
        qCritical() << "ComposerScene::onPlugItemPositionChanged" << "Sender is not a PlugItem";
    }
}

void ComposerScene::onExecutionStarted(const Node *node)
{
    GenericNodeItem *nodeItem;
    if((nodeItem = findItem(node)) != nullptr)
    {
        nodeItem->executionStarted();
    }
}

void ComposerScene::onExecutionProgress(const Node *node, qreal progress)
{
    GenericNodeItem *nodeItem;
    if((nodeItem = findItem(node)) != nullptr)
    {
        nodeItem->executionProgress(progress);
    }
}

void ComposerScene::onExecutionEnded(const Node *node,
                                     const Properties &outputs,
                                     const Properties &inputs,
                                     qint64 duration,
                                     const QString &error)
{
    GenericNodeItem *nodeItem;
    if((nodeItem = findItem(node)) != nullptr)
    {
        nodeItem->executionEnded(outputs, inputs, duration, error);
    }
}

void ComposerScene::onNodeInvalid(const Node *node)
{
    GenericNodeItem *nodeItem;
    if((nodeItem = findItem(node)) != nullptr)
    {
        nodeItem->nodeInvalid();
    }
}

void ComposerScene::onNodePropertyChanged(const QString &name, const QVariant &value)
{
    auto node = qobject_cast<Node *>(sender());
    if(node)
    {
        GenericNodeItem *nodeItem;
        if((nodeItem = findItem(node)))
        {
            nodeItem->setPlugProperty(name, value);
        }
    }
}

void ComposerScene::onPlugValueChanged(const QString &name, const QVariant &value)
{
    auto item = qobject_cast<GenericNodeItem *>(sender());
    if(item)
    {
        auto node = item->getNode();
        _commandsStack->push(new EditValueCommand(_model,
                                                  node->getUid(),
                                                  name,
                                                  node->getProperties().value(name),
                                                  value));
    }
}

GenericNodeItem *ComposerScene::findItem(const Node *node)
{
    for(GenericNodeItem *nodeItem : _nodes)
    {
        if(nodeItem->getNode() == node)
        {
            return nodeItem;
        }
    }

    qCritical() << "No item found for node" << node->getName();
    return nullptr;
}

QUuid ComposerScene::loadUid(const QDomElement &node, const QString &attributePrefix)
{
    QUuid uid = QUuid(node.attribute(QString("%1uid").arg(attributePrefix)));
    if(uid.isNull())
    {
        // Retro-compatibility with old format
        quint64 oldId = node.attribute(QString("%1id").arg(attributePrefix)).toULongLong();
        uid = QUuid(static_cast<quint32>(oldId >> 32),
                    static_cast<quint16>(oldId >> 16),
                    static_cast<quint16>(oldId),
                    0, 0, 0, 0, 0, 0, 0, 0);
    }

    return uid;
}
