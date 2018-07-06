/*
 * Your License should go here
 */
#pragma once

#include <QtCore/QString>

/**
* @brief This namespace is used to define some Constants for the plugin itself.
*/
namespace TomvizToolboxConstants
{
  const QString TomvizToolboxPluginFile("TomvizToolboxPlugin");
  const QString TomvizToolboxPluginDisplayName("Tomviz Toolbox");
  const QString TomvizToolboxBaseName("TomvizToolbox");

  namespace FilterGroups
  {
  	const QString TomvizToolboxFilters("TomvizToolbox");
  }
}

/**
* @brief Use this namespace to define any custom GUI widgets that collect FilterParameters
* for a filter. Do NOT define general reusable widgets here.
*/
namespace FilterParameterWidgetType
{
/* const QString SomeCustomWidget("SomeCustomWidget"); */
}
