# Phase 1 GAS 인수 회고 자료

기준 범위: 808972b docs(lobby): Phase 0 회고 결과 반영 이후부터 현재 HEAD까지의 gas scope 커밋.

자가 검증 판정은 포함하지 않는다. PIE 통과 판정은 인수자가 직접 수행한다.

## 1. 커밋 로그 (git log -p)

명령:

```powershell
git -c core.quotepath=false log -p --reverse --stat 808972b..HEAD --extended-regexp --grep='^[a-z]+\(gas\):'
```

```diff
commit 4f898664981e9fc3916fe7a63d8df02022460ef0
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 20:51:45 2026 +0900

    docs(gas): Phase 1 설계 마찰 기록
    
    GAS Phase 1 구현 전에 결정해야 할 의존성, ASC 접근 계약, 검증용 수치, 임시 입력 책임 경계를 friction-log에 남긴다.
---
 docs/friction-log.md | 6 ++++++
 1 file changed, 6 insertions(+)

diff --git a/docs/friction-log.md b/docs/friction-log.md
index bc05da1..30a478e 100644
--- a/docs/friction-log.md
+++ b/docs/friction-log.md
@@ -7,3 +7,9 @@
 **형식:** `- [날짜] (scope) 한 줄 설명 — 그 자리에서 어떻게 처리했는지`
 
 ---
+
+- [2026-06-26] (phase1_gas) GAS Build.cs 모듈 의존성(GameplayAbilities/GameplayTags/GameplayTasks)이 문서에 빠져 구현 첫 단계가 모호함 — 설계 보강 필요로 기록
+- [2026-06-26] (phase1_gas) PlayerState ASC 소유만 있고 IAbilitySystemInterface 구현 여부가 명시되지 않아 이후 접근 계약이 흔들릴 수 있음 — 설계 보강 필요로 기록
+- [2026-06-26] (phase1_gas) Attribute 초기값과 테스트 GE 수치가 추후 튜닝으로만 남아 P0/P1 검증값을 정할 근거가 부족함 — 검증용 임시값 명시 필요로 기록
+- [2026-06-26] (phase1_gas) Phase 0 임시 입력은 Character에 있는데 문서는 PlayerController 입력 바인딩처럼 읽혀 구현 위치 해석이 갈림 — Character 입력 확장/RPC 송신 책임 분리 명시 필요로 기록
+- [2026-06-26] (phase1_gas) Armor 튜닝값 저장 방식, GAS Attribute 복제 매크로 패턴, 델리게이트 중복 구독 방지, 테스트 GE의 C++/에셋 형태가 덜 명확함 — 세부 구현 계약 보강 필요로 기록

commit c6df392ee33037756e5aa9f46bd0c83e308907f1
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 21:09:34 2026 +0900

    docs(gas): Phase 1 잔여 마찰 기록
    
    수정된 GAS Phase 1 설계에서 아직 구현 전에 확정해야 할 GE 에셋 생성 방식, Armor 계산 계약, ASC 복제 세부를 friction-log에 남긴다.
---
 docs/friction-log.md | 3 +++
 1 file changed, 3 insertions(+)

diff --git a/docs/friction-log.md b/docs/friction-log.md
index 30a478e..74780a0 100644
--- a/docs/friction-log.md
+++ b/docs/friction-log.md
@@ -13,3 +13,6 @@
 - [2026-06-26] (phase1_gas) Attribute 초기값과 테스트 GE 수치가 추후 튜닝으로만 남아 P0/P1 검증값을 정할 근거가 부족함 — 검증용 임시값 명시 필요로 기록
 - [2026-06-26] (phase1_gas) Phase 0 임시 입력은 Character에 있는데 문서는 PlayerController 입력 바인딩처럼 읽혀 구현 위치 해석이 갈림 — Character 입력 확장/RPC 송신 책임 분리 명시 필요로 기록
 - [2026-06-26] (phase1_gas) Armor 튜닝값 저장 방식, GAS Attribute 복제 매크로 패턴, 델리게이트 중복 구독 방지, 테스트 GE의 C++/에셋 형태가 덜 명확함 — 세부 구현 계약 보강 필요로 기록
+- [2026-06-26] (phase1_gas) 테스트 GE를 BP/DataAsset 인스턴스로 만들라고 했지만 UGameplayEffect 에셋 생성·참조 방식이 구현 관점에서 아직 모호함 — 설계 보강 필요로 기록
+- [2026-06-26] (phase1_gas) Armor 고정감소+cap의 정확한 산식과 Health/Shield/Armor/MovementSpeed 클램프 규칙이 빠져 Attribute 계산 결과가 갈릴 수 있음 — 계산 계약 보강 필요로 기록
+- [2026-06-26] (phase1_gas) ASC 복제 활성화(SetIsReplicated)와 GAS 모듈 의존성 Public/Private 위치가 명시되지 않아 Build/Replication 구현이 흔들릴 수 있음 — 세부 구현 계약 보강 필요로 기록

commit 39383d66867c9a808df5415033649bd6095ef9aa
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 21:31:02 2026 +0900

    docs(gas): GE 네이밍 관례 충돌 기록
    
    Phase 1이 GameplayEffect를 순수 C++로 확정하면서 기존 coding.md의 GE 에셋 네이밍 관례와 충돌하므로, 구현 전 보정이 필요한 마찰로 남긴다.
---
 docs/friction-log.md | 1 +
 1 file changed, 1 insertion(+)

diff --git a/docs/friction-log.md b/docs/friction-log.md
index 74780a0..70bb27e 100644
--- a/docs/friction-log.md
+++ b/docs/friction-log.md
@@ -16,3 +16,4 @@
 - [2026-06-26] (phase1_gas) 테스트 GE를 BP/DataAsset 인스턴스로 만들라고 했지만 UGameplayEffect 에셋 생성·참조 방식이 구현 관점에서 아직 모호함 — 설계 보강 필요로 기록
 - [2026-06-26] (phase1_gas) Armor 고정감소+cap의 정확한 산식과 Health/Shield/Armor/MovementSpeed 클램프 규칙이 빠져 Attribute 계산 결과가 갈릴 수 있음 — 계산 계약 보강 필요로 기록
 - [2026-06-26] (phase1_gas) ASC 복제 활성화(SetIsReplicated)와 GAS 모듈 의존성 Public/Private 위치가 명시되지 않아 Build/Replication 구현이 흔들릴 수 있음 — 세부 구현 계약 보강 필요로 기록
+- [2026-06-26] (phase1_gas) phase1_gas는 GE 순수 C++를 확정했지만 coding.md에는 GE_ 에셋 관례가 남아 문서 간 계약이 충돌함 — 사람 승인 후 coding.md 보정 필요로 기록

commit 9f5687ed12c2d518f6b3f025ad07733bbdd9e9f9
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 21:54:45 2026 +0900

    docs(gas): GE 구현 관례 명시
    
    현재 구현 단계에서는 GameplayEffect를 C++ UGE_ 클래스로 작성하고, GE_ 에셋은 콘텐츠와 밸런스 튜닝 단계에서 도입하도록 관례를 분리한다.
---
 docs/conventions/coding.md | 4 ++++
 1 file changed, 4 insertions(+)

diff --git a/docs/conventions/coding.md b/docs/conventions/coding.md
index 7d61bd8..d8d2702 100644
--- a/docs/conventions/coding.md
+++ b/docs/conventions/coding.md
@@ -29,6 +29,10 @@
 
 > 위 표는 초안이다. 어색하면 검토 시 수정. 핵심은 "한번 정하면 Codex가 일관되게 따른다"는 것.
 
+> 현재 구현 단계에서는 GameplayEffect를 우선 순수 C++ `UGE_` 클래스로 작성한다.
+> BP/DataAsset 기반 `GE_` 에셋은 콘텐츠/밸런스 튜닝 단계에서 필요해질 때 도입한다.
+> 따라서 Codex 구현 지시에서 별도 언급이 없으면 GE 에셋 생성을 전제로 하지 않는다.
+
 ## 3. 파일/구조
 
 - 클래스 하나당 `.h` / `.cpp` 한 쌍을 기본으로 한다.

commit 438caeec1b7f86c1800629b5027e56a01bd42d02
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 21:57:56 2026 +0900

    docs(gas): Phase 1 최종 점검 기록
    
    구현 시작 전에 phase1_gas에 남은 GE 관례 정리 문장과 Config 기반 수치 계약의 모호함을 friction-log에 남긴다.
---
 docs/friction-log.md | 2 ++
 1 file changed, 2 insertions(+)

diff --git a/docs/friction-log.md b/docs/friction-log.md
index 70bb27e..fbfdf49 100644
--- a/docs/friction-log.md
+++ b/docs/friction-log.md
@@ -17,3 +17,5 @@
 - [2026-06-26] (phase1_gas) Armor 고정감소+cap의 정확한 산식과 Health/Shield/Armor/MovementSpeed 클램프 규칙이 빠져 Attribute 계산 결과가 갈릴 수 있음 — 계산 계약 보강 필요로 기록
 - [2026-06-26] (phase1_gas) ASC 복제 활성화(SetIsReplicated)와 GAS 모듈 의존성 Public/Private 위치가 명시되지 않아 Build/Replication 구현이 흔들릴 수 있음 — 세부 구현 계약 보강 필요로 기록
 - [2026-06-26] (phase1_gas) phase1_gas는 GE 순수 C++를 확정했지만 coding.md에는 GE_ 에셋 관례가 남아 문서 간 계약이 충돌함 — 사람 승인 후 coding.md 보정 필요로 기록
+- [2026-06-26] (phase1_gas) coding.md 보정 후에도 phase1_gas 9항에는 GE 에셋 관례 충돌이 미해결로 남아 문서 상태가 어긋남 — phase1_gas 후속 정리 필요로 기록
+- [2026-06-26] (phase1_gas) GE 수치를 UPROPERTY(Config)로 둔다고 했지만 UCLASS Config 지정과 .ini 섹션 위치가 명시되지 않아 구현 방식이 갈릴 수 있음 — Config 계약 보강 필요로 기록

commit 281818b1f0a210073b01696a1aef738e7a906e87
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 22:05:33 2026 +0900

    docs(gas): GE C++ 워크플로 고정
    
    Codex가 BP 자식 에셋을 만들거나 에디터 값을 세팅할 수 없으므로, GameplayEffect는 순수 C++ UGE_ 클래스로 구현한다는 관례를 명확히 한다.
---
 docs/conventions/coding.md | 4 +---
 1 file changed, 1 insertion(+), 3 deletions(-)

diff --git a/docs/conventions/coding.md b/docs/conventions/coding.md
index d8d2702..8a61f24 100644
--- a/docs/conventions/coding.md
+++ b/docs/conventions/coding.md
@@ -29,9 +29,7 @@
 
 > 위 표는 초안이다. 어색하면 검토 시 수정. 핵심은 "한번 정하면 Codex가 일관되게 따른다"는 것.
 
-> 현재 구현 단계에서는 GameplayEffect를 우선 순수 C++ `UGE_` 클래스로 작성한다.
-> BP/DataAsset 기반 `GE_` 에셋은 콘텐츠/밸런스 튜닝 단계에서 필요해질 때 도입한다.
-> 따라서 Codex 구현 지시에서 별도 언급이 없으면 GE 에셋 생성을 전제로 하지 않는다.
+> 이 프로젝트의 GameplayEffect는 전부 순수 C++ `UGE_` 클래스로 작성한다. 이는 "아직 초기 단계라서"가 아니라, 구현을 전담하는 Codex가 BP 자식 에셋(`.uasset`)을 만들거나 에디터에서 값을 세팅할 수 없기 때문이다. 이 제약은 프로젝트가 진행돼도 자동으로 풀리지 않는다. BP/DataAsset 기반 `GE_` 에셋은 사람이 직접 에디터에서 콘텐츠/밸런스를 튜닝하기로 워크플로 자체를 바꾸는 시점에만 도입한다. 그 전까지 Codex 구현 지시에서 별도 언급이 없으면 GE 에셋 생성을 전제로 하지 않는다.
 
 ## 3. 파일/구조
 

commit bfcd05bf8fae1648693bdc42c768cf2876b802f8
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Fri Jun 26 22:08:04 2026 +0900

    docs(gas): Phase 1 GAS 설계 추가
    
    GAS Phase 1 구현 전에 ASC 배치, AttributeSet, C++ GameplayEffect, Config 기반 검증값과 네트워크 검증 계약을 문서화한다.
---
 docs/architecture/phase1_gas.md | 171 ++++++++++++++++++++++++++++++++++++++++
 1 file changed, 171 insertions(+)

diff --git a/docs/architecture/phase1_gas.md b/docs/architecture/phase1_gas.md
new file mode 100644
index 0000000..ee6f083
--- /dev/null
+++ b/docs/architecture/phase1_gas.md
@@ -0,0 +1,171 @@
+# GAS 척추 (ASC / AttributeSet / 기본 GE 흐름)
+
+## 1. 목적
+
+- ASC 배치, Replication Mode, AttributeSet, 기본 GameplayEffect 흐름을 구축해 GAS의 골격을 세우는 Phase 1 인프라.
+- `project.md`의 GAS 증명 목표(AttributeSet, GameplayEffect 흐름, ASC Replication Mode 선택과 그 근거 문서화) 전부를 담당. 네트워크 증명 목표 중 **GE 기반 데미지 흐름의 서버 권위·복제**도 같이 시연한다. GameplayAbility 자체(어빌리티 활성, Enhanced Input 바인딩)는 해당 없음(Phase 2).
+
+## 2. 설계 의도 / 핵심 결정
+
+- **ASC 위치는 PlayerState.** 이 프로젝트는 이미 Seamless Travel을 쓴다(`phase0_lobby.md`) — PlayerState는 레벨 전환을 넘어 보존되지만 Character는 전환마다 파괴·재생성된다. ASC를 Character에 두면 전환·리스폰(Phase 5 웨이브 사망 포함)마다 ASC를 재구성해야 하는데, 그걸 정당화할 근거가 없다.
+  - 탈락 대안: Character 배치. → Pawn 생명주기에 ASC/Attribute 상태가 종속되어, 리스폰이 있는 구조(웨이브 PvE)에서 매번 재초기화 비용이 발생. 탈락.
+
+- **`IAbilitySystemInterface`는 `AGDSPlayerState`와 `AGDSCharacter` 둘 다 구현한다.** `AGDSPlayerState::GetAbilitySystemComponent()`는 자신의 ASC를 직접 반환. `AGDSCharacter::GetAbilitySystemComponent()`는 자신의 PlayerState의 ASC로 **위임**(delegate)한다. GAS 표준 패턴(예: `UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)`, GameplayCue 라우팅 등)이 PlayerState와 Avatar 양쪽 어느 쪽으로 호출돼도 ASC를 찾을 수 있어야 하므로, ASC를 PlayerState에 두기로 한 결정(위 항목)에는 이 인터페이스 계약이 같이 따라온다 — 빠지면 "어느 쪽 액터로 물어보느냐"에 따라 ASC 조회가 null이 되는 경로가 생긴다.
+  - 탈락 대안: PlayerState만 구현. → Character를 통해 ASC를 찾는 호출 경로(다수의 GAS 헬퍼 함수가 이렇게 함)가 전부 깨짐. 탈락.
+
+- **ASC Replication Mode는 Mixed.** Co-op이라 동료의 Attribute 변화·GameplayCue는 모든 클라(시뮬레이션 프록시 포함)가 봐야 하는데, Minimal·Mixed 둘 다 이 요건은 만족한다. 갈리는 지점은 "소유 클라에게 GE 자체를 복제하는가" — Phase 2~3에서 어빌리티가 들어오면 클라이언트 예측(prediction) 경로가 필요해질 가능성이 높고, 예측은 소유 클라가 자신의 GE 적용 결과를 미리 알아야 가능하다. Minimal로 가면 그 경로가 막혀 나중에 Mode를 다시 바꿔야 한다.
+  - 탈락 대안: Full. → 시뮬레이션 프록시에게 불필요한 GE 디테일까지 복제해 대역폭을 낭비. `project.md`의 네트워크 최적화 증명 목표와 정면 충돌. 탈락.
+  - 탈락 대안: Minimal. → 소유 클라 GE 복제가 없어 향후 prediction 도입 시 Mode 재변경이 필요해짐(재작업). 탈락.
+
+- **ASC 초기화(`InitAbilityActorInfo`) 트리거는 서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`.** Owner(PlayerState)는 Avatar가 누군지 알 길이 없고, Character(Avatar)는 Possess·복제 시점에 자기 PlayerState를 확실히 들고 있다. 그래서 "Avatar 교체를 감지하는 쪽"이 트리거를 쥔다.
+  - 이 보장이 성립하려면 **ASC와 AttributeSet은 PlayerState 생성자에서 `CreateDefaultSubobject`로 생성**해야 한다(지연 생성 금지). 이 전제가 없으면 두 훅 시점에 ASC 존재가 보장되지 않는다.
+  - Avatar 교체(웨이브 리스폰 등) · 늦은 접속 · 비소유 클라 포함 — 셋 다 이 두 훅(서버 Possess는 교체마다 재발화, 클라 OnRep은 그 클라에 relevant해지는 모든 시점에 재발화)으로 자연히 커버된다. 별도 메커니즘 불필요.
+  - PlayerController는 이 계약 범위 밖이다. PC가 필요해지는 시점(어빌리티 입력 바인딩)은 Phase 2.
+  - 탈락 대안: 서버가 초기화 완료를 별도 RPC로 클라에 통지. → 클라는 이미 `OnRep_PlayerState`로 자체 판단 가능하므로 RPC 왕복만 추가. 탈락.
+
+- **ASC↔Character 연결은 "범용 Attribute 변경 델리게이트 구독"이라는 채널로 가고, 지금 실제로 그 채널에 거는 내용은 MovementSpeed → `CharacterMovementComponent::MaxWalkSpeed` 하나뿐이다.** 채널 자체를 MovementSpeed 전용으로 좁혀서 만들면, Phase 5에서 Health=0 반응이 필요해질 때 Codex가 또 다른 패턴(Tick 폴링, AttributeSet→Character 직접 호출 등)을 새로 발명할 위험이 있다. 채널을 범용으로 두면 Phase 5는 같은 채널에 구독만 추가하면 된다.
+  - 구독 시점은 위 Init 훅과 동일한 자리(서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`).
+  - **이 구독은 중복 방지가 보장돼야 한다.** `InitAbilityActorInfo` 자체는 같은 Avatar로 재호출돼도 ASC가 내부적으로 안전하게 처리하지만(앞서 결정한 대로 별도 가드 불필요), MovementSpeed 델리게이트 구독은 **우리가 직접 작성하는 코드**라 그 보장이 없다. `PossessedBy`/`OnRep_PlayerState`가 같은 Avatar에 대해 다시 불릴 가능성(재초기화, 재접속 등)에 대비해, 매번 무조건 새로 바인딩하지 말고 **재구독 전 기존 바인딩을 해제**(`RemoveAll(this)` 등)하거나 **멱등성 있는 구독 방식**(`AddUniqueDynamic` 등 중복 바인딩을 막는 API)을 쓴다. 그렇지 않으면 같은 콜백이 중복 바인딩돼 MovementSpeed 변경 시 `MaxWalkSpeed` 갱신이 여러 번 실행되는 등의 부작용이 생긴다.
+
+- **AttributeSet은 단일 클래스(`UCombatAttributeSet`)로 간다.** 지금 규모(Health/MaxHealth/Shield/Armor/MovementSpeed/Damage)에서 여러 클래스로 분리할 근거가 없다. 커지면 그때 분리한다. (`coding.md` 2항의 단일 예시 `UCombatAttributeSet`과도 일치.)
+
+- **Attribute 목록: Health, MaxHealth, Shield, Armor, MovementSpeed, Damage(Meta Attribute).**
+  - Health/MaxHealth를 분리하는 이유: 둘이 같은 값이면 "최대치 자체를 바꾸는 버프" 같은 GE를 표현할 수 없다.
+  - Shield와 Armor는 동의어가 아니라 서로 다른 메커니즘이다 — Shield는 자동 회복형 풀, Armor는 인스턴스당 데미지 자체를 줄여주는 풀. (오버워치 기준 확인: Shield는 무피격 시간 후 초당 회복, Armor는 회복 없이 맞을 때마다 고정량+%cap으로 데미지를 줄임.) 단순 % 감소 버프보다 훨씬 깊은 GAS 증명 소재라 채택.
+  - Damage는 Meta Attribute — 저장 상태가 아니라 "들어온 데미지를 계산해 다른 Attribute에 반영"하는 계산 통로. `PostGameplayEffectExecute` 처리 후 반드시 0으로 리셋.
+  - 보류했던 후보(Stamina, AttackPower/DefensePower, CooldownReduction 등)는 그 자원/배율 모델이 아직 결정되지 않은 채로 만들면 추측이 되므로 이 시스템에서 다루지 않는다(별도로 기록도 하지 않음 — 해당 시스템 설계 시 자연히 다뤄질 것).
+
+- **검증용 초기값(Phase 1 한정 placeholder, 최종 밸런스 아님).** P0/P1 통과 기준을 사람이 로그로 판정하려면 "정상이면 어떤 값이 찍혀야 하는가"의 기준점이 있어야 한다. 그래서 최종 밸런스와 분리된 **검증용 임시값**을 지금 못 박는다(추후 밸런스 패스에서 자유롭게 교체):
+  - 초기 Attribute 값: `MaxHealth = 100`, `Health = 100`, `Shield = 50`, `Armor = 50`, `MovementSpeed = 600`(UE 기본 TPS 보행속도와 동일선상)
+  - `UGE_TestDamage` 1회 적용량: SetByCaller(`Data.Damage.Test`)로 Damage(Meta)에 `40` 주입
+  - Armor 캐스케이드 임시 수치: 인스턴스당 고정감소 `7`, 최대 `50%` cap (오버워치 수치를 그대로 시작점으로 채용)
+  - `UGE_TestSpeedBuff`: MovementSpeed에 `-300` Instant 적용(편도 1회성 감속 — 복귀 로직 없음, "변화가 보이는가"만 확인하면 되므로 토글/타이머 불필요)
+  - 이 값들로는 데미지 테스트를 약 2~3회 누적 입력하면 Shield→Armor가 차례로 소진되고 Health까지 영향이 보일 정도의 규모다. 정확한 횟수는 Armor 처리 디테일에 따라 달라질 수 있으니 사람이 로그로 직접 확인한다.
+
+- **Attribute 클램프 규칙.**
+  - `Health`: `[0, MaxHealth]`로 클램프(`PreAttributeChange`/`PostGameplayEffectExecute`에서).
+  - `Shield`, `Armor`: 하한 `0`만 클램프. 상한은 지금 별도 Max Attribute가 없어 의미가 없다(증가시키는 GE가 이 Phase에 없음) — 추후 MaxShield/MaxArmor가 필요해지면 그때 추가.
+  - `MovementSpeed`: 하한 **`100`**으로 클램프(내가 정함). 0 또는 음수로 떨어지면 캐릭터가 완전히 멈추거나 Movement/애니메이션이 비정상 동작할 위험이 있어 안전망이 필요하다. `100`은 기본값 `600`의 1/6 수준 — 디버프가 "느려졌다"는 걸 충분히 체감하게 하면서도 완전 정지는 막는 임계값으로 잡았다. 상한은 이 Phase의 버프(`GE_TestSpeedBuff`가 감속만 함)로는 도달할 일이 없어 지금 정하지 않는다.
+- **ASC 복제 활성화.** `AGDSPlayerState`에서 ASC를 `CreateDefaultSubobject`한 직후 `SetIsReplicated(true)`를 명시적으로 호출한다. `UAbilitySystemComponent`는 컴포넌트 자체 복제 플래그를 자동으로 켜주지 않으므로, 빠뜨리면 Attribute 자체가 정의돼 있어도 복제가 안 되는 조용한 실패가 난다.
+- **`Build.cs` 모듈 의존성 위치는 `PublicDependencyModuleNames`.** `AGDSPlayerState`/`AGDSCharacter` 헤더가 `UAbilitySystemComponent*`/`UCombatAttributeSet*` 타입을 멤버로 선언해 외부에 노출하므로, 이 타입을 정의하는 `GameplayAbilities`/`GameplayTags`/`GameplayTasks` 모듈은 `Private`이 아니라 `Public`으로 가야 그 헤더를 include하는 다른 모듈에서도 타입이 보인다.
+
+- **Damage 캐스케이드 순서는 Shield → Armor → Health(소모), 회복은 역순.** 오버워치의 4단계 HP 모델 중 GDS에 필요한 3단을 그대로 채용.
+  - **Armor의 흡수 방식은 인스턴스당 고정감소 + %cap, 오버플로는 다음 단(Health)으로 넘어간다.** 단순 "Armor 있으면 데미지 20% 감소" 같은 전역 % 버프보다, 현재 Armor 값을 읽고 감소를 계산하고 차감하고 오버플로를 넘기는 다단 Attribute 상호작용이 Execution 로직의 깊이를 보여준다. 구체 수치(고정감소량, %cap)는 밸런스 튜닝값이라 `UPROPERTY(Config)`로 분리해 추후 자유롭게 바꿀 수 있게 하되, **검증을 위한 시작값은 위 "검증용 초기값" 항목에서 확정**(고정감소 7, cap 50%) — 빈 채로 두면 P0/P1 통과 기준을 사람이 판정할 기준점이 없어진다.
+  - **캐스케이드 산식을 하나로 고정한다(아래 순서를 그대로 구현, 다른 해석 금지):**
+    1. `D` = 들어온 raw Damage(Meta Attribute 값)
+    2. **Shield 단계:** `shieldAbsorbed = min(CurrentShield, D)`; `CurrentShield -= shieldAbsorbed`; `D -= shieldAbsorbed`. `D <= 0`이면 여기서 멈춤(Armor/Health 불변).
+    3. **Armor 단계(`D > 0`일 때만):** `reduction = min(FlatReduction, D * CapPercent)`(검증값: `min(7, D * 0.5)`); `effectiveD = D - reduction`; `armorAbsorbed = min(CurrentArmor, effectiveD)`; `CurrentArmor -= armorAbsorbed`; `D = effectiveD - armorAbsorbed`. `D <= 0`이면 여기서 멈춤(Health 불변, `reduction`만큼은 그냥 사라진 데미지로 취급 — 다음 단으로 안 넘어감).
+    4. **Health 단계(`D > 0`일 때만):** `CurrentHealth -= D` (클램프는 아래 클램프 규칙 적용).
+  - **단순화 지점(의도적):** 오버워치 원본은 한 데미지 인스턴스가 Armor를 막 소진시키는 경계에서 reduction을 "비례 조정"하는 예외 규칙이 있는데, 그건 다수의 작은 인스턴스가 동시에 오가는 PvP 밸런스용 디테일이다. GDS는 Phase 1에서 그 정밀도가 증명 목표에 기여하지 않으므로(Execution 흐름의 깊이를 보여주는 게 목적, 정확한 OW 리메이크가 목적이 아님) 그 경계 보정은 빼고 위 4단계로 단순화했다.
+  - 탈락 대안: 단순 % 감소형 Armor. → GE Modifier 하나로 끝나 Execution Calculation을 거의 안 써도 됨. 증명 깊이가 안 나와 탈락.
+
+- **Shield의 자동 회복은 Phase 3로 이관. Phase 1의 Shield는 회복 없는 정적 풀.** 자동 회복은 Periodic GameplayEffect(지속시간 기반 반복 적용)가 필요한데, 이는 Phase 1이 검증하려는 "GE 1회 적용 → Attribute 변경" 흐름과는 다른 종류의 패턴이다. Phase 3(상태이상/버프 흐름)에서 본격적으로 다룰 때 같이 정한다.
+
+- **Phase 1 검증용 GE 트리거는 임시 디버그 입력(Phase 0의 임시 Input Mapping Context 확장) → Server RPC → 서버가 Self에게 테스트 GE 적용.** GameplayAbility/Enhanced Input 본 바인딩이 아직 없는 상태에서 GE 흐름을 보여줘야 하므로, Phase 0이 입력 시스템 부재 문제를 풀었던 것과 같은 패턴(임시 입력, 이후 폐기)을 재사용한다. Self 대상으로 한정해 타겟팅(라인트레이스 등, Phase 2~3 영역)이 검증에 섞이지 않게 한다. Server RPC를 거치는 이유는 클라가 직접 Attribute/GE를 바꾸면 `constraints.md` 1항 위반이고, `project.md`의 RPC 구조 증명 기회도 날리기 때문.
+  - **이 입력 바인딩과 Server RPC는 모두 `AGDSCharacter`가 가진다 — `AGDSPlayerController`가 아니다.** Phase 0에서 이미 WASD/Mouse2D 입력이 `AGDSCharacter`의 임시 Input Mapping Context에 바인딩되는 걸로 실제 구현됐다(`phase0_lobby.md` 6항). 이 Phase의 테스트 액션 2개는 그 **같은 Input Mapping Context를 확장**하는 것이고, 입력을 받는 쪽(Character)이 그대로 Server RPC도 쏘는 게 자연스럽다 — 입력은 Character가 받고 RPC는 PlayerController가 쏘는 식으로 액터를 갈라놓으면 불필요한 액터 간 홉이 하나 늘어난다. `AGDSCharacter`는 자신의 `GetPlayerState<AGDSPlayerState>()`를 통해 ASC를 얻어 GE를 적용한다.
+  - `AGDSPlayerController`는 이 시스템에서 맡는 역할이 없다. PC의 역할은 Phase 2(어빌리티 입력 바인딩, GAS 표준 패턴상 PC를 거칠 수도 있음)에서 다시 검토한다.
+  - 탈락 대안: 콘솔 명령으로 직접 GE 적용. → 서버 권위 경로(RPC)를 안 거쳐 RPC 구조 증명 기회를 날림. 탈락.
+  - 이 임시 입력·테스트 GE 자산은 Phase 2에서 진짜 어빌리티 바인딩이 들어오면 폐기 대상이다(`phase0_lobby.md` 6항과 같은 원칙).
+
+- **GameplayEffect는 순수 C++ 클래스로만 만든다. BP 에셋을 따로 만들지 않는다.** 일반적인 GAS 권장 방식은 맞다 — C++ 베이스 클래스 + BP 자식 에셋으로 디자이너가 수치를 에디터에서 튜닝하는 게 보통의 정석. **하지만 그 권장은 "에디터를 여는 사람이 있다"는 전제가 깔려 있고, 이 프로젝트의 실제 파이프라인은 그 전제가 깨져 있다.** Codex는 텍스트/코드만 다루는 에이전트라 `.uasset`(BP 자식 클래스)을 만들거나 그 안의 Details 패널 값을 세팅하는 에디터 작업을 할 수 없다 — 그건 사람(YH)이 직접 에디터를 열어야 하는 수작업이고, 그 순간 "구현은 전부 Codex" 파이프라인이 깨진다. 그래서 일반 권장과 다르게, **이 프로젝트의 모든 GameplayEffect는 코드만으로 완결**되는 형태로 간다.
+  - **`UGE_TestDamage`** — Damage(Meta) Modifier의 Magnitude를 **SetByCaller**(태그 `Data.Damage.Test`)로 선언. 실제 수치(`40`)는 GE 자체가 아니라, GE를 적용하는 쪽(`AGDSCharacter`의 RPC 핸들러)이 `FGameplayEffectSpecHandle`을 만들 때 코드로 주입한다. 이 방식이 자연스러운 이유: Damage는 본질적으로 "매번 다른 값이 들어오는" Attribute라(나중에 무기마다 데미지가 다름, Phase 3), SetByCaller가 GAS에서 이런 경우의 표준 패턴이다.
+  - **`UGE_TestSpeedBuff`** — MovementSpeed Modifier의 Magnitude는 GE 클래스 자신의 `UPROPERTY(EditDefaultsOnly, Config)` float(`-300`)을 생성자에서 `FScalableFloat`로 사용. 고정값 테스트라 SetByCaller까지는 불필요.
+  - 두 경우 모두 매직 넘버는 `UPROPERTY(Config)`로 빼서 `.ini`에 둔다(`constraints.md` 6항) — 이건 텍스트 파일이라 Codex가 직접 쓸 수 있고, 에디터가 필요 없다. `UGE_TestDamage`/`UGE_TestSpeedBuff`는 `UCLASS(Config=Game)`로 선언하고, 검증용 값은 `Config/DefaultGame.ini`의 각 클래스 섹션에 둔다.
+  - 탈락 대안: C++ 클래스 + BP 자식 에셋(`GE_TestDamage` 등). → Codex가 만들 수 없는 산출물이 생겨 파이프라인이 끊김. 탈락.
+  - **참고:** `coding.md` 2항 네이밍 표의 "GameplayEffect 에셋(BP/DataAsset) — `GE_`" 행은 이 결정과 충돌한다. 이 프로젝트의 모든 GE가 앞으로도 이 방식(순수 C++)으로 갈 거라면 그 행 자체가 이 프로젝트엔 안 맞는 관례라, `coding.md` 수정이 필요할 수 있다 — 별도로 확인받을 것(9항 참조).
+
+- **GameplayTag는 Phase 1에서 GE 식별용으로만 쓴다(`Effect.Damage.Test`).** 상태 판정·상호작용을 위한 태그 시스템(Death, 상태이상 등)은 Phase 3 영역이라, 지금 만들면 그 시스템의 축소판을 미리 만들게 되어 나중에 구조가 부딪힌다.
+  - **태그 등록 방식은 네이티브 C++ 매크로(`UE_DEFINE_GAMEPLAY_TAG` 계열).** `constraints.md` 6항의 "C++ 우선" 원칙과 일치.
+  - 탈락 대안: DataTable 기반 태그 등록. → Blueprint 친화적이지만 C++ 우선 원칙과 안 맞음. 탈락.
+
+## 3. 구조
+
+- **`AGDSPlayerState`** — ASC(`UAbilitySystemComponent`, 생성자에서 `CreateDefaultSubobject`) + `UCombatAttributeSet` 소유. Replication Mode = Mixed로 설정. `IAbilitySystemInterface` 구현(자신의 ASC를 직접 반환).
+- **`UCombatAttributeSet`** — Health/MaxHealth/Shield/Armor/MovementSpeed/Damage(Meta) 선언, 복제 등록(Damage 제외), Damage 캐스케이드 로직(Shield→Armor→Health) 담당.
+- **`AGDSCharacter`** — `IAbilitySystemInterface` 구현(자신의 PlayerState의 ASC로 위임). 서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`에서 ASC `InitAbilityActorInfo` 호출 + MovementSpeed 변경 델리게이트 구독·`CharacterMovementComponent::MaxWalkSpeed` 갱신 책임. Phase 0의 임시 Input Mapping Context(WASD/Mouse2D)를 그대로 소유하며, 이 Phase의 테스트 액션 2개와 그에 대응하는 Server RPC(`ServerApplyTestDamage`/`ServerApplyTestSpeedEffect`)도 이 클래스가 가진다.
+- **`UGE_TestDamage` / `UGE_TestSpeedBuff`** — Phase 1 검증용 임시 GameplayEffect, 순수 C++ 클래스(BP 자식 에셋 없음). `UGE_TestDamage`는 Damage(Meta) Modifier를 SetByCaller(`Data.Damage.Test`)로 선언하고 실제 수치는 적용 시점에 `AGDSCharacter`가 주입한다. `UGE_TestSpeedBuff`는 MovementSpeed Modifier 수치를 `UPROPERTY(Config)`로 자체 보유한다. Phase 2에서 진짜 어빌리티 GE로 대체되며 이 둘은 폐기.
+- **`AGDSPlayerController`** — 이 시스템에서 맡는 역할 없음. 입력·RPC는 모두 `AGDSCharacter`가 가진다(위 참조). PC의 역할은 Phase 2에서 재검토.
+
+**관계:** PlayerState가 ASC/AttributeSet을 소유한다. Character는 자신의 PlayerState를 통해 ASC에 접근해 Init을 트리거하고, MovementSpeed 변경을 구독해 자신의 Movement 컴포넌트에 반영한다. Character는 동시에 Phase 0부터 이어진 입력 소유자이기도 해서, 이 Phase의 테스트 입력을 직접 받아 Server RPC로 서버에 GE 적용을 요청한다(GE 판정 자체에는 개입하지 않음 — 판정은 항상 서버의 AttributeSet/GE 쪽에서 일어남). PlayerController는 이 시스템에서 어느 쪽에도 끼지 않는다.
+
+## 4. 데이터 / 실행 흐름
+
+- **Possess/초기화:** 서버 `PossessedBy` → ASC `InitAbilityActorInfo`(서버) → PlayerState 복제 → 클라 `OnRep_PlayerState` → 각 클라 ASC `InitAbilityActorInfo`(클라)
+- **MovementSpeed 변경 전파:** 어떤 GE가 MovementSpeed Attribute를 바꾸면 → Attribute 변경 델리게이트 발화 → Character가 `MaxWalkSpeed` 갱신 (서버·클라 각자, 자신의 Init 시점에 구독한 콜백으로)
+- **데미지 테스트:** 클라 입력 → Character의 `ServerApplyTestDamage` RPC → 서버가 `UGE_TestDamage` Spec 생성 + SetByCaller(`Data.Damage.Test`)로 `40` 주입 → ASC에 적용 → Damage(Meta)에 값 반영 → `PostGameplayEffectExecute`에서 Shield 흡수 → 남으면 Armor(고정감소+cap) 흡수 → 남으면 Health 차감 → Damage Meta 0 리셋 → Shield/Armor/Health 복제 → 각 클라 로그 반영
+- **속도 테스트:** 클라 입력 → Character의 Server RPC → `UGE_TestSpeedBuff` 적용 → MovementSpeed Instant 변경(클램프 적용) → 복제 → MovementSpeed 델리게이트 발화 → 각 클라(소유자 포함) Character가 `MaxWalkSpeed` 갱신
+
+## 5. 네트워크 측면
+
+- **서버 권위:** GE 적용 전체, AttributeSet 캐스케이드 계산(Shield→Armor→Health), ASC Init 트리거(서버 측 `PossessedBy`), RPC validation.
+- **복제:** Health/MaxHealth/Shield/Armor/MovementSpeed 모두 `COND_None`으로 전체 클라이언트에 복제 — Co-op이라 동료 상태를 전원이 봐야 하므로 숨길 이유가 없다. Damage는 Meta Attribute라 복제하지 않는다(연산 통로일 뿐 저장 상태가 아님).
+- **복제 조건(`ReplicationCondition`):** 이 시스템에서는 쓰지 않는다. 조건부 선택적 복제가 의미를 갖는 시점은 Phase 4(스캐너/마킹)이며, 지금은 전원 공개 정보를 전제로 한다.
+- **RPC:** `ServerApplyTestDamage` / `ServerApplyTestSpeedEffect`(또는 종류 구분 파라미터를 포함한 단일 RPC — 정확한 분리 방식은 Codex 판단) — **`AGDSCharacter`가 소유하는** Server RPC, Self 대상만이라 별도 대상 검증은 불필요하지만 `WithValidation` 형식은 갖춘다.
+- **클라이언트 예측:** 없음. Mixed Mode를 택한 이유 중 하나가 "향후 prediction 경로를 막지 않기 위함"이지만, Phase 1 자체에서 prediction을 구현하지는 않는다.
+- **최적화:** 해당 없음(Phase 6 대상). Relevancy/Dormancy/Lag Compensation은 이 시스템 스코프 밖.
+- **로깅(검증 근거):** `coding.md` 5항 원칙을 이 시스템에 적용한 구체 목록 —
+  - 서버: GE 적용 시점에 영향받는 Shield/Armor/Health **세 Attribute 각각의 before→after** (캐스케이드 중간 단계를 안 찍으면 오버플로가 맞게 일어났는지 검증 불가)
+  - 클라(전원): Attribute 변경 델리게이트/`OnRep_` 수신 시점에 동일한 세 Attribute 값
+  - RPC validation 통과/거부 결과
+  - 모두 핫패스가 아니므로 기본 `Log` verbosity로 충분.
+
+## 6. 시스템 고유 제약
+
+- ASC와 `UCombatAttributeSet`은 `AGDSPlayerState` 생성자에서 `CreateDefaultSubobject`로 생성한다. `BeginPlay` 등 지연 생성 금지 — Init 동기화 안전성 전제가 이 생성 시점에 의존한다.
+- ASC 생성 직후 `SetIsReplicated(true)`를 명시적으로 호출한다 — `UAbilitySystemComponent`는 자동으로 복제되지 않는다.
+- **이 프로젝트 모듈의 `Build.cs`에 `GameplayAbilities`/`GameplayTags`/`GameplayTasks` 모듈 의존성을 `PublicDependencyModuleNames`로 추가해야 한다.** Phase 0은 GAS를 안 써서 필요 없었지만, 이 시스템이 GAS를 처음 끌어오는 지점이라 지금 추가가 필요하다(P0의 첫 단계로 들어감). `Private`이 아니라 `Public`인 이유는 `AGDSPlayerState`/`AGDSCharacter` 헤더가 ASC/AttributeSet 타입을 멤버로 노출하기 때문.
+- **GameplayEffect는 항상 순수 C++ 클래스로 만든다. BP 자식 에셋을 만들지 않는다.** Codex가 에디터 GUI 작업을 할 수 없다는 이 프로젝트의 운영 제약 때문(2항 참조). Damage처럼 매번 값이 달라지는 Modifier는 SetByCaller로, 고정값 Modifier는 `UPROPERTY(Config)`로 받는다. Config를 쓰는 GameplayEffect 클래스는 `UCLASS(Config=Game)`로 선언하고, 값은 `Config/DefaultGame.ini`의 해당 클래스 섹션에 둔다.
+- `InitAbilityActorInfo` 호출은 서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`로 한정한다. `PlayerController`에서 호출하지 않는다(Phase 2 책임과 혼동 방지).
+- Attribute 값은 항상 GameplayEffect를 통해서만 바뀐다(`constraints.md` 2항). Phase 1의 테스트 트리거도 GE를 통해 적용하며, `PlayerController`/`Character`에서 Attribute를 직접 setter로 바꾸지 않는다.
+- **Attribute 클램프:** `Health`는 `[0, MaxHealth]`, `Shield`/`Armor`는 하한 `0`만, `MovementSpeed`는 하한 `100`. 전부 `PreAttributeChange`/`PostGameplayEffectExecute`에서 처리(상세 근거는 2항).
+- Damage(Meta) Attribute는 저장 상태가 아니다 — `PostGameplayEffectExecute` 처리 후 반드시 0으로 리셋한다. 리셋을 빠뜨리면 다음 GE 적용 시 값이 누적 오염된다.
+- Armor 캐스케이드의 고정감소량·%cap은 매직 넘버로 하드코딩하지 않고 `UPROPERTY(Config)`로 분리한다(`constraints.md` 6항). 검증용 시작값은 2항에 명시(고정감소 7, cap 50%).
+- **Attribute 선언은 GAS 표준 매크로 패턴을 그대로 따른다.** 헤더에서 `ATTRIBUTE_ACCESSORS` 매크로로 각 Attribute의 접근자(Getter/Setter/AttributeStatic)를 생성하고, `GetLifetimeReplicatedProps`에서 `DOREPLIFETIME_CONDITION_NOTIFY`로 복제 등록(`COND_None`, `REPNOTIFY_Always`)하고, 각 `OnRep_` 함수 안에서 `GAMEPLAYATTRIBUTE_REPNOTIFY` 매크로를 호출한다. 이 프로젝트 최초의 AttributeSet이라 정해진 관례가 없던 지점이었고, 여기서 정한 패턴이 이후 모든 AttributeSet(Phase 3 확장, Phase 5 AI 등)의 기준이 된다(`AGENTS.md` 1항).
+- 이 Phase의 임시 디버그 입력과 `UGE_TestDamage`/`UGE_TestSpeedBuff`는 Phase 2에서 진짜 어빌리티가 들어오면 폐기 대상이다. 이 임시 구조를 확장하지 않는다(`phase0_lobby.md` 6항과 같은 원칙).
+- AttributeSet의 Attribute 변경 델리게이트 같은 비동기/지연 콜백에 람다를 바인딩할 때는 `constraints.md` 5항대로 `TWeakObjectPtr` 캡처 + 유효성 검증을 따른다. MovementSpeed 델리게이트 구독은 추가로 2항에 명시한 중복 구독 방지 규칙을 따른다.
+
+## 7. 구현 지시 (Codex용)
+
+### P0 — ASC/AttributeSet 골격 + Init 동기화 (값 변화 없이 존재·복제·초기화 확인)
+- **만들 것 (순서):**
+  1) 프로젝트 모듈 `Build.cs`의 `PublicDependencyModuleNames`에 `GameplayAbilities`/`GameplayTags`/`GameplayTasks` 추가
+  2) `UCombatAttributeSet` 정의 — Health/MaxHealth/Shield/Armor/MovementSpeed/Damage(Meta) 선언(표준 `ATTRIBUTE_ACCESSORS` 매크로로 접근자 생성), `GetLifetimeReplicatedProps`에서 `DOREPLIFETIME_CONDITION_NOTIFY`로 등록(Damage 제외, `COND_None`/`REPNOTIFY_Always`), 각 `OnRep_`에서 `GAMEPLAYATTRIBUTE_REPNOTIFY` 호출, 초기값 설정(`MaxHealth=100, Health=100, Shield=50, Armor=50, MovementSpeed=600`), 클램프 구현(`Health∈[0,MaxHealth]`, `Shield/Armor≥0`, `MovementSpeed≥100`)
+  3) `AGDSPlayerState` 생성자에 ASC(`CreateDefaultSubobject` + `SetIsReplicated(true)`) + `UCombatAttributeSet` 추가, Replication Mode = Mixed 설정, `IAbilitySystemInterface` 구현(자신의 ASC 반환)
+  4) `AGDSCharacter`에 `IAbilitySystemInterface` 구현(자신의 PlayerState ASC로 위임) + `PossessedBy`(서버) / `OnRep_PlayerState`(클라 전원)에서 `InitAbilityActorInfo(PlayerState, this)` 호출
+  5) 같은 자리에서 MovementSpeed Attribute 변경 델리게이트 구독 → `CharacterMovementComponent::MaxWalkSpeed` 갱신 콜백 등록(지금은 트리거할 게 없어 연결만 존재) — 재호출 시 중복 바인딩되지 않도록 재구독 전 해제 또는 멱등 구독 방식 적용
+  6) `Effect.Damage.Test` GameplayTag 네이티브 등록
+  7) 로그: Init 호출 시점 Owner/Avatar 확인 + 초기 Attribute 6종 값 (서버/각 클라)
+- **멈춤 지점:** 여기까지 하고 멈춰 보고한다.
+- **통과 기준 (사람 확인):** 컴파일 통과 + DS 1 + 클라 2 PIE 접속 → 서버 로그와 각 클라 로그에 `InitAbilityActorInfo` 호출(Owner=PlayerState, Avatar=Character) 확인 + Health/MaxHealth/Shield/Armor 초기값이 서버·양쪽 클라 로그에서 위에서 정한 검증값(`100/100/50/50`)과 일치하는지 확인(초기 복제 검증)
+
+### P1 — 테스트 GE 트리거 → 전체 파이프라인 + 복제 확인 (Phase 1 본 게이트)
+- **만들 것 (순서):**
+  1) `AGDSCharacter`가 가진 기존 임시 Input Mapping Context에 테스트 액션 2개 추가(데미지 테스트 / 속도 테스트)
+  2) `AGDSCharacter`에 테스트용 Server RPC 추가(`ServerApplyTestDamage`/`ServerApplyTestSpeedEffect` 또는 종류 구분 파라미터 포함 1개 — 형식은 Codex 판단) — Self 대상만이라 별도 대상 검증은 불필요하지만 `WithValidation` 형식은 갖춤. `AGDSPlayerController`에는 추가하지 않는다.
+  3) `Data.Damage.Test` GameplayTag 네이티브 등록(SetByCaller 키)
+  4) `UGE_TestDamage` C++ 클래스 작성(BP 에셋 없음, `UCLASS(Config=Game)`) — Damage(Meta) Modifier를 `Data.Damage.Test` SetByCaller로 선언. 테스트 데미지 값 `40`은 `UPROPERTY(Config)`로 보유하고 `Config/DefaultGame.ini`의 `UGE_TestDamage` 클래스 섹션에 둔다. `ServerApplyTestDamage` 핸들러에서 Spec 생성 시 해당 Config 값을 태그로 주입 후 적용
+  5) `UCombatAttributeSet`에 캐스케이드 로직 구현 — 2항에 고정한 산식 그대로(Shield 흡수 → Armor 고정감소 `7`/cap `50%` 흡수 → 오버플로 Health → Damage Meta 0 리셋)
+  6) `UGE_TestSpeedBuff` C++ 클래스 작성(BP 에셋 없음, `UCLASS(Config=Game)`) — MovementSpeed Instant Modifier `-300`을 `UPROPERTY(Config)`로 보유하고 `Config/DefaultGame.ini`의 `UGE_TestSpeedBuff` 클래스 섹션에 둔다(Duration형 버프는 Phase 3 영역, 지금은 연결 확인만)
+  7) 로그: GE 적용 시점 Shield/Armor/Health before→after, MovementSpeed before→after, RPC validation 결과 (서버+클라)
+- **멈춤 지점:** 여기까지 하고 멈춰 보고한다.
+- **통과 기준 (사람 확인):**
+  (a) 데미지 테스트 키 입력 → 서버 로그에 Shield→Armor→Health 캐스케이드 값 확인 + 본인/타 클라 로그에 동일 최종값 복제 확인
+  (b) 데미지를 누적 입력해(검증용 수치 기준 약 2~3회) Shield/Armor를 소진시켜 Health까지 깎이는 오버플로 동작 확인
+  (c) 속도 테스트 키 입력 → PIE 화면에서 실제 이동 속도 변화(600→300)가 보이고, 그 값이 양쪽 클라 로그에서 일치 확인
+
+## 8. 시스템 인수 검증 (전체)
+
+- **시나리오:** DS 1 + 클라 2 PIE → 두 클라 접속 → (P0) 서버/양쪽 클라 로그로 ASC Init 호출과 초기 Attribute 복제 일치 확인 → (P1) 한 클라에서 데미지 테스트 키 입력(단발) → Shield→Armor→Health 캐스케이드가 서버/양쪽 클라 로그에서 일치 → 같은 클라에서 데미지 테스트를 누적 입력해 Shield/Armor를 소진시켜 Health까지 깎이는 오버플로 확인 → 속도 테스트 키 입력 → 실제 이동 속도 변화가 PIE 화면에서 보이고 로그 일치 확인.
+- **서버-클라 검증:** 로그에 `[Server]`/`[Client]` 태그로 Init 호출, GE 적용, Attribute before/after, RPC validation 시점을 모두 남겨 PIE 멀티 인스턴스에서 교차 확인한다.
+- **증명 목표 시연 여부:** AttributeSet 설계, GameplayEffect 적용 흐름, ASC Replication Mode(Mixed)와 그 근거, 서버 권위적 GE 적용, RPC 구조(서버 검증)를 시연한다. GameplayTag는 식별용으로만 등장(상태 판정 시스템은 Phase 3). 클라이언트 예측·Relevancy·Dormancy·Lag Compensation·`ReplicationCondition`은 이 시스템 스코프가 아니다(각각 이후 Phase).
+
+## 9. 미해결 / 추후
+
+- Shield 자동 회복(Periodic GameplayEffect) — Phase 3
+- Armor 고정감소량·%cap(현재 검증용 `7`/`50%`), Shield/Armor/Health 기본값(현재 검증용 `50/50/100`) 등 — 이 값들은 P0/P1 검증을 위한 시작값으로 확정했을 뿐 최종 밸런스가 아니다. 실제 밸런스 수치는 추후 튜닝.
+- Health=0 도달 시 사망 처리(Character 반응 콘텐츠) — Phase 5. 단 연결 채널(Attribute 변경 델리게이트 구독)은 Phase 1에서 이미 마련한 것을 그대로 재사용 — 새 패턴 발명 불필요
+- MovementSpeed Duration형 버프/디버프(상태이상으로서의 속도 변화) — Phase 3
+- GameplayAbility 활성화·Enhanced Input 본 바인딩 — Phase 2. 그 시점에 이 Phase의 임시 디버그 입력·`UGE_TestDamage`/`UGE_TestSpeedBuff` 폐기
+- **`coding.md` 2항의 "GameplayEffect 에셋(BP/DataAsset) — `GE_`" 행이 이 시스템의 "GE는 순수 C++만" 결정과 충돌한다.** 이 프로젝트의 모든 GE가 앞으로도 BP 자식 에셋 없이 갈 거라면 그 행을 빼거나 수정해야 함 — 사람 승인 필요, 임의로 고치지 않음.
+- AI(Phase 5)용 ASC Replication Mode(Minimal일 가능성이 높음, owning client가 없는 구조라서) — Phase 5에서 별도 결정
+- 팀/분대 단위로 Attribute 가시성이 갈리는 경우 — 이 시스템은 전원 공개 복제를 전제로 하며, Phase 4(스캐너/마킹)에서 재검토

commit cd90ea18a2f2b4a8715e4b8a319492b337126283
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Sat Jun 27 16:21:54 2026 +0900

    feat(gas): ASC와 AttributeSet 골격 추가
    
    Phase 1 P0에서 PlayerState 보존 생명주기에 ASC를 두고, Character가 Avatar 초기화와 MovementSpeed 반영만 맡도록 역할을 나눴다.
    
    초기 Attribute 복제와 Init 로그를 통해 Dedicated Server와 클라이언트 양쪽에서 GAS 골격이 같은 값으로 관측되게 한다.
---
 GDS.uproject                                       |  6 +-
 Source/GDS/GAS/Attributes/CombatAttributeSet.cpp   | 99 ++++++++++++++++++++++
 Source/GDS/GAS/Attributes/CombatAttributeSet.h     | 71 ++++++++++++++++
 Source/GDS/GAS/GDSGameplayTags.cpp                 |  5 ++
 Source/GDS/GAS/GDSGameplayTags.h                   |  7 ++
 Source/GDS/GDS.Build.cs                            |  2 +-
 .../GDS/GameFramework/Character/GDSCharacter.cpp   | 94 ++++++++++++++++++++
 Source/GDS/GameFramework/Character/GDSCharacter.h  | 12 ++-
 .../GameFramework/PlayerState/GDSPlayerState.cpp   | 21 +++++
 .../GDS/GameFramework/PlayerState/GDSPlayerState.h | 16 +++-
 10 files changed, 329 insertions(+), 4 deletions(-)

diff --git a/GDS.uproject b/GDS.uproject
index bdee873..f060f39 100644
--- a/GDS.uproject
+++ b/GDS.uproject
@@ -11,6 +11,10 @@
 		}
 	],
 	"Plugins": [
+		{
+			"Name": "GameplayAbilities",
+			"Enabled": true
+		},
 		{
 			"Name": "ModelingToolsEditorMode",
 			"Enabled": true,
@@ -19,4 +23,4 @@
 			]
 		}
 	]
-}
\ No newline at end of file
+}
diff --git a/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp b/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
new file mode 100644
index 0000000..d9332dc
--- /dev/null
+++ b/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
@@ -0,0 +1,99 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#include "GDS/GAS/Attributes/CombatAttributeSet.h"
+
+#include "GDS/GDS.h"
+#include "GameplayEffectExtension.h"
+#include "Net/UnrealNetwork.h"
+
+UCombatAttributeSet::UCombatAttributeSet()
+{
+	InitMaxHealth(100.0f);
+	InitHealth(100.0f);
+	InitShield(50.0f);
+	InitArmor(50.0f);
+	InitMovementSpeed(600.0f);
+	InitDamage(0.0f);
+}
+
+void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
+{
+	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
+
+	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Health, COND_None, REPNOTIFY_Always);
+	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
+	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Shield, COND_None, REPNOTIFY_Always);
+	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Armor, COND_None, REPNOTIFY_Always);
+	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
+}
+
+void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
+{
+	Super::PreAttributeChange(Attribute, NewValue);
+	ClampAttribute(Attribute, NewValue);
+}
+
+void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
+{
+	Super::PostGameplayEffectExecute(Data);
+	ClampCurrentAttributes();
+}
+
+void UCombatAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
+{
+	if (Attribute == GetHealthAttribute())
+	{
+		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
+	}
+	else if (Attribute == GetMaxHealthAttribute())
+	{
+		NewValue = FMath::Max(0.0f, NewValue);
+	}
+	else if (Attribute == GetShieldAttribute() || Attribute == GetArmorAttribute())
+	{
+		NewValue = FMath::Max(0.0f, NewValue);
+	}
+	else if (Attribute == GetMovementSpeedAttribute())
+	{
+		NewValue = FMath::Max(MinMovementSpeed, NewValue);
+	}
+}
+
+void UCombatAttributeSet::ClampCurrentAttributes()
+{
+	SetMaxHealth(FMath::Max(0.0f, GetMaxHealth()));
+	SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
+	SetShield(FMath::Max(0.0f, GetShield()));
+	SetArmor(FMath::Max(0.0f, GetArmor()));
+	SetMovementSpeed(FMath::Max(MinMovementSpeed, GetMovementSpeed()));
+}
+
+void UCombatAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
+{
+	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Health, OldHealth);
+	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: Health %.1f -> %.1f"), OldHealth.GetCurrentValue(), GetHealth());
+}
+
+void UCombatAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
+{
+	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxHealth, OldMaxHealth);
+	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: MaxHealth %.1f -> %.1f"), OldMaxHealth.GetCurrentValue(), GetMaxHealth());
+}
+
+void UCombatAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
+{
+	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Shield, OldShield);
+	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: Shield %.1f -> %.1f"), OldShield.GetCurrentValue(), GetShield());
+}
+
+void UCombatAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
+{
+	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Armor, OldArmor);
+	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: Armor %.1f -> %.1f"), OldArmor.GetCurrentValue(), GetArmor());
+}
+
+void UCombatAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
+{
+	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MovementSpeed, OldMovementSpeed);
+	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: MovementSpeed %.1f -> %.1f"), OldMovementSpeed.GetCurrentValue(), GetMovementSpeed());
+}
diff --git a/Source/GDS/GAS/Attributes/CombatAttributeSet.h b/Source/GDS/GAS/Attributes/CombatAttributeSet.h
new file mode 100644
index 0000000..6d590bb
--- /dev/null
+++ b/Source/GDS/GAS/Attributes/CombatAttributeSet.h
@@ -0,0 +1,71 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#pragma once
+
+#include "AbilitySystemComponent.h"
+#include "AttributeSet.h"
+#include "CombatAttributeSet.generated.h"
+
+#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
+	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
+	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
+	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
+	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
+
+UCLASS()
+class GDS_API UCombatAttributeSet : public UAttributeSet
+{
+	GENERATED_BODY()
+
+public:
+	UCombatAttributeSet();
+
+	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
+	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
+	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
+
+	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Combat")
+	FGameplayAttributeData Health;
+	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health)
+
+	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Combat")
+	FGameplayAttributeData MaxHealth;
+	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxHealth)
+
+	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Combat")
+	FGameplayAttributeData Shield;
+	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Shield)
+
+	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Combat")
+	FGameplayAttributeData Armor;
+	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Armor)
+
+	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Combat")
+	FGameplayAttributeData MovementSpeed;
+	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MovementSpeed)
+
+	UPROPERTY(BlueprintReadOnly, Category = "Combat")
+	FGameplayAttributeData Damage;
+	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Damage)
+
+private:
+	static constexpr float MinMovementSpeed = 100.0f;
+
+	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
+	void ClampCurrentAttributes();
+
+	UFUNCTION()
+	void OnRep_Health(const FGameplayAttributeData& OldHealth);
+
+	UFUNCTION()
+	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
+
+	UFUNCTION()
+	void OnRep_Shield(const FGameplayAttributeData& OldShield);
+
+	UFUNCTION()
+	void OnRep_Armor(const FGameplayAttributeData& OldArmor);
+
+	UFUNCTION()
+	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
+};
diff --git a/Source/GDS/GAS/GDSGameplayTags.cpp b/Source/GDS/GAS/GDSGameplayTags.cpp
new file mode 100644
index 0000000..41d9291
--- /dev/null
+++ b/Source/GDS/GAS/GDSGameplayTags.cpp
@@ -0,0 +1,5 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#include "GDS/GAS/GDSGameplayTags.h"
+
+UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Damage_Test, "Effect.Damage.Test");
diff --git a/Source/GDS/GAS/GDSGameplayTags.h b/Source/GDS/GAS/GDSGameplayTags.h
new file mode 100644
index 0000000..5b651c4
--- /dev/null
+++ b/Source/GDS/GAS/GDSGameplayTags.h
@@ -0,0 +1,7 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#pragma once
+
+#include "NativeGameplayTags.h"
+
+UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Test);
diff --git a/Source/GDS/GDS.Build.cs b/Source/GDS/GDS.Build.cs
index 70177d9..87a1e13 100644
--- a/Source/GDS/GDS.Build.cs
+++ b/Source/GDS/GDS.Build.cs
@@ -8,7 +8,7 @@ public class GDS : ModuleRules
 	{
 		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
 	
-		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
+		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
 
 		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
 		
diff --git a/Source/GDS/GameFramework/Character/GDSCharacter.cpp b/Source/GDS/GameFramework/Character/GDSCharacter.cpp
index f55667f..2129942 100644
--- a/Source/GDS/GameFramework/Character/GDSCharacter.cpp
+++ b/Source/GDS/GameFramework/Character/GDSCharacter.cpp
@@ -2,9 +2,12 @@
 
 #include "GDS/GameFramework/Character/GDSCharacter.h"
 
+#include "AbilitySystemComponent.h"
 #include "EnhancedInputComponent.h"
 #include "EnhancedInputSubsystems.h"
 #include "GDS/GDS.h"
+#include "GDS/GAS/Attributes/CombatAttributeSet.h"
+#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"
 #include "GameFramework/CharacterMovementComponent.h"
 #include "InputAction.h"
 #include "InputActionValue.h"
@@ -44,6 +47,12 @@ AGDSCharacter::AGDSCharacter()
 	Phase0InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
 }
 
+UAbilitySystemComponent* AGDSCharacter::GetAbilitySystemComponent() const
+{
+	const AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
+	return IsValid(GDSPlayerState) ? GDSPlayerState->GetAbilitySystemComponent() : nullptr;
+}
+
 void AGDSCharacter::BeginPlay()
 {
 	Super::BeginPlay();
@@ -58,6 +67,18 @@ void AGDSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
 	Super::EndPlay(EndPlayReason);
 }
 
+void AGDSCharacter::PossessedBy(AController* NewController)
+{
+	Super::PossessedBy(NewController);
+	InitializeAbilitySystem();
+}
+
+void AGDSCharacter::OnRep_PlayerState()
+{
+	Super::OnRep_PlayerState();
+	InitializeAbilitySystem();
+}
+
 void AGDSCharacter::PawnClientRestart()
 {
 	Super::PawnClientRestart();
@@ -167,3 +188,76 @@ FVector AGDSCharacter::GetPlanarRightDirection() const
 	const FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : GetActorRotation();
 	return FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
 }
+
+void AGDSCharacter::InitializeAbilitySystem()
+{
+	AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
+	if (!IsValid(GDSPlayerState))
+	{
+		return;
+	}
+
+	UAbilitySystemComponent* AbilitySystemComponent = GDSPlayerState->GetAbilitySystemComponent();
+	const UCombatAttributeSet* CombatAttributeSet = GDSPlayerState->GetCombatAttributeSet();
+	if (!IsValid(AbilitySystemComponent) || !IsValid(CombatAttributeSet))
+	{
+		return;
+	}
+
+	AbilitySystemComponent->InitAbilityActorInfo(GDSPlayerState, this);
+	BindAttributeChangeDelegates(AbilitySystemComponent);
+	ApplyMovementSpeedToCharacter(CombatAttributeSet->GetMovementSpeed());
+
+	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
+	UE_LOG(LogGDS, Log,
+		TEXT("[%s] ASC Init: Owner=%s, Avatar=%s, Health=%.1f, MaxHealth=%.1f, Shield=%.1f, Armor=%.1f, MovementSpeed=%.1f, Damage=%.1f"),
+		NetContext,
+		*GetNameSafe(GDSPlayerState),
+		*GetNameSafe(this),
+		CombatAttributeSet->GetHealth(),
+		CombatAttributeSet->GetMaxHealth(),
+		CombatAttributeSet->GetShield(),
+		CombatAttributeSet->GetArmor(),
+		CombatAttributeSet->GetMovementSpeed(),
+		CombatAttributeSet->GetDamage());
+}
+
+void AGDSCharacter::BindAttributeChangeDelegates(UAbilitySystemComponent* AbilitySystemComponent)
+{
+	if (!IsValid(AbilitySystemComponent))
+	{
+		return;
+	}
+
+	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetMovementSpeedAttribute()).RemoveAll(this);
+	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetMovementSpeedAttribute()).AddUObject(this, &ThisClass::HandleMovementSpeedChanged);
+}
+
+void AGDSCharacter::HandleMovementSpeedChanged(const FOnAttributeChangeData& ChangeData)
+{
+	ApplyMovementSpeedToCharacter(ChangeData.NewValue);
+
+	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
+	UE_LOG(LogGDS, Log, TEXT("[%s] MovementSpeed Delegate: %.1f -> %.1f"),
+		NetContext,
+		ChangeData.OldValue,
+		ChangeData.NewValue);
+}
+
+void AGDSCharacter::ApplyMovementSpeedToCharacter(float NewMovementSpeed)
+{
+	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
+	if (!IsValid(MovementComponent))
+	{
+		return;
+	}
+
+	const float OldMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
+	MovementComponent->MaxWalkSpeed = NewMovementSpeed;
+
+	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
+	UE_LOG(LogGDS, Log, TEXT("[%s] MaxWalkSpeed: %.1f -> %.1f"),
+		NetContext,
+		OldMaxWalkSpeed,
+		MovementComponent->MaxWalkSpeed);
+}
diff --git a/Source/GDS/GameFramework/Character/GDSCharacter.h b/Source/GDS/GameFramework/Character/GDSCharacter.h
index 04b9be2..a9307cf 100644
--- a/Source/GDS/GameFramework/Character/GDSCharacter.h
+++ b/Source/GDS/GameFramework/Character/GDSCharacter.h
@@ -2,25 +2,31 @@
 
 #pragma once
 
+#include "AbilitySystemInterface.h"
 #include "CoreMinimal.h"
 #include "GameFramework/Character.h"
 #include "GDSCharacter.generated.h"
 
+class UAbilitySystemComponent;
 class UInputAction;
 class UInputMappingContext;
 struct FInputActionValue;
+struct FOnAttributeChangeData;
 
 UCLASS()
-class GDS_API AGDSCharacter : public ACharacter
+class GDS_API AGDSCharacter : public ACharacter, public IAbilitySystemInterface
 {
 	GENERATED_BODY()
 
 public:
 	AGDSCharacter();
+	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
 
 protected:
 	virtual void BeginPlay() override;
 	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
+	virtual void PossessedBy(AController* NewController) override;
+	virtual void OnRep_PlayerState() override;
 	virtual void PawnClientRestart() override;
 	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
 	virtual void UnPossessed() override;
@@ -53,4 +59,8 @@ private:
 	void Look(const FInputActionValue& Value);
 	FVector GetPlanarForwardDirection() const;
 	FVector GetPlanarRightDirection() const;
+	void InitializeAbilitySystem();
+	void BindAttributeChangeDelegates(UAbilitySystemComponent* AbilitySystemComponent);
+	void HandleMovementSpeedChanged(const FOnAttributeChangeData& ChangeData);
+	void ApplyMovementSpeedToCharacter(float NewMovementSpeed);
 };
diff --git a/Source/GDS/GameFramework/PlayerState/GDSPlayerState.cpp b/Source/GDS/GameFramework/PlayerState/GDSPlayerState.cpp
index 395157d..d5fa1d2 100644
--- a/Source/GDS/GameFramework/PlayerState/GDSPlayerState.cpp
+++ b/Source/GDS/GameFramework/PlayerState/GDSPlayerState.cpp
@@ -2,9 +2,20 @@
 
 #include "GDS/GameFramework/PlayerState/GDSPlayerState.h"
 
+#include "AbilitySystemComponent.h"
 #include "GDS/GDS.h"
+#include "GDS/GAS/Attributes/CombatAttributeSet.h"
 #include "Net/UnrealNetwork.h"
 
+AGDSPlayerState::AGDSPlayerState()
+{
+	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
+	AbilitySystemComponent->SetIsReplicated(true);
+	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
+
+	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));
+}
+
 void AGDSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
 {
 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
@@ -19,6 +30,11 @@ void AGDSPlayerState::OnRep_PlayerName()
 	OnIdentityChanged.Broadcast();
 }
 
+UAbilitySystemComponent* AGDSPlayerState::GetAbilitySystemComponent() const
+{
+	return AbilitySystemComponent;
+}
+
 bool AGDSPlayerState::IsRoomOwner() const
 {
 	return bIsRoomOwner;
@@ -29,6 +45,11 @@ bool AGDSPlayerState::IsReady() const
 	return bIsReady;
 }
 
+UCombatAttributeSet* AGDSPlayerState::GetCombatAttributeSet() const
+{
+	return CombatAttributeSet;
+}
+
 void AGDSPlayerState::OnRep_IsRoomOwner()
 {
 	UE_LOG(LogGDS, Log, TEXT("[Client] 방장 상태 복제: Player=%s, IsRoomOwner=%s"),
diff --git a/Source/GDS/GameFramework/PlayerState/GDSPlayerState.h b/Source/GDS/GameFramework/PlayerState/GDSPlayerState.h
index 9931cab..aeab3ab 100644
--- a/Source/GDS/GameFramework/PlayerState/GDSPlayerState.h
+++ b/Source/GDS/GameFramework/PlayerState/GDSPlayerState.h
@@ -2,23 +2,31 @@
 
 #pragma once
 
+#include "AbilitySystemInterface.h"
 #include "CoreMinimal.h"
 #include "GameFramework/PlayerState.h"
 #include "GDSPlayerState.generated.h"
 
+class UAbilitySystemComponent;
+class UCombatAttributeSet;
+
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGDSPlayerStateChangedSignature);
 
 UCLASS()
-class GDS_API AGDSPlayerState : public APlayerState
+class GDS_API AGDSPlayerState : public APlayerState, public IAbilitySystemInterface
 {
 	GENERATED_BODY()
 
 public:
+	AGDSPlayerState();
+
 	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
 	virtual void OnRep_PlayerName() override;
+	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
 
 	bool IsRoomOwner() const;
 	bool IsReady() const;
+	UCombatAttributeSet* GetCombatAttributeSet() const;
 
 	UPROPERTY(BlueprintAssignable)
 	FGDSPlayerStateChangedSignature OnIdentityChanged;
@@ -39,6 +47,12 @@ private:
 	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
 	bool bIsReady = false;
 
+	UPROPERTY(VisibleAnywhere, Category = "GAS")
+	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
+
+	UPROPERTY(VisibleAnywhere, Category = "GAS")
+	TObjectPtr<UCombatAttributeSet> CombatAttributeSet;
+
 	UFUNCTION()
 	void OnRep_IsRoomOwner();
 

commit 9246a8c64a83b5f08ae436ae6b0930c23ecc5d8b
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Sat Jun 27 22:16:55 2026 +0900

    feat(gas): 테스트 GE 흐름 추가
    
    Phase 1 P1에서 임시 입력이 Server RPC를 거쳐 순수 C++ GameplayEffect를 적용하도록 연결했다.
    
    Damage는 SetByCaller 메타 Attribute로 받아 Shield-Armor-Health 캐스케이드를 검증하고, MovementSpeed는 Instant GE로 CharacterMovement 반영 경로를 확인한다.
---
 Config/DefaultGame.ini                             |  10 ++
 Source/GDS/GAS/Attributes/CombatAttributeSet.cpp   |  50 ++++++++
 Source/GDS/GAS/Attributes/CombatAttributeSet.h     |   9 +-
 Source/GDS/GAS/Effects/GE_TestDamage.cpp           |  42 +++++++
 Source/GDS/GAS/Effects/GE_TestDamage.h             |  25 ++++
 Source/GDS/GAS/Effects/GE_TestSpeedBuff.cpp        |  33 ++++++
 Source/GDS/GAS/Effects/GE_TestSpeedBuff.h          |  25 ++++
 Source/GDS/GAS/GDSGameplayTags.cpp                 |   1 +
 Source/GDS/GAS/GDSGameplayTags.h                   |   1 +
 .../GDS/GameFramework/Character/GDSCharacter.cpp   | 127 +++++++++++++++++++++
 Source/GDS/GameFramework/Character/GDSCharacter.h  |  14 +++
 11 files changed, 336 insertions(+), 1 deletion(-)

diff --git a/Config/DefaultGame.ini b/Config/DefaultGame.ini
index 6589e6e..730118a 100644
--- a/Config/DefaultGame.ini
+++ b/Config/DefaultGame.ini
@@ -4,3 +4,13 @@ ProjectID=7D1F0F5F43255AF026BDEA9E11BDF79C
 
 [/Script/GDS.GDSLobbyGameMode]
 MainMapPath=/Game/Levels/Main
+
+[/Script/GDS.CombatAttributeSet]
+ArmorFlatDamageReduction=7.0
+ArmorDamageReductionCapPercent=0.5
+
+[/Script/GDS.GE_TestDamage]
+TestDamage=40.0
+
+[/Script/GDS.GE_TestSpeedBuff]
+MovementSpeedDelta=-300.0
diff --git a/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp b/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
index d9332dc..ef70296 100644
--- a/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
+++ b/Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
@@ -36,6 +36,12 @@ void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute
 void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
 {
 	Super::PostGameplayEffectExecute(Data);
+
+	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
+	{
+		ExecuteDamageCascade(GetDamage());
+	}
+
 	ClampCurrentAttributes();
 }
 
@@ -68,6 +74,50 @@ void UCombatAttributeSet::ClampCurrentAttributes()
 	SetMovementSpeed(FMath::Max(MinMovementSpeed, GetMovementSpeed()));
 }
 
+void UCombatAttributeSet::ExecuteDamageCascade(float RawDamage)
+{
+	const float OldShield = GetShield();
+	const float OldArmor = GetArmor();
+	const float OldHealth = GetHealth();
+
+	float RemainingDamage = FMath::Max(0.0f, RawDamage);
+	if (RemainingDamage > 0.0f)
+	{
+		const float ShieldAbsorbed = FMath::Min(GetShield(), RemainingDamage);
+		SetShield(GetShield() - ShieldAbsorbed);
+		RemainingDamage -= ShieldAbsorbed;
+	}
+
+	float ArmorReduction = 0.0f;
+	if (RemainingDamage > 0.0f)
+	{
+		const float ClampedReductionCap = FMath::Clamp(ArmorDamageReductionCapPercent, 0.0f, 1.0f);
+		ArmorReduction = FMath::Min(ArmorFlatDamageReduction, RemainingDamage * ClampedReductionCap);
+		const float EffectiveDamage = RemainingDamage - ArmorReduction;
+		const float ArmorAbsorbed = FMath::Min(GetArmor(), EffectiveDamage);
+		SetArmor(GetArmor() - ArmorAbsorbed);
+		RemainingDamage = EffectiveDamage - ArmorAbsorbed;
+	}
+
+	if (RemainingDamage > 0.0f)
+	{
+		SetHealth(GetHealth() - RemainingDamage);
+	}
+
+	SetDamage(0.0f);
+
+	UE_LOG(LogGDS, Log,
+		TEXT("[Server] Damage Cascade: Raw=%.1f, ArmorReduction=%.1f, Shield %.1f -> %.1f, Armor %.1f -> %.1f, Health %.1f -> %.1f"),
+		RawDamage,
+		ArmorReduction,
+		OldShield,
+		GetShield(),
+		OldArmor,
+		GetArmor(),
+		OldHealth,
+		GetHealth());
+}
+
 void UCombatAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
 {
 	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Health, OldHealth);
diff --git a/Source/GDS/GAS/Attributes/CombatAttributeSet.h b/Source/GDS/GAS/Attributes/CombatAttributeSet.h
index 6d590bb..901feab 100644
--- a/Source/GDS/GAS/Attributes/CombatAttributeSet.h
+++ b/Source/GDS/GAS/Attributes/CombatAttributeSet.h
@@ -12,7 +12,7 @@
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 
-UCLASS()
+UCLASS(Config = Game)
 class GDS_API UCombatAttributeSet : public UAttributeSet
 {
 	GENERATED_BODY()
@@ -51,8 +51,15 @@ public:
 private:
 	static constexpr float MinMovementSpeed = 100.0f;
 
+	UPROPERTY(EditDefaultsOnly, Config, Category = "Combat|Armor")
+	float ArmorFlatDamageReduction = 7.0f;
+
+	UPROPERTY(EditDefaultsOnly, Config, Category = "Combat|Armor")
+	float ArmorDamageReductionCapPercent = 0.5f;
+
 	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
 	void ClampCurrentAttributes();
+	void ExecuteDamageCascade(float RawDamage);
 
 	UFUNCTION()
 	void OnRep_Health(const FGameplayAttributeData& OldHealth);
diff --git a/Source/GDS/GAS/Effects/GE_TestDamage.cpp b/Source/GDS/GAS/Effects/GE_TestDamage.cpp
new file mode 100644
index 0000000..ff2011d
--- /dev/null
+++ b/Source/GDS/GAS/Effects/GE_TestDamage.cpp
@@ -0,0 +1,42 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#include "GDS/GAS/Effects/GE_TestDamage.h"
+
+#include "GDS/GAS/Attributes/CombatAttributeSet.h"
+#include "GDS/GAS/GDSGameplayTags.h"
+#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
+
+UGE_TestDamage::UGE_TestDamage()
+{
+	RebuildModifiers();
+}
+
+void UGE_TestDamage::PostInitProperties()
+{
+	Super::PostInitProperties();
+	RebuildModifiers();
+}
+
+float UGE_TestDamage::GetTestDamage() const
+{
+	return TestDamage;
+}
+
+void UGE_TestDamage::RebuildModifiers()
+{
+	DurationPolicy = EGameplayEffectDurationType::Instant;
+
+	Modifiers.Reset();
+
+	FSetByCallerFloat SetByCallerMagnitude;
+	SetByCallerMagnitude.DataTag = TAG_Data_Damage_Test;
+
+	FGameplayModifierInfo& DamageModifier = Modifiers.AddDefaulted_GetRef();
+	DamageModifier.Attribute = UCombatAttributeSet::GetDamageAttribute();
+	DamageModifier.ModifierOp = EGameplayModOp::Additive;
+	DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerMagnitude);
+
+	FInheritedTagContainer AssetTags;
+	AssetTags.AddTag(TAG_Effect_Damage_Test);
+	FindOrAddComponent<UAssetTagsGameplayEffectComponent>().SetAndApplyAssetTagChanges(AssetTags);
+}
diff --git a/Source/GDS/GAS/Effects/GE_TestDamage.h b/Source/GDS/GAS/Effects/GE_TestDamage.h
new file mode 100644
index 0000000..36848fd
--- /dev/null
+++ b/Source/GDS/GAS/Effects/GE_TestDamage.h
@@ -0,0 +1,25 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#pragma once
+
+#include "GameplayEffect.h"
+#include "GE_TestDamage.generated.h"
+
+UCLASS(Config = Game)
+class GDS_API UGE_TestDamage : public UGameplayEffect
+{
+	GENERATED_BODY()
+
+public:
+	UGE_TestDamage();
+
+	virtual void PostInitProperties() override;
+
+	float GetTestDamage() const;
+
+private:
+	UPROPERTY(EditDefaultsOnly, Config, Category = "Test")
+	float TestDamage = 40.0f;
+
+	void RebuildModifiers();
+};
diff --git a/Source/GDS/GAS/Effects/GE_TestSpeedBuff.cpp b/Source/GDS/GAS/Effects/GE_TestSpeedBuff.cpp
new file mode 100644
index 0000000..81672fe
--- /dev/null
+++ b/Source/GDS/GAS/Effects/GE_TestSpeedBuff.cpp
@@ -0,0 +1,33 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#include "GDS/GAS/Effects/GE_TestSpeedBuff.h"
+
+#include "GDS/GAS/Attributes/CombatAttributeSet.h"
+
+UGE_TestSpeedBuff::UGE_TestSpeedBuff()
+{
+	RebuildModifiers();
+}
+
+void UGE_TestSpeedBuff::PostInitProperties()
+{
+	Super::PostInitProperties();
+	RebuildModifiers();
+}
+
+float UGE_TestSpeedBuff::GetMovementSpeedDelta() const
+{
+	return MovementSpeedDelta;
+}
+
+void UGE_TestSpeedBuff::RebuildModifiers()
+{
+	DurationPolicy = EGameplayEffectDurationType::Instant;
+
+	Modifiers.Reset();
+
+	FGameplayModifierInfo& SpeedModifier = Modifiers.AddDefaulted_GetRef();
+	SpeedModifier.Attribute = UCombatAttributeSet::GetMovementSpeedAttribute();
+	SpeedModifier.ModifierOp = EGameplayModOp::Additive;
+	SpeedModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(MovementSpeedDelta));
+}
diff --git a/Source/GDS/GAS/Effects/GE_TestSpeedBuff.h b/Source/GDS/GAS/Effects/GE_TestSpeedBuff.h
new file mode 100644
index 0000000..44fb325
--- /dev/null
+++ b/Source/GDS/GAS/Effects/GE_TestSpeedBuff.h
@@ -0,0 +1,25 @@
+// Copyright Epic Games, Inc. All Rights Reserved.
+
+#pragma once
+
+#include "GameplayEffect.h"
+#include "GE_TestSpeedBuff.generated.h"
+
+UCLASS(Config = Game)
+class GDS_API UGE_TestSpeedBuff : public UGameplayEffect
+{
+	GENERATED_BODY()
+
+public:
+	UGE_TestSpeedBuff();
+
+	virtual void PostInitProperties() override;
+
+	float GetMovementSpeedDelta() const;
+
+private:
+	UPROPERTY(EditDefaultsOnly, Config, Category = "Test")
+	float MovementSpeedDelta = -300.0f;
+
+	void RebuildModifiers();
+};
diff --git a/Source/GDS/GAS/GDSGameplayTags.cpp b/Source/GDS/GAS/GDSGameplayTags.cpp
index 41d9291..c4250ff 100644
--- a/Source/GDS/GAS/GDSGameplayTags.cpp
+++ b/Source/GDS/GAS/GDSGameplayTags.cpp
@@ -3,3 +3,4 @@
 #include "GDS/GAS/GDSGameplayTags.h"
 
 UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Damage_Test, "Effect.Damage.Test");
+UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Damage_Test, "Data.Damage.Test");
diff --git a/Source/GDS/GAS/GDSGameplayTags.h b/Source/GDS/GAS/GDSGameplayTags.h
index 5b651c4..6c91b3f 100644
--- a/Source/GDS/GAS/GDSGameplayTags.h
+++ b/Source/GDS/GAS/GDSGameplayTags.h
@@ -5,3 +5,4 @@
 #include "NativeGameplayTags.h"
 
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Test);
+UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Damage_Test);
diff --git a/Source/GDS/GameFramework/Character/GDSCharacter.cpp b/Source/GDS/GameFramework/Character/GDSCharacter.cpp
index 2129942..f58d2cb 100644
--- a/Source/GDS/GameFramework/Character/GDSCharacter.cpp
+++ b/Source/GDS/GameFramework/Character/GDSCharacter.cpp
@@ -7,6 +7,9 @@
 #include "EnhancedInputSubsystems.h"
 #include "GDS/GDS.h"
 #include "GDS/GAS/Attributes/CombatAttributeSet.h"
+#include "GDS/GAS/Effects/GE_TestDamage.h"
+#include "GDS/GAS/Effects/GE_TestSpeedBuff.h"
+#include "GDS/GAS/GDSGameplayTags.h"
 #include "GDS/GameFramework/PlayerState/GDSPlayerState.h"
 #include "GameFramework/CharacterMovementComponent.h"
 #include "InputAction.h"
@@ -45,6 +48,14 @@ AGDSCharacter::AGDSCharacter()
 	LookAction = CreateDefaultSubobject<UInputAction>(TEXT("LookAction"));
 	LookAction->ValueType = EInputActionValueType::Axis2D;
 	Phase0InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
+
+	TestDamageAction = CreateDefaultSubobject<UInputAction>(TEXT("TestDamageAction"));
+	TestDamageAction->ValueType = EInputActionValueType::Boolean;
+	Phase0InputMappingContext->MapKey(TestDamageAction, EKeys::One);
+
+	TestSpeedEffectAction = CreateDefaultSubobject<UInputAction>(TEXT("TestSpeedEffectAction"));
+	TestSpeedEffectAction->ValueType = EInputActionValueType::Boolean;
+	Phase0InputMappingContext->MapKey(TestSpeedEffectAction, EKeys::Two);
 }
 
 UAbilitySystemComponent* AGDSCharacter::GetAbilitySystemComponent() const
@@ -101,6 +112,8 @@ void AGDSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputCompon
 	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
 	EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ThisClass::MoveLeft);
 	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
+	EnhancedInputComponent->BindAction(TestDamageAction, ETriggerEvent::Started, this, &ThisClass::ApplyTestDamage);
+	EnhancedInputComponent->BindAction(TestSpeedEffectAction, ETriggerEvent::Started, this, &ThisClass::ApplyTestSpeedEffect);
 }
 
 void AGDSCharacter::UnPossessed()
@@ -177,6 +190,22 @@ void AGDSCharacter::Look(const FInputActionValue& Value)
 	AddControllerPitchInput(-LookAxis.Y);
 }
 
+void AGDSCharacter::ApplyTestDamage(const FInputActionValue& Value)
+{
+	if (Value.Get<bool>())
+	{
+		ServerApplyTestDamage();
+	}
+}
+
+void AGDSCharacter::ApplyTestSpeedEffect(const FInputActionValue& Value)
+{
+	if (Value.Get<bool>())
+	{
+		ServerApplyTestSpeedEffect();
+	}
+}
+
 FVector AGDSCharacter::GetPlanarForwardDirection() const
 {
 	const FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : GetActorRotation();
@@ -261,3 +290,101 @@ void AGDSCharacter::ApplyMovementSpeedToCharacter(float NewMovementSpeed)
 		OldMaxWalkSpeed,
 		MovementComponent->MaxWalkSpeed);
 }
+
+bool AGDSCharacter::ServerApplyTestDamage_Validate()
+{
+	const bool bValid = IsValid(GetAbilitySystemComponent());
+	UE_LOG(LogGDS, Log, TEXT("[Server] RPC Validate: ServerApplyTestDamage=%s"), bValid ? TEXT("pass") : TEXT("reject"));
+	return bValid;
+}
+
+void AGDSCharacter::ServerApplyTestDamage_Implementation()
+{
+	if (!HasAuthority())
+	{
+		return;
+	}
+
+	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
+	const AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
+	const UCombatAttributeSet* CombatAttributeSet = IsValid(GDSPlayerState) ? GDSPlayerState->GetCombatAttributeSet() : nullptr;
+	if (!IsValid(AbilitySystemComponent) || !IsValid(CombatAttributeSet))
+	{
+		return;
+	}
+
+	const float OldShield = CombatAttributeSet->GetShield();
+	const float OldArmor = CombatAttributeSet->GetArmor();
+	const float OldHealth = CombatAttributeSet->GetHealth();
+
+	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
+	EffectContext.AddSourceObject(this);
+
+	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UGE_TestDamage::StaticClass(), 1.0f, EffectContext);
+	if (!SpecHandle.IsValid())
+	{
+		UE_LOG(LogGDS, Warning, TEXT("[Server] UGE_TestDamage Spec creation failed"));
+		return;
+	}
+
+	const UGE_TestDamage* TestDamageEffect = GetDefault<UGE_TestDamage>();
+	const float TestDamage = IsValid(TestDamageEffect) ? TestDamageEffect->GetTestDamage() : 0.0f;
+	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage_Test, TestDamage);
+
+	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
+
+	UE_LOG(LogGDS, Log,
+		TEXT("[Server] UGE_TestDamage Applied: Damage=%.1f, Shield %.1f -> %.1f, Armor %.1f -> %.1f, Health %.1f -> %.1f"),
+		TestDamage,
+		OldShield,
+		CombatAttributeSet->GetShield(),
+		OldArmor,
+		CombatAttributeSet->GetArmor(),
+		OldHealth,
+		CombatAttributeSet->GetHealth());
+}
+
+bool AGDSCharacter::ServerApplyTestSpeedEffect_Validate()
+{
+	const bool bValid = IsValid(GetAbilitySystemComponent());
+	UE_LOG(LogGDS, Log, TEXT("[Server] RPC Validate: ServerApplyTestSpeedEffect=%s"), bValid ? TEXT("pass") : TEXT("reject"));
+	return bValid;
+}
+
+void AGDSCharacter::ServerApplyTestSpeedEffect_Implementation()
+{
+	if (!HasAuthority())
+	{
+		return;
+	}
+
+	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
+	const AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
+	const UCombatAttributeSet* CombatAttributeSet = IsValid(GDSPlayerState) ? GDSPlayerState->GetCombatAttributeSet() : nullptr;
+	if (!IsValid(AbilitySystemComponent) || !IsValid(CombatAttributeSet))
+	{
+		return;
+	}
+
+	const float OldMovementSpeed = CombatAttributeSet->GetMovementSpeed();
+
+	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
+	EffectContext.AddSourceObject(this);
+
+	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UGE_TestSpeedBuff::StaticClass(), 1.0f, EffectContext);
+	if (!SpecHandle.IsValid())
+	{
+		UE_LOG(LogGDS, Warning, TEXT("[Server] UGE_TestSpeedBuff Spec creation failed"));
+		return;
+	}
+
+	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
+
+	const UGE_TestSpeedBuff* TestSpeedEffect = GetDefault<UGE_TestSpeedBuff>();
+	const float MovementSpeedDelta = IsValid(TestSpeedEffect) ? TestSpeedEffect->GetMovementSpeedDelta() : 0.0f;
+
+	UE_LOG(LogGDS, Log, TEXT("[Server] UGE_TestSpeedBuff Applied: Delta=%.1f, MovementSpeed %.1f -> %.1f"),
+		MovementSpeedDelta,
+		OldMovementSpeed,
+		CombatAttributeSet->GetMovementSpeed());
+}
diff --git a/Source/GDS/GameFramework/Character/GDSCharacter.h b/Source/GDS/GameFramework/Character/GDSCharacter.h
index a9307cf..5f7c21c 100644
--- a/Source/GDS/GameFramework/Character/GDSCharacter.h
+++ b/Source/GDS/GameFramework/Character/GDSCharacter.h
@@ -50,6 +50,12 @@ private:
 	UPROPERTY(Transient)
 	TObjectPtr<UInputAction> LookAction;
 
+	UPROPERTY(Transient)
+	TObjectPtr<UInputAction> TestDamageAction;
+
+	UPROPERTY(Transient)
+	TObjectPtr<UInputAction> TestSpeedEffectAction;
+
 	void AddPhase0InputMapping();
 	void RemovePhase0InputMapping();
 	void MoveForward(const FInputActionValue& Value);
@@ -57,10 +63,18 @@ private:
 	void MoveRight(const FInputActionValue& Value);
 	void MoveLeft(const FInputActionValue& Value);
 	void Look(const FInputActionValue& Value);
+	void ApplyTestDamage(const FInputActionValue& Value);
+	void ApplyTestSpeedEffect(const FInputActionValue& Value);
 	FVector GetPlanarForwardDirection() const;
 	FVector GetPlanarRightDirection() const;
 	void InitializeAbilitySystem();
 	void BindAttributeChangeDelegates(UAbilitySystemComponent* AbilitySystemComponent);
 	void HandleMovementSpeedChanged(const FOnAttributeChangeData& ChangeData);
 	void ApplyMovementSpeedToCharacter(float NewMovementSpeed);
+
+	UFUNCTION(Server, Reliable, WithValidation)
+	void ServerApplyTestDamage();
+
+	UFUNCTION(Server, Reliable, WithValidation)
+	void ServerApplyTestSpeedEffect();
 };

commit 6d22dadfe34da7fff8f96c75fbbc47fed939ac14
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Sat Jun 27 22:27:50 2026 +0900

    fix(gas): GE asset tag component 생성 수정
    
    GameplayEffect 생성자 안에서 FindOrAddComponent가 NewObject(NAME_None)를 호출해 CDO 생성 중 크래시가 발생했다.
    
    태그 컴포넌트를 이름 있는 default subobject로 만들고 GEComponents에 등록해 UE 5.4 생성자 규칙을 지킨다.
---
 Source/GDS/GAS/Effects/GE_TestDamage.cpp | 5 ++++-
 Source/GDS/GAS/Effects/GE_TestDamage.h   | 5 +++++
 2 files changed, 9 insertions(+), 1 deletion(-)

diff --git a/Source/GDS/GAS/Effects/GE_TestDamage.cpp b/Source/GDS/GAS/Effects/GE_TestDamage.cpp
index ff2011d..8ea62d2 100644
--- a/Source/GDS/GAS/Effects/GE_TestDamage.cpp
+++ b/Source/GDS/GAS/Effects/GE_TestDamage.cpp
@@ -8,6 +8,9 @@
 
 UGE_TestDamage::UGE_TestDamage()
 {
+	AssetTagsComponent = CreateDefaultSubobject<UAssetTagsGameplayEffectComponent>(TEXT("AssetTagsComponent"));
+	GEComponents.Add(AssetTagsComponent);
+
 	RebuildModifiers();
 }
 
@@ -38,5 +41,5 @@ void UGE_TestDamage::RebuildModifiers()
 
 	FInheritedTagContainer AssetTags;
 	AssetTags.AddTag(TAG_Effect_Damage_Test);
-	FindOrAddComponent<UAssetTagsGameplayEffectComponent>().SetAndApplyAssetTagChanges(AssetTags);
+	AssetTagsComponent->SetAndApplyAssetTagChanges(AssetTags);
 }
diff --git a/Source/GDS/GAS/Effects/GE_TestDamage.h b/Source/GDS/GAS/Effects/GE_TestDamage.h
index 36848fd..df3f9a6 100644
--- a/Source/GDS/GAS/Effects/GE_TestDamage.h
+++ b/Source/GDS/GAS/Effects/GE_TestDamage.h
@@ -5,6 +5,8 @@
 #include "GameplayEffect.h"
 #include "GE_TestDamage.generated.h"
 
+class UAssetTagsGameplayEffectComponent;
+
 UCLASS(Config = Game)
 class GDS_API UGE_TestDamage : public UGameplayEffect
 {
@@ -18,6 +20,9 @@ public:
 	float GetTestDamage() const;
 
 private:
+	UPROPERTY()
+	TObjectPtr<UAssetTagsGameplayEffectComponent> AssetTagsComponent;
+
 	UPROPERTY(EditDefaultsOnly, Config, Category = "Test")
 	float TestDamage = 40.0f;
 

commit a32e193f2ca14058b21f19076688053d0bf9a1fd
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Sat Jun 27 22:36:09 2026 +0900

    fix(gas): 중복 MovementSpeed 로그 생략
    
    Instant GE 적용 뒤 클램프 경로에서 같은 MovementSpeed 값으로 delegate가 다시 호출되어 검증 로그가 중복됐다.
    
    값 변화가 없는 delegate 이벤트는 무시해 실제 이동속도 변경만 로그와 MaxWalkSpeed 갱신에 남긴다.
---
 Source/GDS/GameFramework/Character/GDSCharacter.cpp | 5 +++++
 1 file changed, 5 insertions(+)

diff --git a/Source/GDS/GameFramework/Character/GDSCharacter.cpp b/Source/GDS/GameFramework/Character/GDSCharacter.cpp
index f58d2cb..b3d4e03 100644
--- a/Source/GDS/GameFramework/Character/GDSCharacter.cpp
+++ b/Source/GDS/GameFramework/Character/GDSCharacter.cpp
@@ -264,6 +264,11 @@ void AGDSCharacter::BindAttributeChangeDelegates(UAbilitySystemComponent* Abilit
 
 void AGDSCharacter::HandleMovementSpeedChanged(const FOnAttributeChangeData& ChangeData)
 {
+	if (FMath::IsNearlyEqual(ChangeData.OldValue, ChangeData.NewValue))
+	{
+		return;
+	}
+
 	ApplyMovementSpeedToCharacter(ChangeData.NewValue);
 
 	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");

commit 58dd6dae99223d10c8782301d8c18ed13872f54f
Author: JiMoJjing <qqq5092@gmail.com>
Date:   Sat Jun 27 22:57:22 2026 +0900

    docs(gas): phase1 마찰 기록 갱신
    
    작업 전 설계 보강 항목을 비우고, P0/P1 구현과 검증 중 실제로 발생한 빌드/GE 컴포넌트/로그 관측 마찰만 남긴다.
---
 docs/friction-log.md | 16 +++++-----------
 1 file changed, 5 insertions(+), 11 deletions(-)

diff --git a/docs/friction-log.md b/docs/friction-log.md
index fbfdf49..5c1fddb 100644
--- a/docs/friction-log.md
+++ b/docs/friction-log.md
@@ -8,14 +8,8 @@
 
 ---
 
-- [2026-06-26] (phase1_gas) GAS Build.cs 모듈 의존성(GameplayAbilities/GameplayTags/GameplayTasks)이 문서에 빠져 구현 첫 단계가 모호함 — 설계 보강 필요로 기록
-- [2026-06-26] (phase1_gas) PlayerState ASC 소유만 있고 IAbilitySystemInterface 구현 여부가 명시되지 않아 이후 접근 계약이 흔들릴 수 있음 — 설계 보강 필요로 기록
-- [2026-06-26] (phase1_gas) Attribute 초기값과 테스트 GE 수치가 추후 튜닝으로만 남아 P0/P1 검증값을 정할 근거가 부족함 — 검증용 임시값 명시 필요로 기록
-- [2026-06-26] (phase1_gas) Phase 0 임시 입력은 Character에 있는데 문서는 PlayerController 입력 바인딩처럼 읽혀 구현 위치 해석이 갈림 — Character 입력 확장/RPC 송신 책임 분리 명시 필요로 기록
-- [2026-06-26] (phase1_gas) Armor 튜닝값 저장 방식, GAS Attribute 복제 매크로 패턴, 델리게이트 중복 구독 방지, 테스트 GE의 C++/에셋 형태가 덜 명확함 — 세부 구현 계약 보강 필요로 기록
-- [2026-06-26] (phase1_gas) 테스트 GE를 BP/DataAsset 인스턴스로 만들라고 했지만 UGameplayEffect 에셋 생성·참조 방식이 구현 관점에서 아직 모호함 — 설계 보강 필요로 기록
-- [2026-06-26] (phase1_gas) Armor 고정감소+cap의 정확한 산식과 Health/Shield/Armor/MovementSpeed 클램프 규칙이 빠져 Attribute 계산 결과가 갈릴 수 있음 — 계산 계약 보강 필요로 기록
-- [2026-06-26] (phase1_gas) ASC 복제 활성화(SetIsReplicated)와 GAS 모듈 의존성 Public/Private 위치가 명시되지 않아 Build/Replication 구현이 흔들릴 수 있음 — 세부 구현 계약 보강 필요로 기록
-- [2026-06-26] (phase1_gas) phase1_gas는 GE 순수 C++를 확정했지만 coding.md에는 GE_ 에셋 관례가 남아 문서 간 계약이 충돌함 — 사람 승인 후 coding.md 보정 필요로 기록
-- [2026-06-26] (phase1_gas) coding.md 보정 후에도 phase1_gas 9항에는 GE 에셋 관례 충돌이 미해결로 남아 문서 상태가 어긋남 — phase1_gas 후속 정리 필요로 기록
-- [2026-06-26] (phase1_gas) GE 수치를 UPROPERTY(Config)로 둔다고 했지만 UCLASS Config 지정과 .ini 섹션 위치가 명시되지 않아 구현 방식이 갈릴 수 있음 — Config 계약 보강 필요로 기록
+- [2026-06-27] (phase1_gas/P0) Build.cs에 GameplayAbilities 모듈을 추가한 뒤 UBT가 uproject 플러그인 의존성 누락 경고를 냄 — GDS.uproject에 GameplayAbilities 플러그인을 명시해 경고 없는 빌드로 정리
+- [2026-06-27] (phase1_gas/P1) UE 5.4의 GameplayEffect Asset Tag 설정이 deprecated 필드와 컴포넌트 방식 사이에서 헷갈리기 쉬웠음 — UAssetTagsGameplayEffectComponent 경로로 맞추고 deprecated 필드는 사용하지 않음
+- [2026-06-27] (phase1_gas/P1) UGameplayEffect 생성자 안에서 FindOrAddComponent가 NewObject(NAME_None)를 호출해 에디터 CDO 생성 시 fatal crash 발생 — 이름 있는 CreateDefaultSubobject로 컴포넌트를 만들고 GEComponents에 등록하도록 수정
+- [2026-06-27] (phase1_gas/P1) GDSEditor 빌드는 통과했지만 GE 컴포넌트 생성자 크래시는 에디터 로드 시점에만 드러남 — 이후 GE 컴포넌트 생성 변경은 빌드 외에 에디터 로드/PIE 확인까지 필요함으로 기록
+- [2026-06-27] (phase1_gas/P1) Instant MovementSpeed GE 적용 뒤 클램프/재평가 경로에서 같은 값(300→300) delegate 로그가 한 번 더 찍혀 검증 로그가 흐려짐 — OldValue와 NewValue가 같으면 MovementSpeed delegate 처리를 생략하도록 정리
```

## 2. friction-log.md 전체 내용

```markdown
# Friction Log

> 상시 기록용. 설계 빈 칸, 스펙 모호, 기존 관례와 충돌 등 거슬렸던 지점을 **한 줄씩** 적는다.
> diff성 증거(무엇을 고쳤는지)는 git이 이미 기록하므로 따로 안 적어도 된다 — 여기는 "왜 거슬렸는지"만.
> `retro.md` 회고 시점에 일괄 처리되고, 처리 후 비운다(durable 기록은 회고 결과로 남은 지침 diff와 커밋).

**형식:** `- [날짜] (scope) 한 줄 설명 — 그 자리에서 어떻게 처리했는지`

---

- [2026-06-27] (phase1_gas/P0) Build.cs에 GameplayAbilities 모듈을 추가한 뒤 UBT가 uproject 플러그인 의존성 누락 경고를 냄 — GDS.uproject에 GameplayAbilities 플러그인을 명시해 경고 없는 빌드로 정리
- [2026-06-27] (phase1_gas/P1) UE 5.4의 GameplayEffect Asset Tag 설정이 deprecated 필드와 컴포넌트 방식 사이에서 헷갈리기 쉬웠음 — UAssetTagsGameplayEffectComponent 경로로 맞추고 deprecated 필드는 사용하지 않음
- [2026-06-27] (phase1_gas/P1) UGameplayEffect 생성자 안에서 FindOrAddComponent가 NewObject(NAME_None)를 호출해 에디터 CDO 생성 시 fatal crash 발생 — 이름 있는 CreateDefaultSubobject로 컴포넌트를 만들고 GEComponents에 등록하도록 수정
- [2026-06-27] (phase1_gas/P1) GDSEditor 빌드는 통과했지만 GE 컴포넌트 생성자 크래시는 에디터 로드 시점에만 드러남 — 이후 GE 컴포넌트 생성 변경은 빌드 외에 에디터 로드/PIE 확인까지 필요함으로 기록
- [2026-06-27] (phase1_gas/P1) Instant MovementSpeed GE 적용 뒤 클램프/재평가 경로에서 같은 값(300→300) delegate 로그가 한 번 더 찍혀 검증 로그가 흐려짐 — OldValue와 NewValue가 같으면 MovementSpeed delegate 처리를 생략하도록 정리

```

## 3. 스펙-코드 불일치 보고

확인된 스펙-코드 불일치는 없다.

다만 구현 중 아래 지점은 스펙이 구체 API까지 지정하지 않은 부분을 UE 5.4 현재 방식으로 해석해 구현했다.

- Effect.Damage.Test GE 식별 태그는 UGE_TestDamage에 UAssetTagsGameplayEffectComponent를 붙여 Asset Tag로 설정했다. phase1_gas.md는 GE 식별용 태그 등록과 사용 의도만 지정하고, UE 5.4의 구체 컴포넌트 API까지는 지정하지 않았다.
- 테스트 입력 키는 문서가 구체 키를 지정하지 않아 1을 데미지 테스트, 2를 속도 테스트로 매핑했다.
- UAssetTagsGameplayEffectComponent는 처음에 FindOrAddComponent로 붙였다가 에디터 CDO 생성 시 크래시가 발생해, 이름 있는 CreateDefaultSubobject 후 GEComponents에 등록하는 방식으로 수정했다. 이는 스펙 변경이 아니라 UE 5.4 생성자 규칙에 맞춘 구현 보정이다.
- MovementSpeed delegate에서 같은 값(OldValue == NewValue)으로 재호출되는 로그는 검증 노이즈라 생략하도록 했다. Attribute 값이나 GE 흐름 자체는 스펙과 동일하다.

## 4. 변경/신규 파일 목록

- Config/DefaultGame.ini — Phase 1 검증용 GAS 설정값(ArmorFlatDamageReduction, ArmorDamageReductionCapPercent, TestDamage, MovementSpeedDelta) 저장.
- GDS.uproject — GameplayAbilities 플러그인 의존성 명시.
- Source/GDS/GDS.Build.cs — GameplayAbilities, GameplayTags, GameplayTasks를 public module dependency로 추가.
- Source/GDS/GAS/Attributes/CombatAttributeSet.h — Health/MaxHealth/Shield/Armor/MovementSpeed/Damage Attribute 선언, 복제 OnRep, Config armor 수치 선언.
- Source/GDS/GAS/Attributes/CombatAttributeSet.cpp — 초기 Attribute 값, 복제 등록, 클램프, Damage Meta 처리, Shield→Armor→Health 캐스케이드, Attribute Rep 로그 구현.
- Source/GDS/GAS/Effects/GE_TestDamage.h — 순수 C++ 테스트 데미지 GameplayEffect 선언, Config 기반 테스트 데미지 값과 Asset Tag 컴포넌트 보유.
- Source/GDS/GAS/Effects/GE_TestDamage.cpp — Instant Damage Meta Modifier, Data.Damage.Test SetByCaller, Effect.Damage.Test Asset Tag 설정 구현.
- Source/GDS/GAS/Effects/GE_TestSpeedBuff.h — 순수 C++ 테스트 속도 GameplayEffect 선언, Config 기반 MovementSpeed delta 보유.
- Source/GDS/GAS/Effects/GE_TestSpeedBuff.cpp — Instant MovementSpeed Additive Modifier(-300) 구현.
- Source/GDS/GAS/GDSGameplayTags.h — Phase 1 네이티브 GameplayTag extern 선언(Effect.Damage.Test, Data.Damage.Test).
- Source/GDS/GAS/GDSGameplayTags.cpp — Phase 1 네이티브 GameplayTag 정의.
- Source/GDS/GameFramework/Character/GDSCharacter.h — IAbilitySystemInterface 구현 선언, 테스트 입력 액션, MovementSpeed delegate, 테스트 Server RPC 선언.
- Source/GDS/GameFramework/Character/GDSCharacter.cpp — PlayerState ASC 위임, InitAbilityActorInfo, MovementSpeed→MaxWalkSpeed 반영, 1/2 테스트 입력, Server RPC validation, 테스트 GE 적용 로그 구현.
- Source/GDS/GameFramework/PlayerState/GDSPlayerState.h — IAbilitySystemInterface 구현 선언, ASC와 UCombatAttributeSet 소유 멤버 추가.
- Source/GDS/GameFramework/PlayerState/GDSPlayerState.cpp — ASC/AttributeSet default subobject 생성, ASC replication 활성화, Mixed replication mode 설정.
- docs/architecture/phase1_gas.md — Phase 1 GAS 척추 설계/구현 지시 문서.
- docs/conventions/coding.md — 이 프로젝트의 GE는 순수 C++ UGE_ 클래스로 구현한다는 관례 명시.
- docs/friction-log.md — Phase 1 P0/P1 구현 중 발생한 마찰 기록.

## 5. 통과 기준 로그 위치

### P0 ASC Init / 초기 Attribute

- 클래스/함수: AGDSCharacter::InitializeAbilitySystem
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[%s] ASC Init: Owner=%s, Avatar=%s, Health=%.1f, MaxHealth=%.1f, Shield=%.1f, Armor=%.1f, MovementSpeed=%.1f, Damage=%.1f
```

PIE에서 찾을 예:

```text
[Server] ASC Init: Owner=GDSPlayerState_0, Avatar=GDSCharacter_0, Health=100.0, MaxHealth=100.0, Shield=50.0, Armor=50.0, MovementSpeed=600.0, Damage=0.0
[Client] ASC Init: Owner=GDSPlayerState_0, Avatar=GDSCharacter_0, Health=100.0, MaxHealth=100.0, Shield=50.0, Armor=50.0, MovementSpeed=600.0, Damage=0.0
```

### RPC validation

- 클래스/함수: AGDSCharacter::ServerApplyTestDamage_Validate
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[Server] RPC Validate: ServerApplyTestDamage=%s
```

- 클래스/함수: AGDSCharacter::ServerApplyTestSpeedEffect_Validate
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[Server] RPC Validate: ServerApplyTestSpeedEffect=%s
```

### 서버 Damage 캐스케이드 before→after

- 클래스/함수: UCombatAttributeSet::ExecuteDamageCascade
- 파일: Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
- 포맷:

```text
[Server] Damage Cascade: Raw=%.1f, ArmorReduction=%.1f, Shield %.1f -> %.1f, Armor %.1f -> %.1f, Health %.1f -> %.1f
```

### 서버 TestDamage GE 적용 before→after

- 클래스/함수: AGDSCharacter::ServerApplyTestDamage_Implementation
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[Server] UGE_TestDamage Applied: Damage=%.1f, Shield %.1f -> %.1f, Armor %.1f -> %.1f, Health %.1f -> %.1f
```

### 서버 TestSpeed GE 적용 before→after

- 클래스/함수: AGDSCharacter::ServerApplyTestSpeedEffect_Implementation
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[Server] UGE_TestSpeedBuff Applied: Delta=%.1f, MovementSpeed %.1f -> %.1f
```

### MovementSpeed delegate / MaxWalkSpeed 반영

- 클래스/함수: AGDSCharacter::HandleMovementSpeedChanged
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[%s] MovementSpeed Delegate: %.1f -> %.1f
```

- 클래스/함수: AGDSCharacter::ApplyMovementSpeedToCharacter
- 파일: Source/GDS/GameFramework/Character/GDSCharacter.cpp
- 포맷:

```text
[%s] MaxWalkSpeed: %.1f -> %.1f
```

참고: OldValue와 NewValue가 같은 MovementSpeed delegate 이벤트는 로그 노이즈를 줄이기 위해 생략한다.

### 클라이언트 Attribute Rep before→after

- 클래스/함수: UCombatAttributeSet::OnRep_Health
- 파일: Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
- 포맷:

```text
[Client] Attribute Rep: Health %.1f -> %.1f
```

- 클래스/함수: UCombatAttributeSet::OnRep_MaxHealth
- 파일: Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
- 포맷:

```text
[Client] Attribute Rep: MaxHealth %.1f -> %.1f
```

- 클래스/함수: UCombatAttributeSet::OnRep_Shield
- 파일: Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
- 포맷:

```text
[Client] Attribute Rep: Shield %.1f -> %.1f
```

- 클래스/함수: UCombatAttributeSet::OnRep_Armor
- 파일: Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
- 포맷:

```text
[Client] Attribute Rep: Armor %.1f -> %.1f
```

- 클래스/함수: UCombatAttributeSet::OnRep_MovementSpeed
- 파일: Source/GDS/GAS/Attributes/CombatAttributeSet.cpp
- 포맷:

```text
[Client] Attribute Rep: MovementSpeed %.1f -> %.1f
```