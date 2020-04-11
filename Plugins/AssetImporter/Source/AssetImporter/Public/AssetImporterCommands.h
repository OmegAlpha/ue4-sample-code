// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AssetImporterStyle.h"

class FAssetImporterCommands : public TCommands<FAssetImporterCommands>
{
public:

	FAssetImporterCommands()
		: TCommands<FAssetImporterCommands>(TEXT("AssetImporter"), NSLOCTEXT("Contexts", "AssetImporter", "AssetImporter Plugin"), NAME_None, FAssetImporterStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:

	TSharedPtr< FUICommandInfo > OpenImportCharacterDialog;
};