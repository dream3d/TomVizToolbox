/******************************************************************************

  This source file is part of the MoleQueue project.

  Copyright 2012-2013 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "jsonrpcclient.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QDataStream>
#include <QtCore/QTimer>
#include <QtNetwork/QLocalSocket>

namespace MoleQueue
{

JsonRpcClient::JsonRpcClient(QObject *parent_) :
  QObject(parent_),
  m_packetCounter(0),
  m_socket(NULL)
{
  connect(this, SIGNAL(newPacket(QByteArray)), SLOT(readPacket(QByteArray)),
          Qt::QueuedConnection);
}

JsonRpcClient::~JsonRpcClient()
{
  flush();
}

bool JsonRpcClient::isConnected() const
{
  if(m_socket == nullptr)
  {
    return false;
  }
  {
    return m_socket->isOpen();
  }
}

bool JsonRpcClient::connectToServer(const QString &serverName_)
{
  if((m_socket != nullptr) && m_socket->isOpen())
  {
    if (m_socket->serverName() == serverName_) {
      return false;
    }

      m_socket->close();
      delete m_socket;
      m_socket = NULL;
  }

  // New connection.
  if (m_socket == NULL) {
    m_socket = new QLocalSocket(this);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
  }

  if (serverName_.isEmpty()) {
    return false;
  }

    m_socket->connectToServer(serverName_);
    return isConnected();
}

QString JsonRpcClient::serverName() const
{
  if(m_socket != nullptr)
  {
    return m_socket->serverName();
  }
  {
    return QString();
  }
}

void JsonRpcClient::flush()
{
  if(m_socket != nullptr)
  {
    m_socket->flush();
  }
}

QJsonObject JsonRpcClient::emptyRequest()
{
  QJsonObject request;
  request["jsonrpc"] = QLatin1String("2.0");
  request["id"] = static_cast<int>(m_packetCounter++);
  return request;
}

bool JsonRpcClient::sendRequest(const QJsonObject &request)
{
  if(m_socket == nullptr)
  {
    return false;
  }

  QJsonDocument document(request);
  QDataStream stream(m_socket);
  stream.setVersion(QDataStream::Qt_4_8);
  stream << document.toJson();
  return true;
}

void JsonRpcClient::readPacket(const QByteArray message)
{
  // Read packet into a Json value
  QJsonParseError error;
  QJsonDocument reader = QJsonDocument::fromJson(message, &error);

  if (error.error != QJsonParseError::NoError) {
    emit badPacketReceived("Unparseable message received\n:"
                           + error.errorString() + "\nContent: " + message);
    return;
  }
  if(!reader.isObject())
  {
    // We need a valid object, something bad happened.
    emit badPacketReceived("Packet did not contain a valid JSON object.");
    return;
  }

    QJsonObject root = reader.object();
    if (root["method"] != QJsonValue::Null) {
      if(root["id"] != QJsonValue::Null)
      {
        emit badPacketReceived("Received a request packet for the client.");
      }
      else
      {
        emit notificationReceived(root);
      }
    }
    if (root["result"] != QJsonValue::Null) {
      // This is a result packet, and should emit a signal.
      emit resultReceived(root);
    }
    else if (root["error"] != QJsonValue::Null) {
      emit errorReceived(root);
    }
}

void JsonRpcClient::readSocket()
{
  if (m_socket->bytesAvailable() > 0) {
    QDataStream stream(m_socket);
    QByteArray json;
    stream >> json;
    emit newPacket(json);
    if(m_socket->bytesAvailable() > 0)
    {
      QTimer::singleShot(0, this, SLOT(readSocket()));
    }
  }
}

} // End namespace MoleQueue
