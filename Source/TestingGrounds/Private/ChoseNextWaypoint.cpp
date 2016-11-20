// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "ChoseNextWaypoint.h"

EBTNodeResult::Type UChoseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ensure(ControlledPawn)) { return EBTNodeResult::Failed; }

	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }
	
	auto PatrollPoints = PatrolRoute->GetPatrollRoute();
	if (PatrollPoints.Num() <= 0) { 
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"))
		return EBTNodeResult::Failed; }

	//get current index
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	//set next waypoint
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrollPoints[Index]);
	

	//cycle index
	Index = (Index + 1) % PatrollPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);

	return EBTNodeResult::Succeeded;


}
