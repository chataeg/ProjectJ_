// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Prop/JCCTVPreview.h"

AJCCTVPreview::AJCCTVPreview()
{


	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAssetRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Blueprints/MI_CCTV_Preview.MI_CCTV_Preview'"));
	if (MaterialAssetRef.Object)
	{
		BodyMesh->SetMaterial(0, MaterialAssetRef.Object);
	}
}
