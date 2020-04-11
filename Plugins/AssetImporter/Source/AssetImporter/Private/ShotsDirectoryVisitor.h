#pragma once

#include "GenericPlatformFile.h"
#include "AnimationImporter.h"

struct FShotsDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
private:
	const FString AssetClass;

public:

	UPROPERTY()
	TArray<TSharedPtr<FAnimationImporter>> AnimationsToImport;

public:
	FShotsDirectoryVisitor(FString AssetClass);
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;
};