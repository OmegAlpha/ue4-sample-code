// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AssetImporter.h"
#include "AssetImporterStyle.h"
#include "AssetImporterCommands.h"
#include "LevelEditor.h"

DEFINE_LOG_CATEGORY(LogAssetImporter);

#define LOCTEXT_NAMESPACE "FAssetImporterModule"

static const FName AssetImporterTabName("AssetImporter");

void FAssetImporterModule::StartupModule()
{
	FAssetImporterStyle::Initialize();
	FAssetImporterStyle::ReloadTextures();

	FAssetImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAssetImporterCommands::Get().OpenImportCharacterDialog,
		FExecuteAction::CreateRaw(this, &FAssetImporterModule::OnClicked_ImportCharacter),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FAssetImporterModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FAssetImporterModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	Importer = MakeShareable(new ImporterObject());
}

void FAssetImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FAssetImporterStyle::Shutdown();

	FAssetImporterCommands::Unregister();
}

void FAssetImporterModule::OnClicked_ImportCharacter()
{
	UE_LOG(LogAssetImporter, Log, TEXT("Open Dialog Import Character"));

	Importer->OpenBulkImportDialog();
}
void FAssetImporterModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FAssetImporterCommands::Get().OpenImportCharacterDialog);
}

void FAssetImporterModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	FSlateIcon Icon = FSlateIcon(FAssetImporterCommands::Get().GetStyleSetName(), "AssetImporter.ImportTabIcon");
	Builder.AddToolBarButton(FAssetImporterCommands::Get().OpenImportCharacterDialog, NAME_None, TAttribute<FText>(), TAttribute<FText>(), Icon);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetImporterModule, AssetImporter)