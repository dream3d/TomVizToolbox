

#include "TomvizToolboxGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TomvizToolboxGuiPlugin::TomvizToolboxGuiPlugin()
: TomvizToolboxPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TomvizToolboxGuiPlugin::~TomvizToolboxGuiPlugin() = default;

#include "TomvizToolbox/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
