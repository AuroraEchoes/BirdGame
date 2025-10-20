#include "BoidEditorVisualisations.h"

#include "BoidFlockSpawnerComponent.h"
#include "ComponentVisualizer.h"
#include "FBoidFlockSpawnerComponentVisualiser.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"

#define LOCTEXT_NAMESPACE "FBoidEditorVisualisationsModule"

void FBoidEditorVisualisationsModule::StartupModule()
{
    if (GUnrealEd != nullptr)
    {
        TSharedPtr<FComponentVisualizer> FlockVisualiser = MakeShareable(new FBoidFlockSpawnerComponentVisualiser);
        GUnrealEd->RegisterComponentVisualizer(UBoidFlockSpawnerComponent::StaticClass()->GetFName(), FlockVisualiser);
    }

}

void FBoidEditorVisualisationsModule::ShutdownModule()
{
    if (GUnrealEd != nullptr)
    {
        GUnrealEd->UnregisterComponentVisualizer(UBoidFlockSpawnerComponent::StaticClass()->GetFName());
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBoidEditorVisualisationsModule, BoidEditorVisualisations)