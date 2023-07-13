// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// ue
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

// chrono
#include "LaserBeam.h"
#include "Modifiers/ModifierTypes.h"

#include "TP_WeaponComponent2.generated.h"

class AChronoCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponCycle2);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHRONO_API UTP_WeaponComponent2 : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserBeam> pause_laser_bp;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserBeam> reset_laser_bp;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserBeam> speed_laser_bp;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserBeam> reverse_laser_bp;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase *FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage *FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext *FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *FireAction;

	/** CycleWeapon Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *CycleActionNext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *CycleActionPrevious;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	FColor laser_color;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnWeaponCycle2 OnWeaponCycle;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent2();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(AChronoCharacter *TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetControls(APlayerController *PlayerController);

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CycleNext();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CyclePrevious();

	/** The Character holding this weapon*/
	AChronoCharacter *Character;

	LaserType _laser_type;
};
