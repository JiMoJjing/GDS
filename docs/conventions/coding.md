# 코딩 컨벤션

언리얼 표준을 기본으로 하되, 이 프로젝트에서 명시적으로 고정하는 규칙을 정리한다. 표준만으로 애매한 부분을 못 박는 것이 목적이다.

**대상 언리얼 버전: UE 5.4**

---

## 1. 네이밍 (언리얼 표준)

- `U` : UObject 파생 (예: `UAbilitySystemComponent`)
- `A` : AActor 파생 (예: `APlayerCharacter`)
- `F` : 일반 struct (예: `FDamageContext`)
- `E` : enum (예: `EDamageType`)
- `I` : interface
- `T` : 템플릿
- `b` : bool 변수 접두사 (예: `bIsDead`)

## 2. GAS 네이밍 (프로젝트 규칙)

| 종류 | 접두사 | 예시 |
|---|---|---|
| GameplayAbility 클래스 | `UGA_` 또는 클래스명에 `GameplayAbility` | `UGA_Fire`, `UGameplayAbility_Dash` |
| GameplayEffect 클래스 | `UGE_` | `UGE_Damage`, `UGE_Burning` |
| AttributeSet 클래스 | `U...AttributeSet` | `UCombatAttributeSet` |
| GameplayEffect 에셋(BP/DataAsset) | `GE_` | `GE_FireDamage` |
| GameplayAbility 에셋 | `GA_` | `GA_PrimaryFire` |
| GameplayTag | 계층 점 표기 | `Ability.Attack.Primary`, `State.Debuff.Burning` |

> 위 표는 초안이다. 어색하면 검토 시 수정. 핵심은 "한번 정하면 Codex가 일관되게 따른다"는 것.

> 이 프로젝트의 GameplayEffect는 전부 순수 C++ `UGE_` 클래스로 작성한다. 이는 "아직 초기 단계라서"가 아니라, 구현을 전담하는 Codex가 BP 자식 에셋(`.uasset`)을 만들거나 에디터에서 값을 세팅할 수 없기 때문이다. 이 제약은 프로젝트가 진행돼도 자동으로 풀리지 않는다. BP/DataAsset 기반 `GE_` 에셋은 사람이 직접 에디터에서 콘텐츠/밸런스를 튜닝하기로 워크플로 자체를 바꾸는 시점에만 도입한다. 그 전까지 Codex 구현 지시에서 별도 언급이 없으면 GE 에셋 생성을 전제로 하지 않는다.

## 3. 파일/구조

- 클래스 하나당 `.h` / `.cpp` 한 쌍을 기본으로 한다.
- 폴더는 도메인 단위로 나눈다. (예: `Abilities/`, `Attributes/`, `Network/`, `Weapons/`, `AI/`)
- include는 필요한 것만. 헤더에서는 가능한 한 **전방 선언(forward declaration)** 을 쓰고, 실제 사용은 `.cpp`에서 include 한다.

## 4. C++ 작성 규칙

- 중괄호는 **Allman 스타일**을 쓴다. 여는 중괄호를 같은 줄에 붙이지 않는다.

```cpp
// Good
if (bIsDead)
{
    HandleDeath();
}

// Bad
if (bIsDead) {
    HandleDeath();
}
```

- `UPROPERTY` / `UFUNCTION` 매크로의 지정자(specifier)는 의도에 맞게 명시한다. (복제 대상은 `Replicated` / `ReplicatedUsing`, 에디터 노출은 적절한 카테고리 등)
- 복제 프로퍼티는 `GetLifetimeReplicatedProps`에 반드시 등록한다. 조건부 복제는 여기서 `COND_` 또는 `ReplicationCondition`으로 지정한다.
- const 정확성을 지킨다. 변경하지 않는 참조/메서드는 `const`.
- 포인터는 사용 전 유효성 검사. (`IsValid()` 등)
- 비동기/지연 콜백(AbilityTask 델리게이트, 타이머, latent, async load, RPC 응답)에 넘기는 람다는 UObject를 `TWeakObjectPtr`로 캡처하고 진입 시 유효성을 검증한다. raw `[this]`/`[=]` 캡처 금지 — 근거와 바인딩 규칙은 `constraints.md` 5항.

## 5. 로깅

> 이 프로젝트는 Dedicated Server + 서버 권위 구조라, 중요한 사건(GE 적용, attribute 복제, OnRep, RPC validation, ReplicationCondition 결과)이 **화면에 안 보이고 두 프로세스에 걸쳐** 일어난다. 그래서 로그는 디버그 보조가 아니라 **1차 관측·검증면**이다. 검증은 사람이 하되, 로그가 그 판정의 증거를 댄다.

- **전용 카테고리 `LogGDS`를 쓴다.** 엔진 기본 카테고리(`LogTemp` 등)를 남발하지 않는다. 하위 카테고리 분리(`LogGDSNet` 등)는 한 카테고리로 읽기 버거워질 때 가서 나눈다 — 미리 쪼개지 않는다.

- **사실을 남긴다, 결론이 아니라.** 값·net role·순서를 적는다. 코드의 추측("정상 적용됨")은 검증 근거가 못 된다.

```cpp
// Good — 값과 권위를 사실로 남김 (검증 가능)
UE_LOG(LogGDS, Log, TEXT("[Server] GE_Damage 적용: Health %.1f -> %.1f"), Old, New);

// Bad — 추측을 적음 (검증 불가)
UE_LOG(LogGDS, Log, TEXT("데미지 정상 처리됨"));
```

- **net role를 로그에 박는다.** DS + 클라 다수의 트레이스를 같이 읽어야 하므로, 누가 찍은 로그인지(`[Server]`/`[Client]`)가 줄 자체로 구분돼야 한다. (PIE에서는 인스턴스가 클라를 더 구분해 준다.)

- **verbosity를 perf에 맞춰 게이팅한다.** 검증에 필요한 핵심 사실은 `Log`/`Display`(기본 노출). 핫패스(매 틱·발사체마다·AI마다 등 고빈도)는 **반드시 `Verbose`+**로 내린다. Phase 6 최적화 측정(Unreal Insights)은 기본 verbosity로 도는데, 핫패스 로그가 `Log`에 있으면 트레이스를 오염시켜 측정이 망가진다.

- **로깅의 양은 게이트가 정한다.** "가능한 한 세세하게"가 아니라, 해당 페이즈의 통과 기준(architecture 7·8항)이 **로그만 읽어도 판정 가능한 만큼**이 적정량이다.

- 임시 디버그 로그는 커밋 전에 정리하거나, 남길 가치가 있으면 위 기준(사실·적정 verbosity)에 맞춰 정식 로그로 승격한다.

## 6. 주석

- "무엇을"이 아니라 "왜"를 남긴다. 코드가 설명하는 건 다시 쓰지 않는다.
- 설계 의도가 담긴 결정에는 짧은 근거 주석을 단다. (나중에 포트폴리오 재료가 된다.)
