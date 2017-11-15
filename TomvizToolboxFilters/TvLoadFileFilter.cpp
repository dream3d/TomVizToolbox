/*
 * Your License or Copyright can go here
 */

#include "TvLoadFileFilter.h"

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "TomvizToolbox/TomvizToolboxConstants.h"
#include "TomvizToolbox/TomvizToolboxVersion.h"

#include "client/jsonrpcclient.h"

// Include the MOC generated file for this class
#include "moc_TvLoadFileFilter.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TvLoadFileFilter::TvLoadFileFilter()
: AbstractFilter()
{
  initialize();
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TvLoadFileFilter::~TvLoadFileFilter()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvLoadFileFilter::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvLoadFileFilter::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_STRING_FP("Socket File", SocketFile, FilterParameter::Parameter, TvLoadFileFilter));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvLoadFileFilter::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  if(getSocketFile().isEmpty())
  {
    QString ss = QObject::tr("The socket file can not be empty.");
    setErrorCondition(-67000);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvLoadFileFilter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves
  // correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can
                                     // do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets
                                     // push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TvLoadFileFilter::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  if(getCancel() == true)
  {
    return;
  }
  
  std::shared_ptr<MoleQueue::JsonRpcClient> client = std::shared_ptr<MoleQueue::JsonRpcClient>(new MoleQueue::JsonRpcClient(this));
  //  MoleQueue::JsonRpcClient* client = ;
  client->connectToServer(getSocketFile());
  QJsonObject request(client->emptyRequest());
  request["method"] = QLatin1String("listQueues");
  client->sendRequest(request);
  
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer TvLoadFileFilter::newFilterInstance(bool copyFilterParameters)
{
  TvLoadFileFilter::Pointer filter = TvLoadFileFilter::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvLoadFileFilter::getCompiledLibraryName()
{
  return TomvizToolboxConstants::TomvizToolboxBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvLoadFileFilter::getBrandingString()
{
  return "TomvizToolbox";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvLoadFileFilter::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream << TomvizToolbox::Version::Major() << "." << TomvizToolbox::Version::Minor() << "." << TomvizToolbox::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvLoadFileFilter::getGroupName()
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvLoadFileFilter::getSubGroupName()
{
  return "TomvizToolbox";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString TvLoadFileFilter::getHumanLabel()
{
  return "TvLoadFileFilter";
}
