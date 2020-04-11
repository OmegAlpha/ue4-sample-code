// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AssetImporterCommands.h"

#define LOCTEXT_NAMESPACE "FAssetImporterModule"

void FAssetImporterCommands::RegisterCommands()
{
	UI_COMMAND(OpenImportAvatarsDialog, "Avatar Import", "Import Avatars and Animations", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
