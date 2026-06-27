# GAS 척추 (ASC / AttributeSet / 기본 GE 흐름)

## 1. 목적

- ASC 배치, Replication Mode, AttributeSet, 기본 GameplayEffect 흐름을 구축해 GAS의 골격을 세우는 Phase 1 인프라.
- `project.md`의 GAS 증명 목표(AttributeSet, GameplayEffect 흐름, ASC Replication Mode 선택과 그 근거 문서화) 전부를 담당. 네트워크 증명 목표 중 **GE 기반 데미지 흐름의 서버 권위·복제**도 같이 시연한다. GameplayAbility 자체(어빌리티 활성, Enhanced Input 바인딩)는 해당 없음(Phase 2).

## 2. 설계 의도 / 핵심 결정

- **ASC 위치는 PlayerState.** 이 프로젝트는 이미 Seamless Travel을 쓴다(`phase0_lobby.md`) — PlayerState는 레벨 전환을 넘어 보존되지만 Character는 전환마다 파괴·재생성된다. ASC를 Character에 두면 전환·리스폰(Phase 5 웨이브 사망 포함)마다 ASC를 재구성해야 하는데, 그걸 정당화할 근거가 없다.
  - 탈락 대안: Character 배치. → Pawn 생명주기에 ASC/Attribute 상태가 종속되어, 리스폰이 있는 구조(웨이브 PvE)에서 매번 재초기화 비용이 발생. 탈락.

- **`IAbilitySystemInterface`는 `AGDSPlayerState`와 `AGDSCharacter` 둘 다 구현한다.** `AGDSPlayerState::GetAbilitySystemComponent()`는 자신의 ASC를 직접 반환. `AGDSCharacter::GetAbilitySystemComponent()`는 자신의 PlayerState의 ASC로 **위임**(delegate)한다. GAS 표준 패턴(예: `UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)`, GameplayCue 라우팅 등)이 PlayerState와 Avatar 양쪽 어느 쪽으로 호출돼도 ASC를 찾을 수 있어야 하므로, ASC를 PlayerState에 두기로 한 결정(위 항목)에는 이 인터페이스 계약이 같이 따라온다 — 빠지면 "어느 쪽 액터로 물어보느냐"에 따라 ASC 조회가 null이 되는 경로가 생긴다.
  - 탈락 대안: PlayerState만 구현. → Character를 통해 ASC를 찾는 호출 경로(다수의 GAS 헬퍼 함수가 이렇게 함)가 전부 깨짐. 탈락.

- **ASC Replication Mode는 Mixed.** Co-op이라 동료의 Attribute 변화·GameplayCue는 모든 클라(시뮬레이션 프록시 포함)가 봐야 하는데, Minimal·Mixed 둘 다 이 요건은 만족한다. 갈리는 지점은 "소유 클라에게 GE 자체를 복제하는가" — Phase 2~3에서 어빌리티가 들어오면 클라이언트 예측(prediction) 경로가 필요해질 가능성이 높고, 예측은 소유 클라가 자신의 GE 적용 결과를 미리 알아야 가능하다. Minimal로 가면 그 경로가 막혀 나중에 Mode를 다시 바꿔야 한다.
  - 탈락 대안: Full. → 시뮬레이션 프록시에게 불필요한 GE 디테일까지 복제해 대역폭을 낭비. `project.md`의 네트워크 최적화 증명 목표와 정면 충돌. 탈락.
  - 탈락 대안: Minimal. → 소유 클라 GE 복제가 없어 향후 prediction 도입 시 Mode 재변경이 필요해짐(재작업). 탈락.

- **ASC 초기화(`InitAbilityActorInfo`) 트리거는 서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`.** Owner(PlayerState)는 Avatar가 누군지 알 길이 없고, Character(Avatar)는 Possess·복제 시점에 자기 PlayerState를 확실히 들고 있다. 그래서 "Avatar 교체를 감지하는 쪽"이 트리거를 쥔다.
  - 이 보장이 성립하려면 **ASC와 AttributeSet은 PlayerState 생성자에서 `CreateDefaultSubobject`로 생성**해야 한다(지연 생성 금지). 이 전제가 없으면 두 훅 시점에 ASC 존재가 보장되지 않는다.
  - Avatar 교체(웨이브 리스폰 등) · 늦은 접속 · 비소유 클라 포함 — 셋 다 이 두 훅(서버 Possess는 교체마다 재발화, 클라 OnRep은 그 클라에 relevant해지는 모든 시점에 재발화)으로 자연히 커버된다. 별도 메커니즘 불필요.
  - PlayerController는 이 계약 범위 밖이다. PC가 필요해지는 시점(어빌리티 입력 바인딩)은 Phase 2.
  - 탈락 대안: 서버가 초기화 완료를 별도 RPC로 클라에 통지. → 클라는 이미 `OnRep_PlayerState`로 자체 판단 가능하므로 RPC 왕복만 추가. 탈락.

- **ASC↔Character 연결은 "범용 Attribute 변경 델리게이트 구독"이라는 채널로 가고, 지금 실제로 그 채널에 거는 내용은 MovementSpeed → `CharacterMovementComponent::MaxWalkSpeed` 하나뿐이다.** 채널 자체를 MovementSpeed 전용으로 좁혀서 만들면, Phase 5에서 Health=0 반응이 필요해질 때 Codex가 또 다른 패턴(Tick 폴링, AttributeSet→Character 직접 호출 등)을 새로 발명할 위험이 있다. 채널을 범용으로 두면 Phase 5는 같은 채널에 구독만 추가하면 된다.
  - 구독 시점은 위 Init 훅과 동일한 자리(서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`).
  - **이 구독은 중복 방지가 보장돼야 한다.** `InitAbilityActorInfo` 자체는 같은 Avatar로 재호출돼도 ASC가 내부적으로 안전하게 처리하지만(앞서 결정한 대로 별도 가드 불필요), MovementSpeed 델리게이트 구독은 **우리가 직접 작성하는 코드**라 그 보장이 없다. `PossessedBy`/`OnRep_PlayerState`가 같은 Avatar에 대해 다시 불릴 가능성(재초기화, 재접속 등)에 대비해, 매번 무조건 새로 바인딩하지 말고 **재구독 전 기존 바인딩을 해제**(`RemoveAll(this)` 등)하거나 **멱등성 있는 구독 방식**(`AddUniqueDynamic` 등 중복 바인딩을 막는 API)을 쓴다. 그렇지 않으면 같은 콜백이 중복 바인딩돼 MovementSpeed 변경 시 `MaxWalkSpeed` 갱신이 여러 번 실행되는 등의 부작용이 생긴다.

- **AttributeSet은 단일 클래스(`UCombatAttributeSet`)로 간다.** 지금 규모(Health/MaxHealth/Shield/Armor/MovementSpeed/Damage)에서 여러 클래스로 분리할 근거가 없다. 커지면 그때 분리한다. (`coding.md` 2항의 단일 예시 `UCombatAttributeSet`과도 일치.)

- **Attribute 목록: Health, MaxHealth, Shield, Armor, MovementSpeed, Damage(Meta Attribute).**
  - Health/MaxHealth를 분리하는 이유: 둘이 같은 값이면 "최대치 자체를 바꾸는 버프" 같은 GE를 표현할 수 없다.
  - Shield와 Armor는 동의어가 아니라 서로 다른 메커니즘이다 — Shield는 자동 회복형 풀, Armor는 인스턴스당 데미지 자체를 줄여주는 풀. (오버워치 기준 확인: Shield는 무피격 시간 후 초당 회복, Armor는 회복 없이 맞을 때마다 고정량+%cap으로 데미지를 줄임.) 단순 % 감소 버프보다 훨씬 깊은 GAS 증명 소재라 채택.
  - Damage는 Meta Attribute — 저장 상태가 아니라 "들어온 데미지를 계산해 다른 Attribute에 반영"하는 계산 통로. `PostGameplayEffectExecute` 처리 후 반드시 0으로 리셋.
  - 보류했던 후보(Stamina, AttackPower/DefensePower, CooldownReduction 등)는 그 자원/배율 모델이 아직 결정되지 않은 채로 만들면 추측이 되므로 이 시스템에서 다루지 않는다(별도로 기록도 하지 않음 — 해당 시스템 설계 시 자연히 다뤄질 것).

- **검증용 초기값(Phase 1 한정 placeholder, 최종 밸런스 아님).** P0/P1 통과 기준을 사람이 로그로 판정하려면 "정상이면 어떤 값이 찍혀야 하는가"의 기준점이 있어야 한다. 그래서 최종 밸런스와 분리된 **검증용 임시값**을 지금 못 박는다(추후 밸런스 패스에서 자유롭게 교체):
  - 초기 Attribute 값: `MaxHealth = 100`, `Health = 100`, `Shield = 50`, `Armor = 50`, `MovementSpeed = 600`(UE 기본 TPS 보행속도와 동일선상)
  - `UGE_TestDamage` 1회 적용량: SetByCaller(`Data.Damage.Test`)로 Damage(Meta)에 `40` 주입
  - Armor 캐스케이드 임시 수치: 인스턴스당 고정감소 `7`, 최대 `50%` cap (오버워치 수치를 그대로 시작점으로 채용)
  - `UGE_TestSpeedBuff`: MovementSpeed에 `-300` Instant 적용(편도 1회성 감속 — 복귀 로직 없음, "변화가 보이는가"만 확인하면 되므로 토글/타이머 불필요)
  - 이 값들로는 데미지 테스트를 약 2~3회 누적 입력하면 Shield→Armor가 차례로 소진되고 Health까지 영향이 보일 정도의 규모다. 정확한 횟수는 Armor 처리 디테일에 따라 달라질 수 있으니 사람이 로그로 직접 확인한다.

- **Attribute 클램프 규칙.**
  - `Health`: `[0, MaxHealth]`로 클램프(`PreAttributeChange`/`PostGameplayEffectExecute`에서).
  - `Shield`, `Armor`: 하한 `0`만 클램프. 상한은 지금 별도 Max Attribute가 없어 의미가 없다(증가시키는 GE가 이 Phase에 없음) — 추후 MaxShield/MaxArmor가 필요해지면 그때 추가.
  - `MovementSpeed`: 하한 **`100`**으로 클램프(내가 정함). 0 또는 음수로 떨어지면 캐릭터가 완전히 멈추거나 Movement/애니메이션이 비정상 동작할 위험이 있어 안전망이 필요하다. `100`은 기본값 `600`의 1/6 수준 — 디버프가 "느려졌다"는 걸 충분히 체감하게 하면서도 완전 정지는 막는 임계값으로 잡았다. 상한은 이 Phase의 버프(`UGE_TestSpeedBuff`가 감속만 함)로는 도달할 일이 없어 지금 정하지 않는다.
- **ASC 복제 활성화.** `AGDSPlayerState`에서 ASC를 `CreateDefaultSubobject`한 직후 `SetIsReplicated(true)`를 명시적으로 호출한다. `UAbilitySystemComponent`는 컴포넌트 자체 복제 플래그를 자동으로 켜주지 않으므로, 빠뜨리면 Attribute 자체가 정의돼 있어도 복제가 안 되는 조용한 실패가 난다.
- **`Build.cs` 모듈 의존성 위치는 `PublicDependencyModuleNames`.** `AGDSPlayerState`/`AGDSCharacter` 헤더가 `UAbilitySystemComponent*`/`UCombatAttributeSet*` 타입을 멤버로 선언해 외부에 노출하므로, 이 타입을 정의하는 `GameplayAbilities`/`GameplayTags`/`GameplayTasks` 모듈은 `Private`이 아니라 `Public`으로 가야 그 헤더를 include하는 다른 모듈에서도 타입이 보인다.

- **Damage 캐스케이드 순서는 Shield → Armor → Health(소모), 회복은 역순.** 오버워치의 4단계 HP 모델 중 GDS에 필요한 3단을 그대로 채용.
  - **Armor의 흡수 방식은 인스턴스당 고정감소 + %cap, 오버플로는 다음 단(Health)으로 넘어간다.** 단순 "Armor 있으면 데미지 20% 감소" 같은 전역 % 버프보다, 현재 Armor 값을 읽고 감소를 계산하고 차감하고 오버플로를 넘기는 다단 Attribute 상호작용이 Execution 로직의 깊이를 보여준다. 구체 수치(고정감소량, %cap)는 밸런스 튜닝값이라 `UPROPERTY(Config)`로 분리해 추후 자유롭게 바꿀 수 있게 하되, **검증을 위한 시작값은 위 "검증용 초기값" 항목에서 확정**(고정감소 7, cap 50%) — 빈 채로 두면 P0/P1 통과 기준을 사람이 판정할 기준점이 없어진다.
  - **캐스케이드 산식을 하나로 고정한다(아래 순서를 그대로 구현, 다른 해석 금지):**
    1. `D` = 들어온 raw Damage(Meta Attribute 값)
    2. **Shield 단계:** `shieldAbsorbed = min(CurrentShield, D)`; `CurrentShield -= shieldAbsorbed`; `D -= shieldAbsorbed`. `D <= 0`이면 여기서 멈춤(Armor/Health 불변).
    3. **Armor 단계(`D > 0`일 때만):** `reduction = min(FlatReduction, D * CapPercent)`(검증값: `min(7, D * 0.5)`); `effectiveD = D - reduction`; `armorAbsorbed = min(CurrentArmor, effectiveD)`; `CurrentArmor -= armorAbsorbed`; `D = effectiveD - armorAbsorbed`. `D <= 0`이면 여기서 멈춤(Health 불변, `reduction`만큼은 그냥 사라진 데미지로 취급 — 다음 단으로 안 넘어감).
    4. **Health 단계(`D > 0`일 때만):** `CurrentHealth -= D` (클램프는 아래 클램프 규칙 적용).
  - **단순화 지점(의도적):** 오버워치 원본은 한 데미지 인스턴스가 Armor를 막 소진시키는 경계에서 reduction을 "비례 조정"하는 예외 규칙이 있는데, 그건 다수의 작은 인스턴스가 동시에 오가는 PvP 밸런스용 디테일이다. GDS는 Phase 1에서 그 정밀도가 증명 목표에 기여하지 않으므로(Execution 흐름의 깊이를 보여주는 게 목적, 정확한 OW 리메이크가 목적이 아님) 그 경계 보정은 빼고 위 4단계로 단순화했다.
  - 탈락 대안: 단순 % 감소형 Armor. → GE Modifier 하나로 끝나 Execution Calculation을 거의 안 써도 됨. 증명 깊이가 안 나와 탈락.

- **Shield의 자동 회복은 Phase 3로 이관. Phase 1의 Shield는 회복 없는 정적 풀.** 자동 회복은 Periodic GameplayEffect(지속시간 기반 반복 적용)가 필요한데, 이는 Phase 1이 검증하려는 "GE 1회 적용 → Attribute 변경" 흐름과는 다른 종류의 패턴이다. Phase 3(상태이상/버프 흐름)에서 본격적으로 다룰 때 같이 정한다.

- **Phase 1 검증용 GE 트리거는 임시 디버그 입력(Phase 0의 임시 Input Mapping Context 확장) → Server RPC → 서버가 Self에게 테스트 GE 적용.** GameplayAbility/Enhanced Input 본 바인딩이 아직 없는 상태에서 GE 흐름을 보여줘야 하므로, Phase 0이 입력 시스템 부재 문제를 풀었던 것과 같은 패턴(임시 입력, 이후 폐기)을 재사용한다. Self 대상으로 한정해 타겟팅(라인트레이스 등, Phase 2~3 영역)이 검증에 섞이지 않게 한다. Server RPC를 거치는 이유는 클라가 직접 Attribute/GE를 바꾸면 `constraints.md` 1항 위반이고, `project.md`의 RPC 구조 증명 기회도 날리기 때문.
  - **이 입력 바인딩과 Server RPC는 모두 `AGDSCharacter`가 가진다 — `AGDSPlayerController`가 아니다.** Phase 0에서 이미 WASD/Mouse2D 입력이 `AGDSCharacter`의 임시 Input Mapping Context에 바인딩되는 걸로 실제 구현됐다(`phase0_lobby.md` 6항). 이 Phase의 테스트 액션 2개는 그 **같은 Input Mapping Context를 확장**하는 것이고, 입력을 받는 쪽(Character)이 그대로 Server RPC도 쏘는 게 자연스럽다 — 입력은 Character가 받고 RPC는 PlayerController가 쏘는 식으로 액터를 갈라놓으면 불필요한 액터 간 홉이 하나 늘어난다. `AGDSCharacter`는 자신의 `GetPlayerState<AGDSPlayerState>()`를 통해 ASC를 얻어 GE를 적용한다.
  - `AGDSPlayerController`는 이 시스템에서 맡는 역할이 없다. PC의 역할은 Phase 2(어빌리티 입력 바인딩, GAS 표준 패턴상 PC를 거칠 수도 있음)에서 다시 검토한다.
  - 탈락 대안: 콘솔 명령으로 직접 GE 적용. → 서버 권위 경로(RPC)를 안 거쳐 RPC 구조 증명 기회를 날림. 탈락.
  - 이 임시 입력·테스트 GE 자산은 Phase 2에서 진짜 어빌리티 바인딩이 들어오면 폐기 대상이다(`phase0_lobby.md` 6항과 같은 원칙).

- **GameplayEffect는 순수 C++ 클래스로만 만든다. BP 에셋을 따로 만들지 않는다.** 일반적인 GAS 권장 방식은 맞다 — C++ 베이스 클래스 + BP 자식 에셋으로 디자이너가 수치를 에디터에서 튜닝하는 게 보통의 정석. **하지만 그 권장은 "에디터를 여는 사람이 있다"는 전제가 깔려 있고, 이 프로젝트의 실제 파이프라인은 그 전제가 깨져 있다.** Codex는 텍스트/코드만 다루는 에이전트라 `.uasset`(BP 자식 클래스)을 만들거나 그 안의 Details 패널 값을 세팅하는 에디터 작업을 할 수 없다 — 그건 사람(YH)이 직접 에디터를 열어야 하는 수작업이고, 그 순간 "구현은 전부 Codex" 파이프라인이 깨진다. 그래서 일반 권장과 다르게, **이 프로젝트의 모든 GameplayEffect는 코드만으로 완결**되는 형태로 간다.
  - **`UGE_TestDamage`** — Damage(Meta) Modifier의 Magnitude를 **SetByCaller**(태그 `Data.Damage.Test`)로 선언. 실제 수치(`40`)는 GE 자체가 아니라, GE를 적용하는 쪽(`AGDSCharacter`의 RPC 핸들러)이 `FGameplayEffectSpecHandle`을 만들 때 코드로 주입한다. 이 방식이 자연스러운 이유: Damage는 본질적으로 "매번 다른 값이 들어오는" Attribute라(나중에 무기마다 데미지가 다름, Phase 3), SetByCaller가 GAS에서 이런 경우의 표준 패턴이다.
  - **`UGE_TestSpeedBuff`** — MovementSpeed Modifier의 Magnitude는 GE 클래스 자신의 `UPROPERTY(EditDefaultsOnly, Config)` float(`-300`)을 생성자에서 `FScalableFloat`로 사용. 고정값 테스트라 SetByCaller까지는 불필요.
  - 두 경우 모두 매직 넘버는 `UPROPERTY(Config)`로 빼서 `.ini`에 둔다(`constraints.md` 6항) — 이건 텍스트 파일이라 Codex가 직접 쓸 수 있고, 에디터가 필요 없다. `UGE_TestDamage`/`UGE_TestSpeedBuff`는 `UCLASS(Config=Game)`로 선언하고, 검증용 값은 `Config/DefaultGame.ini`의 각 클래스 섹션에 둔다.
  - 탈락 대안: C++ 클래스 + BP 자식 에셋(`GE_TestDamage` 등). → Codex가 만들 수 없는 산출물이 생겨 파이프라인이 끊김. 탈락.
  - **참고:** `coding.md` 2항이 이 결정과 일치하도록 갱신됨 — "GE는 전부 순수 C++ `UGE_`, BP/DataAsset `GE_` 에셋은 워크플로 자체를 바꾸는 시점에만 도입"으로 명시. 더 이상 충돌 없음.

- **GameplayTag는 Phase 1에서 GE 식별용으로만 쓴다(`Effect.Damage.Test`).** 상태 판정·상호작용을 위한 태그 시스템(Death, 상태이상 등)은 Phase 3 영역이라, 지금 만들면 그 시스템의 축소판을 미리 만들게 되어 나중에 구조가 부딪힌다.
  - **태그 등록 방식은 네이티브 C++ 매크로(`UE_DEFINE_GAMEPLAY_TAG` 계열).** `constraints.md` 6항의 "C++ 우선" 원칙과 일치.
  - 탈락 대안: DataTable 기반 태그 등록. → Blueprint 친화적이지만 C++ 우선 원칙과 안 맞음. 탈락.

## 3. 구조

- **`AGDSPlayerState`** — ASC(`UAbilitySystemComponent`, 생성자에서 `CreateDefaultSubobject`) + `UCombatAttributeSet` 소유. Replication Mode = Mixed로 설정. `IAbilitySystemInterface` 구현(자신의 ASC를 직접 반환).
- **`UCombatAttributeSet`** — Health/MaxHealth/Shield/Armor/MovementSpeed/Damage(Meta) 선언, 복제 등록(Damage 제외), Damage 캐스케이드 로직(Shield→Armor→Health) 담당.
- **`AGDSCharacter`** — `IAbilitySystemInterface` 구현(자신의 PlayerState의 ASC로 위임). 서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`에서 ASC `InitAbilityActorInfo` 호출 + MovementSpeed 변경 델리게이트 구독·`CharacterMovementComponent::MaxWalkSpeed` 갱신 책임. Phase 0의 임시 Input Mapping Context(WASD/Mouse2D)를 그대로 소유하며, 이 Phase의 테스트 액션 2개와 그에 대응하는 Server RPC(`ServerApplyTestDamage`/`ServerApplyTestSpeedEffect`)도 이 클래스가 가진다.
- **`UGE_TestDamage` / `UGE_TestSpeedBuff`** — Phase 1 검증용 임시 GameplayEffect, 순수 C++ 클래스(BP 자식 에셋 없음). `UGE_TestDamage`는 Damage(Meta) Modifier를 SetByCaller(`Data.Damage.Test`)로 선언하고 실제 수치는 적용 시점에 `AGDSCharacter`가 주입한다. `UGE_TestSpeedBuff`는 MovementSpeed Modifier 수치를 `UPROPERTY(Config)`로 자체 보유한다. Phase 2에서 진짜 어빌리티 GE로 대체되며 이 둘은 폐기.
- **`AGDSPlayerController`** — 이 시스템에서 맡는 역할 없음. 입력·RPC는 모두 `AGDSCharacter`가 가진다(위 참조). PC의 역할은 Phase 2에서 재검토.

**관계:** PlayerState가 ASC/AttributeSet을 소유한다. Character는 자신의 PlayerState를 통해 ASC에 접근해 Init을 트리거하고, MovementSpeed 변경을 구독해 자신의 Movement 컴포넌트에 반영한다. Character는 동시에 Phase 0부터 이어진 입력 소유자이기도 해서, 이 Phase의 테스트 입력을 직접 받아 Server RPC로 서버에 GE 적용을 요청한다(GE 판정 자체에는 개입하지 않음 — 판정은 항상 서버의 AttributeSet/GE 쪽에서 일어남). PlayerController는 이 시스템에서 어느 쪽에도 끼지 않는다.

## 4. 데이터 / 실행 흐름

- **Possess/초기화:** 서버 `PossessedBy` → ASC `InitAbilityActorInfo`(서버) → PlayerState 복제 → 클라 `OnRep_PlayerState` → 각 클라 ASC `InitAbilityActorInfo`(클라)
- **MovementSpeed 변경 전파:** 어떤 GE가 MovementSpeed Attribute를 바꾸면 → Attribute 변경 델리게이트 발화 → Character가 `MaxWalkSpeed` 갱신 (서버·클라 각자, 자신의 Init 시점에 구독한 콜백으로)
- **데미지 테스트:** 클라 입력 → Character의 `ServerApplyTestDamage` RPC → 서버가 `UGE_TestDamage` Spec 생성 + SetByCaller(`Data.Damage.Test`)로 `40` 주입 → ASC에 적용 → Damage(Meta)에 값 반영 → `PostGameplayEffectExecute`에서 Shield 흡수 → 남으면 Armor(고정감소+cap) 흡수 → 남으면 Health 차감 → Damage Meta 0 리셋 → Shield/Armor/Health 복제 → 각 클라 로그 반영
- **속도 테스트:** 클라 입력 → Character의 Server RPC → `UGE_TestSpeedBuff` 적용 → MovementSpeed Instant 변경(클램프 적용) → 복제 → MovementSpeed 델리게이트 발화 → 각 클라(소유자 포함) Character가 `MaxWalkSpeed` 갱신

## 5. 네트워크 측면

- **서버 권위:** GE 적용 전체, AttributeSet 캐스케이드 계산(Shield→Armor→Health), ASC Init 트리거(서버 측 `PossessedBy`), RPC validation.
- **복제:** Health/MaxHealth/Shield/Armor/MovementSpeed 모두 `COND_None`으로 전체 클라이언트에 복제 — Co-op이라 동료 상태를 전원이 봐야 하므로 숨길 이유가 없다. Damage는 Meta Attribute라 복제하지 않는다(연산 통로일 뿐 저장 상태가 아님).
- **복제 조건(`ReplicationCondition`):** 이 시스템에서는 쓰지 않는다. 조건부 선택적 복제가 의미를 갖는 시점은 Phase 4(스캐너/마킹)이며, 지금은 전원 공개 정보를 전제로 한다.
- **RPC:** `ServerApplyTestDamage` / `ServerApplyTestSpeedEffect`(또는 종류 구분 파라미터를 포함한 단일 RPC — 정확한 분리 방식은 Codex 판단) — **`AGDSCharacter`가 소유하는** Server RPC, Self 대상만이라 별도 대상 검증은 불필요하지만 `WithValidation` 형식은 갖춘다.
- **클라이언트 예측:** 없음. Mixed Mode를 택한 이유 중 하나가 "향후 prediction 경로를 막지 않기 위함"이지만, Phase 1 자체에서 prediction을 구현하지는 않는다.
- **최적화:** 해당 없음(Phase 6 대상). Relevancy/Dormancy/Lag Compensation은 이 시스템 스코프 밖.
- **로깅(검증 근거):** `coding.md` 5항 원칙을 이 시스템에 적용한 구체 목록 —
  - 서버: GE 적용 시점에 영향받는 Shield/Armor/Health **세 Attribute 각각의 before→after** (캐스케이드 중간 단계를 안 찍으면 오버플로가 맞게 일어났는지 검증 불가)
  - 클라(전원): Attribute 변경 델리게이트/`OnRep_` 수신 시점에 동일한 세 Attribute 값
  - RPC validation 통과/거부 결과
  - 모두 핫패스가 아니므로 기본 `Log` verbosity로 충분.

## 6. 시스템 고유 제약

- ASC와 `UCombatAttributeSet`은 `AGDSPlayerState` 생성자에서 `CreateDefaultSubobject`로 생성한다. `BeginPlay` 등 지연 생성 금지 — Init 동기화 안전성 전제가 이 생성 시점에 의존한다.
- ASC 생성 직후 `SetIsReplicated(true)`를 명시적으로 호출한다 — `UAbilitySystemComponent`는 자동으로 복제되지 않는다.
- **이 프로젝트 모듈의 `Build.cs`에 `GameplayAbilities`/`GameplayTags`/`GameplayTasks` 모듈 의존성을 `PublicDependencyModuleNames`로 추가해야 한다.** Phase 0은 GAS를 안 써서 필요 없었지만, 이 시스템이 GAS를 처음 끌어오는 지점이라 지금 추가가 필요하다(P0의 첫 단계로 들어감). `Private`이 아니라 `Public`인 이유는 `AGDSPlayerState`/`AGDSCharacter` 헤더가 ASC/AttributeSet 타입을 멤버로 노출하기 때문.
- **GameplayEffect는 항상 순수 C++ 클래스로 만든다. BP 자식 에셋을 만들지 않는다.** Codex가 에디터 GUI 작업을 할 수 없다는 이 프로젝트의 운영 제약 때문(2항 참조). Damage처럼 매번 값이 달라지는 Modifier는 SetByCaller로, 고정값 Modifier는 `UPROPERTY(Config)`로 받는다. Config를 쓰는 GameplayEffect 클래스는 `UCLASS(Config=Game)`로 선언하고, 값은 `Config/DefaultGame.ini`의 해당 클래스 섹션에 둔다.
- `InitAbilityActorInfo` 호출은 서버 `PossessedBy` / 클라 전원 `OnRep_PlayerState`로 한정한다. `PlayerController`에서 호출하지 않는다(Phase 2 책임과 혼동 방지).
- Attribute 값은 항상 GameplayEffect를 통해서만 바뀐다(`constraints.md` 2항). Phase 1의 테스트 트리거도 GE를 통해 적용하며, `PlayerController`/`Character`에서 Attribute를 직접 setter로 바꾸지 않는다.
- **Attribute 클램프:** `Health`는 `[0, MaxHealth]`, `Shield`/`Armor`는 하한 `0`만, `MovementSpeed`는 하한 `100`. 전부 `PreAttributeChange`/`PostGameplayEffectExecute`에서 처리(상세 근거는 2항).
- Damage(Meta) Attribute는 저장 상태가 아니다 — `PostGameplayEffectExecute` 처리 후 반드시 0으로 리셋한다. 리셋을 빠뜨리면 다음 GE 적용 시 값이 누적 오염된다.
- Armor 캐스케이드의 고정감소량·%cap은 매직 넘버로 하드코딩하지 않고 `UPROPERTY(Config)`로 분리한다(`constraints.md` 6항). 검증용 시작값은 2항에 명시(고정감소 7, cap 50%).
- **Attribute 선언은 GAS 표준 매크로 패턴을 그대로 따른다.** 헤더에서 `ATTRIBUTE_ACCESSORS` 매크로로 각 Attribute의 접근자(Getter/Setter/AttributeStatic)를 생성하고, `GetLifetimeReplicatedProps`에서 `DOREPLIFETIME_CONDITION_NOTIFY`로 복제 등록(`COND_None`, `REPNOTIFY_Always`)하고, 각 `OnRep_` 함수 안에서 `GAMEPLAYATTRIBUTE_REPNOTIFY` 매크로를 호출한다. 이 프로젝트 최초의 AttributeSet이라 정해진 관례가 없던 지점이었고, 여기서 정한 패턴이 이후 모든 AttributeSet(Phase 3 확장, Phase 5 AI 등)의 기준이 된다(`AGENTS.md` 1항).
- 이 Phase의 임시 디버그 입력과 `UGE_TestDamage`/`UGE_TestSpeedBuff`는 Phase 2에서 진짜 어빌리티가 들어오면 폐기 대상이다. 이 임시 구조를 확장하지 않는다(`phase0_lobby.md` 6항과 같은 원칙).
- AttributeSet의 Attribute 변경 델리게이트 같은 비동기/지연 콜백에 람다를 바인딩할 때는 `constraints.md` 5항대로 `TWeakObjectPtr` 캡처 + 유효성 검증을 따른다. MovementSpeed 델리게이트 구독은 추가로 2항에 명시한 중복 구독 방지 규칙을 따른다.

## 7. 구현 지시 (Codex용)

### P0 — ASC/AttributeSet 골격 + Init 동기화 (값 변화 없이 존재·복제·초기화 확인)
- **만들 것 (순서):**
  1) 프로젝트 모듈 `Build.cs`의 `PublicDependencyModuleNames`에 `GameplayAbilities`/`GameplayTags`/`GameplayTasks` 추가
  2) `UCombatAttributeSet` 정의 — Health/MaxHealth/Shield/Armor/MovementSpeed/Damage(Meta) 선언(표준 `ATTRIBUTE_ACCESSORS` 매크로로 접근자 생성), `GetLifetimeReplicatedProps`에서 `DOREPLIFETIME_CONDITION_NOTIFY`로 등록(Damage 제외, `COND_None`/`REPNOTIFY_Always`), 각 `OnRep_`에서 `GAMEPLAYATTRIBUTE_REPNOTIFY` 호출, 초기값 설정(`MaxHealth=100, Health=100, Shield=50, Armor=50, MovementSpeed=600`), 클램프 구현(`Health∈[0,MaxHealth]`, `Shield/Armor≥0`, `MovementSpeed≥100`)
  3) `AGDSPlayerState` 생성자에 ASC(`CreateDefaultSubobject` + `SetIsReplicated(true)`) + `UCombatAttributeSet` 추가, Replication Mode = Mixed 설정, `IAbilitySystemInterface` 구현(자신의 ASC 반환)
  4) `AGDSCharacter`에 `IAbilitySystemInterface` 구현(자신의 PlayerState ASC로 위임) + `PossessedBy`(서버) / `OnRep_PlayerState`(클라 전원)에서 `InitAbilityActorInfo(PlayerState, this)` 호출
  5) 같은 자리에서 MovementSpeed Attribute 변경 델리게이트 구독 → `CharacterMovementComponent::MaxWalkSpeed` 갱신 콜백 등록(지금은 트리거할 게 없어 연결만 존재) — 재호출 시 중복 바인딩되지 않도록 재구독 전 해제 또는 멱등 구독 방식 적용
  6) `Effect.Damage.Test` GameplayTag 네이티브 등록
  7) 로그: Init 호출 시점 Owner/Avatar 확인 + 초기 Attribute 6종 값 (서버/각 클라)
- **멈춤 지점:** 여기까지 하고 멈춰 보고한다.
- **통과 기준 (사람 확인):** 컴파일 통과 + DS 1 + 클라 2 PIE 접속 → 서버 로그와 각 클라 로그에 `InitAbilityActorInfo` 호출(Owner=PlayerState, Avatar=Character) 확인 + Health/MaxHealth/Shield/Armor 초기값이 서버·양쪽 클라 로그에서 위에서 정한 검증값(`100/100/50/50`)과 일치하는지 확인(초기 복제 검증)

### P1 — 테스트 GE 트리거 → 전체 파이프라인 + 복제 확인 (Phase 1 본 게이트)
- **만들 것 (순서):**
  1) `AGDSCharacter`가 가진 기존 임시 Input Mapping Context에 테스트 액션 2개 추가(데미지 테스트 / 속도 테스트)
  2) `AGDSCharacter`에 테스트용 Server RPC 추가(`ServerApplyTestDamage`/`ServerApplyTestSpeedEffect` 또는 종류 구분 파라미터 포함 1개 — 형식은 Codex 판단) — Self 대상만이라 별도 대상 검증은 불필요하지만 `WithValidation` 형식은 갖춤. `AGDSPlayerController`에는 추가하지 않는다.
  3) `Data.Damage.Test` GameplayTag 네이티브 등록(SetByCaller 키)
  4) `UGE_TestDamage` C++ 클래스 작성(BP 에셋 없음, `UCLASS(Config=Game)`) — Damage(Meta) Modifier를 `Data.Damage.Test` SetByCaller로 선언. 테스트 데미지 값 `40`은 `UPROPERTY(Config)`로 보유하고 `Config/DefaultGame.ini`의 `UGE_TestDamage` 클래스 섹션에 둔다. `ServerApplyTestDamage` 핸들러에서 Spec 생성 시 해당 Config 값을 태그로 주입 후 적용
  5) `UCombatAttributeSet`에 캐스케이드 로직 구현 — 2항에 고정한 산식 그대로(Shield 흡수 → Armor 고정감소 `7`/cap `50%` 흡수 → 오버플로 Health → Damage Meta 0 리셋)
  6) `UGE_TestSpeedBuff` C++ 클래스 작성(BP 에셋 없음, `UCLASS(Config=Game)`) — MovementSpeed Instant Modifier `-300`을 `UPROPERTY(Config)`로 보유하고 `Config/DefaultGame.ini`의 `UGE_TestSpeedBuff` 클래스 섹션에 둔다(Duration형 버프는 Phase 3 영역, 지금은 연결 확인만)
  7) 로그: GE 적용 시점 Shield/Armor/Health before→after, MovementSpeed before→after, RPC validation 결과 (서버+클라)
- **멈춤 지점:** 여기까지 하고 멈춰 보고한다.
- **통과 기준 (사람 확인):**
  (a) 데미지 테스트 키 입력 → 서버 로그에 Shield→Armor→Health 캐스케이드 값 확인 + 본인/타 클라 로그에 동일 최종값 복제 확인
  (b) 데미지를 누적 입력해(검증용 수치 기준 약 2~3회) Shield/Armor를 소진시켜 Health까지 깎이는 오버플로 동작 확인
  (c) 속도 테스트 키 입력 → PIE 화면에서 실제 이동 속도 변화(600→300)가 보이고, 그 값이 양쪽 클라 로그에서 일치 확인

## 8. 시스템 인수 검증 (전체)

- **결과:** 2026-06-27 통과. DS 1 + 클라 2 PIE에서 ASC Init/초기 Attribute 복제(P0), Damage 캐스케이드(Shield→Armor→Health) 서버-클라 일치, MovementSpeed 변경 및 MaxWalkSpeed 반영, RPC validation을 확인함(P1).
- **시나리오:** DS 1 + 클라 2 PIE → 두 클라 접속 → (P0) 서버/양쪽 클라 로그로 ASC Init 호출과 초기 Attribute 복제 일치 확인 → (P1) 한 클라에서 데미지 테스트 키 입력(단발) → Shield→Armor→Health 캐스케이드가 서버/양쪽 클라 로그에서 일치 → 같은 클라에서 데미지 테스트를 누적 입력해 Shield/Armor를 소진시켜 Health까지 깎이는 오버플로 확인 → 속도 테스트 키 입력 → 실제 이동 속도 변화가 PIE 화면에서 보이고 로그 일치 확인.
- **서버-클라 검증:** 로그에 `[Server]`/`[Client]` 태그로 Init 호출, GE 적용, Attribute before/after, RPC validation 시점을 모두 남겨 PIE 멀티 인스턴스에서 교차 확인한다.
- **증명 목표 시연 여부:** AttributeSet 설계, GameplayEffect 적용 흐름, ASC Replication Mode(Mixed)와 그 근거, 서버 권위적 GE 적용, RPC 구조(서버 검증)를 시연한다. GameplayTag는 식별용으로만 등장(상태 판정 시스템은 Phase 3). 클라이언트 예측·Relevancy·Dormancy·Lag Compensation·`ReplicationCondition`은 이 시스템 스코프가 아니다(각각 이후 Phase).

## 9. 미해결 / 추후

- Shield 자동 회복(Periodic GameplayEffect) — Phase 3
- Armor 고정감소량·%cap(현재 검증용 `7`/`50%`), Shield/Armor/Health 기본값(현재 검증용 `50/50/100`) 등 — 이 값들은 P0/P1 검증을 위한 시작값으로 확정했을 뿐 최종 밸런스가 아니다. 실제 밸런스 수치는 추후 튜닝.
- Health=0 도달 시 사망 처리(Character 반응 콘텐츠) — Phase 5. 단 연결 채널(Attribute 변경 델리게이트 구독)은 Phase 1에서 이미 마련한 것을 그대로 재사용 — 새 패턴 발명 불필요
- MovementSpeed Duration형 버프/디버프(상태이상으로서의 속도 변화) — Phase 3
- GameplayAbility 활성화·Enhanced Input 본 바인딩 — Phase 2. 그 시점에 이 Phase의 임시 디버그 입력·`UGE_TestDamage`/`UGE_TestSpeedBuff` 폐기
- AI(Phase 5)용 ASC Replication Mode(Minimal일 가능성이 높음, owning client가 없는 구조라서) — Phase 5에서 별도 결정
- 팀/분대 단위로 Attribute 가시성이 갈리는 경우 — 이 시스템은 전원 공개 복제를 전제로 하며, Phase 4(스캐너/마킹)에서 재검토
