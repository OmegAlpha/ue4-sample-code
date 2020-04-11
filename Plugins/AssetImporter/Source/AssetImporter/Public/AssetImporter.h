// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Logic/ImporterObject.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAssetImporter, Log, All);

class FToolBarBuilder;
class FMenuBuilder;

class FAssetImporterModule : public IModuleInterface
{
public:

	TSharedPtr<ImporterObject> Importer;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void OnClicked_ImportCharacter();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
