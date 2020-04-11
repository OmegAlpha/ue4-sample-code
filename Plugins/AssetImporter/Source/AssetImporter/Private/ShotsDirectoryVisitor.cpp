#include "ShotsDirectoryVisitor.h"
#include "Paths.h"

FShotsDirectoryVisitor::FShotsDirectoryVisitor(FString AssetClass) :
	AssetClass(AssetClass)
{

}

bool FShotsDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (bIsDirectory)
	{
		AnimationsToImport.Push(MakeShareable(new FAnimationImporter(AssetClass, FilenameOrDirectory)));
	}

	return true;
}