#pragma once
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>

#include <Box2D.h>

#include <SFGUI\Widgets.hpp>

#include <cereal\cereal.hpp>

#include <UI\ColliderEditor.h>
#include <UI\ConfirmationDialog.h>
#include <UI\DataFileSelector.h>
#include <UI\GameWindow.h>
#include <UI\HardPointEditor.h>
#include <UI\HardPointWidget.h>
#include <UI\ImageSelector.h>
//#include <UI\InventoryItemWidget.h>
//#include <UI\InventoryWidget.h>
//#include <UI\InventoryWindow.h>
#include <UI\RigEditor.h>
#include <UI\RigEditorWidget.h>
#include <UI\RigTypeSelector.h>
#include <UI\RigNameEntry.h>
#include <UI\LaserRigEditorWidget.h>
#include <UI\MissileRigEditorWidget.h>
#include <UI\StringEntryPopup.h>
#include <UI\RigSelector.h>
#include <UI\ShieldStateDisplay.h>
#include <UI\ShipEditorWindow.h>
#include <UI\ShipNameEntry.h>
#include <UI\ShipSelector.h>
#include <UI\StationTradeWindow.h>
#include <UI\StationWindow.h>
#include <UI\ThrusterLocationEditor.h>
#include <UI\UI.h>
#include <UI\WidgetHelpers.h>

#include <ExtendedMath.h>
#include <VectorMath.h>
#include <WorldConstants.h>
#include <EventQueue.h>
#include <EntityHandle.h>
#include <PlayerData.h>