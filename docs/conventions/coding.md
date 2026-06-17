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

## 5. 로깅

- 프로젝트 전용 로그 카테고리를 정의해 사용한다. (`UE_LOG`에 엔진 기본 카테고리를 남발하지 않는다.)
- 로그 카테고리명: `LogGDS`
- 임시 디버그 로그는 커밋 전에 정리하거나, 의도된 것이면 verbosity를 낮춘다.

## 6. 주석

- "무엇을"이 아니라 "왜"를 남긴다. 코드가 설명하는 건 다시 쓰지 않는다.
- 설계 의도가 담긴 결정에는 짧은 근거 주석을 단다. (나중에 포트폴리오 재료가 된다.)
