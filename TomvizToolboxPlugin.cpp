/*
 * Your License or Copyright can go here
 */

#include "TomvizToolboxPlugin.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/IFilterFactory.hpp"
#include "SIMPLib/Filtering/FilterFactory.hpp"

#include "TomvizToolbox/TomvizToolboxConstants.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TomvizToolboxPlugin::TomvizToolboxPlugin() :
m_Version("0.1.0"),                            // Initialize TomvizToolbox's Version Number Here
m_CompatibilityVersion("0.1.0"), // Initialize TomvizToolbox's Compatibility Version Number Here
m_Vendor("Vendor Name"),                                // Initialize TomvizToolbox's Vendor Name Here
m_URL("URL"),                                           // Initialize Company URL Here
m_Location("Location"),                                 // Initialize TomvizToolbox library Location Here
m_Description("Description"),                           // Initialize TomvizToolbox's Description Here
m_Copyright("Copyright"),                               // Initialize TomvizToolbox's Copyright Here
m_Filters(QList<QString>()),                        // Initialize TomvizToolbox's List of Dependencies Here
m_DidLoad(false)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TomvizToolboxPlugin::~TomvizToolboxPlugin() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getPluginFileName()
{
  return TomvizToolboxConstants::TomvizToolboxPluginFile;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getPluginDisplayName()
{
  return TomvizToolboxConstants::TomvizToolboxPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getPluginBaseName()
{
  return TomvizToolboxConstants::TomvizToolboxBaseName;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getVersion()
{
  return m_Version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getCompatibilityVersion()
{
  return m_CompatibilityVersion;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getVendor()
{
  return m_Vendor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getURL()
{
  return m_URL;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getLocation()
{
  return m_Location;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getDescription()
{
  /* PLEASE UPDATE YOUR PLUGIN'S DESCRIPTION FILE.
  It is located at TomvizToolbox/Resources/TomvizToolbox/TomvizToolboxDescription.txt */

  QFile licenseFile(":/TomvizToolbox/TomvizToolboxDescription.txt");
  QFileInfo licenseFileInfo(licenseFile);
  QString text = "<<--Description was not read-->>";

  if ( licenseFileInfo.exists() )
  {
    if ( licenseFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      QTextStream in(&licenseFile);
      text = in.readAll();
    }
  }
  return text;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getCopyright()
{
  return m_Copyright;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString TomvizToolboxPlugin::getLicense()
{
  /* PLEASE UPDATE YOUR PLUGIN'S LICENSE FILE.
  It is located at TomvizToolbox/Resources/TomvizToolbox/TomvizToolboxLicense.txt */

  QFile licenseFile(":/TomvizToolbox/TomvizToolboxLicense.txt");
  QFileInfo licenseFileInfo(licenseFile);
  QString text = "<<--License was not read-->>";

  if ( licenseFileInfo.exists() )
  {
    if ( licenseFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      QTextStream in(&licenseFile);
      text = in.readAll();
    }
  }
  return text;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMap<QString, QString> TomvizToolboxPlugin::getThirdPartyLicenses()
{
  QMap<QString, QString> licenseMap;
  QList<QString> fileStrList;
  fileStrList.push_back(":/ThirdParty/HDF5.txt");
  
  fileStrList.push_back(":/ThirdParty/Qt.txt");
  fileStrList.push_back(":/ThirdParty/Qwt.txt");

  for (QList<QString>::iterator iter = fileStrList.begin(); iter != fileStrList.end(); iter++)
  {
    QFile file(*iter);
    QFileInfo licenseFileInfo(file);

    if ( licenseFileInfo.exists() )
    {
      if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
      {
        QTextStream in(&file);
        licenseMap.insert(licenseFileInfo.baseName(), in.readAll());
      }
    }
  }

  return licenseMap;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool TomvizToolboxPlugin::getDidLoad()
{
  return m_DidLoad;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TomvizToolboxPlugin::setDidLoad(bool didLoad)
{
  m_DidLoad = didLoad;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TomvizToolboxPlugin::setLocation(QString filePath)
{
  m_Location = filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TomvizToolboxPlugin::writeSettings(QSettings& prefs)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TomvizToolboxPlugin::readSettings(QSettings& prefs)
{
}

#include "TomvizToolboxFilters/RegisterKnownFilters.cpp"

#include "FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"

