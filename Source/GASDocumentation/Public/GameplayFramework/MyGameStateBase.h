// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * AGameState ---> AGameStateBase ---> AInfo ---> AActor
 * 所有玩家需要共享的, 与游戏规则相关的事件, 在这里保存和同步
 * * 游戏运行了多久
 * * 每个加入游戏的Player, Player当前的状态
 * * 当前GameMode 的基类
 * * 游戏是否开始
 * 
 */

UCLASS()
class GASDOCUMENTATION_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
};
