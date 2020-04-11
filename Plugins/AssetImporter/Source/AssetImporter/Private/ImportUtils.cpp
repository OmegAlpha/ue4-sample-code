#include "ImportUtils.h"
#include "FileHelpers.h"

bool FImportUtils::SaveDirtyAssets()
{
	TArray<UPackage*> DirtyPackages;
	UEditorLoadingAndSavingUtils::GetDirtyContentPackages(DirtyPackages);

	bool bResult = true;

	if (DirtyPackages.Num())
	{
		bResult = UEditorLoadingAndSavingUtils::SavePackages(DirtyPackages, true);
	}

	return bResult;
}
