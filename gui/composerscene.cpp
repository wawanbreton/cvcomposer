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
#include "gui/genericnodeitem.h"
#include "gui/customitems.h"
#include "gui/connectionitem.h"
#include "gui/plugitem.h"
#include "plugwidget/abstractplugwidget.h"
#include "global/parser.h"


ComposerScene::ComposerScene(QObject *parent) :
    QGraphicsScene(parent),
    _model(new ComposerModel(this)),
    _scheduler(new ComposerScheduler(_model, this)),
    _editedConnection(),
    _editedNode(),
    _connections()
{
    init();

    _scheduler->start();
}

ComposerScene::ComposerScene(const QDomDocument &doc, QMainWindow *mainWindow, QObject *parent) :
    QGraphicsScene(parent),
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
    _editedConnection.item = NULL;
    _editedConnection.plugInput = NULL;
    _editedConnection.plugOutput = NULL;

    _editedNode.item = NULL;

    connect(_model, SIGNAL(nodeRemoved(const Node*)),
                    SLOT(onNodeRemoved(const Node*)));
    connect(_model, SIGNAL(connectionAdded(const Connection *)),
                    SLOT(onConnectionAdded(const Connection *)));
    connect(_model, SIGNAL(connectionRemoved(const Connection *)),
                    SLOT(onConnectionRemoved(const Connection *)));

    setBackgroundBrush(QColor("#273035"));
}

const QList<GenericNodeItem *> &ComposerScene::getNodes() const
{
    return _nodes;
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

GenericNodeItem *ComposerScene::addNode(const QString &nodeName)
{
    Node *node = new Node(nodeName, ProcessorsFactory::toUserReadableName(nodeName));
    _model->addNode(node);

    GenericNodeItem *item = new GenericNodeItem(node);
    addItem(item);
    _nodes << item;

    foreach(PlugItem *plugItem, item->getInputs() + item->getOutputs())
    {
        connect(plugItem, SIGNAL(positionChanged()), SLOT(onPlugItemPositionChanged()));
    }

    return item;
}

void ComposerScene::save(QDomDocument &doc, QMainWindow *mainWindow) const
{
    QDomElement rootNode = doc.createElement(QCoreApplication::applicationName().toLower());

    // Nodes
    for(GenericNodeItem *nodeItem : getNodes())
    {
        const Node *node = nodeItem->getNode();

        QDomElement nodeElement = doc.createElement("node");
        nodeElement.setAttribute("name", node->getName());
        nodeElement.setAttribute("id", QString::number(quint64(node)));

        QMap<QString, QString> nodeItemProperties = nodeItem->save();
        auto iterator = nodeItemProperties.constBegin();
        while(iterator != nodeItemProperties.constEnd())
        {
            QDomElement nodeItemPropertyElement = doc.createElement("item-property");
            nodeItemPropertyElement.setAttribute("name", iterator.key());
            nodeItemPropertyElement.setAttribute("value", iterator.value());
            nodeElement.appendChild(nodeItemPropertyElement);

            iterator++;
        }

        for(const Plug *plug : node->getInputs())
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
            const AbstractPlugWidget *plugWidget = nodeItem->getInputWidget(plugName);
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
                nodeElement.appendChild(plugNode);
            }
        }

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
            connectionElement.setAttribute("output_id", QString::number(quint64(outputNode)));
            connectionElement.setAttribute("output_plug", outputPlug->getDefinition().name);
            connectionElement.setAttribute("input_id", QString::number(quint64(inputNode)));
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
    connect(_scheduler, SIGNAL(ended()), SIGNAL(ended()));
    _scheduler->end();
}

void ComposerScene::load(const QDomDocument &doc, QMainWindow *mainWindow)
{
    QDomNode mainNode = doc.namedItem(QCoreApplication::applicationName().toLower());
    QDomNodeList childrenNodes = mainNode.childNodes();
    QMap<quint64, Node *> loadedNodes;
    for(int i = 0 ; i < childrenNodes.count() ; i++)
    {
        QDomElement childNode = childrenNodes.at(i).toElement();
        if(childNode.nodeName() == "node")
        {
            QString nodeName = childNode.attribute("name");
            GenericNodeItem *item = addNode(nodeName);
            loadedNodes.insert(childNode.attribute("id").toULongLong(), item->accessNode());

            QDomNodeList nodeProperties = childNode.childNodes();
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

                        item->accessInputWidget(plugName)->load(plugWidgetProperties);
                    }
                    else
                    {
                        qWarning() << "No input plug named" << plugName << "on node" << nodeName;
                    }
                }
            }

            item->load(nodeItemProperties);
        }
        else if(childNode.nodeName() == "connection")
        {
            quint64 outputId = childNode.attribute("output_id").toULongLong();
            quint64 inputId = childNode.attribute("input_id").toULongLong();

            auto iteratorOutput = loadedNodes.find(outputId);
            auto iteratorInput = loadedNodes.find(inputId);

            if(iteratorOutput != loadedNodes.end() && iteratorInput != loadedNodes.end())
            {
                QString outputName = childNode.attribute("output_plug");
                QString inputName = childNode.attribute("input_plug");

                Plug *plugOutput = iteratorOutput.value()->findOutput(outputName);
                Plug *plugInput = iteratorInput.value()->findInput(inputName);

                if(plugOutput == NULL)
                {
                    qWarning() << "Unable to find output named" << outputName << "on node" << outputId;
                }
                else if(plugInput == NULL)
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

        GenericNodeItem *item =
                addNode(QString::fromUtf8(event->mimeData()->data("application/x-cvcomposerfilter")));
        item->setPos(event->scenePos());
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
            else if(item->type() == CustomItems::Plug)
            {
                PlugItem *plug = static_cast<PlugItem *>(item);
                Node *nodeInput = _model->findInputPlug(plug->getPlug());
                Node *nodeOutput = _model->findOutputPlug(plug->getPlug());
                bool isInput = nodeInput != NULL;
                bool isOutput = nodeOutput != NULL;

                _editedConnection.item = new ConnectionItem();
                addItem(_editedConnection.item);

                // When editing an input node, edit its actual connection if there is one
                if(isInput)
                {
                    foreach(ConnectionItem *connectionItem, _connections)
                    {
                        const Connection *connection = connectionItem->getConnection();
                        if(connection->getInput() == plug->getPlug())
                        {
                            // We have found an existing connection, edit it
                            _editedConnection.item->setOutput(connectionItem->getOutput());
                            _editedConnection.item->setInput(event->scenePos());
                            _editedConnection.plugOutput = connection->getOutput();
                            _editedConnection.fromOutput = true;
                            _model->removeConnection(connectionItem->getConnection());
                            return;
                        }
                    }
                }

                // We are not editing an existing connection
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
            else if(item->type() == CustomItems::Node)
            {
                if(item->mapFromScene(event->scenePos()).y() < GenericNodeItem::titleHeight)
                {
                    // Mouse pressed over title : start dragging it
                    event->widget()->setCursor(Qt::ClosedHandCursor);
                    _editedNode.item = static_cast<GenericNodeItem *>(item);
                    _editedNode.initClickPos = event->scenePos();
                    _editedNode.initNodePose = _editedNode.item->pos();
                }

                // Select node
                QPainterPath path;
                path.addRect(QRectF(event->scenePos(), QSize(1, 1)));
                setSelectionArea(path);

                // Move node on top
                foreach(GenericNodeItem *node, _nodes)
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
    QCursor cursor = Qt::ArrowCursor;

    if(_editedConnection.item)
    {
        bool plugFound = false;

        foreach(GenericNodeItem *nodeItem, _nodes)
        {
            if(nodeItem->getNode() == _editedConnection.baseNode)
            {
                continue;
            }

            const QList<PlugItem *> &plugItems = _editedConnection.fromOutput ? nodeItem->getInputs() : nodeItem->getOutputs();
            foreach(PlugItem *plugItem, plugItems)
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
                _editedConnection.plugInput = NULL;
            }
            else
            {
                _editedConnection.item->setOutput(event->scenePos());
                _editedConnection.plugOutput = NULL;
            }
        }
    }
    else if(_editedNode.item)
    {
        cursor = Qt::ClosedHandCursor;

        _editedNode.item->setPos(_editedNode.initNodePose + (event->scenePos() - _editedNode.initClickPos));
    }
    else
    {
        // We are not editing anything
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if(item)
        {
            if(item->type() == QGraphicsProxyWidget::Type)
            {
                // Let the parent do its job fully
                return QGraphicsScene::mouseMoveEvent(event);
            }
            else if(item->type() == CustomItems::Plug)
            {
                cursor = Qt::PointingHandCursor;
            }
            else if(item->type() == CustomItems::Node)
            {
                if(item->mapFromScene(event->scenePos()).y() < GenericNodeItem::titleHeight)
                {
                    cursor = Qt::OpenHandCursor;
                }
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
        if(_editedConnection.plugInput && _editedConnection.plugOutput)
        {
            _model->addConnection(_editedConnection.plugOutput, _editedConnection.plugInput);
        }

        removeItem(_editedConnection.item);
        delete _editedConnection.item;

        _editedConnection.item = NULL;
        _editedConnection.plugInput = NULL;
        _editedConnection.plugOutput = NULL;
    }
    else if(_editedNode.item)
    {
        _editedNode.item = NULL;
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
        foreach(QGraphicsItem *item, selectedItems())
        {
            if(item->type() == CustomItems::Node)
            {
                _model->removeNode(((GenericNodeItem *)item)->accessNode());
            }
        }
    }
    else
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}

void ComposerScene::onNodeRemoved(const Node *node)
{
    foreach(GenericNodeItem *nodeItem, _nodes)
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
    ConnectionItem *connectionItem = new ConnectionItem();
    connectionItem->setConnection(connection);

    foreach(const GenericNodeItem *nodeView, _nodes)
    {
        foreach(const PlugItem *plugItem, nodeView->getInputs())
        {
            if(plugItem->getPlug() == connection->getInput())
            {
                connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
        foreach(const PlugItem *plugItem, nodeView->getOutputs())
        {
            if(plugItem->getPlug() == connection->getOutput())
            {
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
    foreach(ConnectionItem *connectionItem, _connections)
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
        foreach(ConnectionItem *connectionItem, _connections)
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
