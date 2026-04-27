// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"
#include "PatrolPath.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#endif


APatrolPoint::APatrolPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(root);

#if WITH_EDITORONLY_DATA
    // 컴포넌트 세팅
    EditorBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
    if (EditorBillboard)
    {
        EditorBillboard->SetupAttachment(root);
        EditorBillboard->bIsEditorOnly = true;
    }

    EditorArrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("EditorArrow"));
    if (EditorArrow)
    {
        EditorArrow->SetupAttachment(root);
        EditorArrow->ArrowColor = FColor::Green;
        EditorArrow->bIsEditorOnly = true;
    }

    EditorIndexText = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(TEXT("EditorIndexText"));
    if (EditorIndexText)
    {
        EditorIndexText->SetupAttachment(root);
        EditorIndexText->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
        EditorIndexText->SetWorldRotation(FRotator(90.f, 0.f, 90.f));
        EditorIndexText->SetWorldSize(50.f);
        EditorIndexText->SetTextRenderColor(FColor::Green);
        EditorIndexText->SetHiddenInGame(true);
        EditorIndexText->bIsEditorOnly = true;
    }
#endif
}

void APatrolPoint::SetOwningPath(APatrolPath* InPath)
{
    OwningPath = InPath;
}

#if WITH_EDITOR
void APatrolPoint::PostEditMove(bool bFinished)
{
    Super::PostEditMove(bFinished);

    // 편집에 반응해서 갱신
    if (OwningPath.IsValid())
    {
        OwningPath->UpdateEditorVisuals();
    }
}

void APatrolPoint::UpdateEditorVisuals()
{
    if (!OwningPath.IsValid())
        return;

    const TArray<TObjectPtr<APatrolPoint>>& points = OwningPath->GetPoints();
    const int32 curIndex = points.IndexOfByKey(this);
    if (curIndex == INDEX_NONE)
        return;

    if (EditorIndexText)
    {
        EditorIndexText->SetText(FText::AsNumber(curIndex));
    }

    if (EditorArrow)
    {
        const int32 nextIndex = (curIndex + 1) % points.Num();
        const FVector toNextVec = points[nextIndex]->GetActorLocation() - GetActorLocation();
        EditorArrow->SetVisibility(points.Num() > 1);
        EditorArrow->SetWorldRotation(toNextVec.Rotation());
        EditorArrow->SetArrowLength(GetDistanceTo(points[nextIndex]));
    }
}
#endif
