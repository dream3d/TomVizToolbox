/* ============================================================================
 * Copyright (c) 2017 BlueQuartz Software, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "TvReloadFile.h"

#include <QtCore/QEventLoop>
#include <QtCore/QFileInfo>

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"

#include "TomvizToolbox/TomvizToolboxConstants.h"
#include "TomvizToolbox/TomvizToolboxVersion.h"

#include "client/jsonrpcclient.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TvReloadFile::TvReloadFile()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TvReloadFile::~TvReloadFile() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvReloadFile::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvReloadFile::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Socket File", SocketFile, FilterParameter::Parameter, TvReloadFile));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvReloadFile::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  QString socketFilePath = getSocketFile();
  QFileInfo fi(socketFilePath);
  
  if(socketFilePath.isEmpty())
  {
    QString ss = QObject::tr("The socket file can not be empty.");
    setErrorCondition(-67000);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }
  if(!fi.exists())
  {
    QString ss = QObject::tr("The specified socket file does not exist.");
    setErrorCondition(-67001);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvReloadFile::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvReloadFile::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  if(getCancel())
  {
    return;
  }

  std::shared_ptr<MoleQueue::JsonRpcClient> client = std::shared_ptr<MoleQueue::JsonRpcClient>(new MoleQueue::JsonRpcClient(nullptr));
  //  MoleQueue::JsonRpcClient* client = ;

  QEventLoop waitLoop;
  client->connectToServer("tomviz");
  QJsonObject request(client->emptyRequest());
  request["method"] = QLatin1String("reloadFile");
  QJsonObject params;
  params["fileName"] = getSocketFile();
  request["params"] = params;
  client->sendRequest(request);

  QObject::connect(client.get(), SIGNAL(resultReceived(QJsonObject)), &waitLoop, SLOT(quit()));
  QObject::connect(client.get(), SIGNAL(errorReceived(QJsonObject)), &waitLoop, SLOT(quit()));

  QObject::connect(client.get(), &MoleQueue::JsonRpcClient::resultReceived, [=](QJsonObject obj) { Q_UNUSED(obj) });
  QObject::connect(client.get(), &MoleQueue::JsonRpcClient::errorReceived, [=] (QJsonObject obj) {
    int errCode = -1;
    QString errMsg = "An unidentified error occurred.";
    if (obj.contains("error") && obj["error"].isObject())
    {
      QJsonObject errObj = obj["error"].toObject();
      if (errObj.contains("code") && errObj["code"].isDouble())
      {
        errCode = errObj["code"].toInt();
      }
      if (errObj.contains("message") && errObj["message"].isString())
      {
        errMsg = errObj["message"].toString();
      }
    }

    setErrorCondition(errCode);
    notifyErrorMessage(getHumanLabel(), errMsg, getErrorCondition());
  });

  waitLoop.exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer TvReloadFile::newFilterInstance(bool copyFilterParameters) const
{
  TvReloadFile::Pointer filter = TvReloadFile::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvReloadFile::getCompiledLibraryName() const
{ return TomvizToolboxConstants::TomvizToolboxBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvReloadFile::getBrandingString() const
{
  return "TomvizToolbox";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvReloadFile::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  TomvizToolbox::Version::Major() << "." << TomvizToolbox::Version::Minor() << "." << TomvizToolbox::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvReloadFile::getGroupName() const
{ return SIMPL::FilterGroups::Unsupported; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid TvReloadFile::getUuid()
{
  return QUuid("{8f7ff79b-b084-5ea7-ac7b-bd26cc4f2ab0}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvReloadFile::getSubGroupName() const
{ return "TomvizToolbox"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvReloadFile::getHumanLabel() const
{ return "Reload File in Tomviz"; }

