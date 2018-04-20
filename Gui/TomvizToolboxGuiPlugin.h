#pragma once

#include "TomvizToolbox/TomvizToolboxPlugin.h"

class TomvizToolboxGuiPlugin : public TomvizToolboxPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "net.bluequartz.dream3d.TomvizToolboxGuiPlugin")

public:
  TomvizToolboxGuiPlugin();
  ~TomvizToolboxGuiPlugin() override;

public:
  TomvizToolboxGuiPlugin(const TomvizToolboxGuiPlugin&) = delete;            // Copy Constructor Not Implemented
  TomvizToolboxGuiPlugin(TomvizToolboxGuiPlugin&&) = delete;                 // Move Constructor
  TomvizToolboxGuiPlugin& operator=(const TomvizToolboxGuiPlugin&) = delete; // Copy Assignment Not Implemented
  TomvizToolboxGuiPlugin& operator=(TomvizToolboxGuiPlugin&&) = delete;      // Move Assignment Not Implemented
};
