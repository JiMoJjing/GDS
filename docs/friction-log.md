# Friction Log

> 상시 기록용. 설계 빈 칸, 스펙 모호, 기존 관례와 충돌 등 거슬렸던 지점을 **한 줄씩** 적는다.
> diff성 증거(무엇을 고쳤는지)는 git이 이미 기록하므로 따로 안 적어도 된다 — 여기는 "왜 거슬렸는지"만.
> `retro.md` 회고 시점에 일괄 처리되고, 처리 후 비운다(durable 기록은 회고 결과로 남은 지침 diff와 커밋).

**형식:** `- [날짜] (scope) 한 줄 설명 — 그 자리에서 어떻게 처리했는지`

---

- [2026-06-26] (phase1_gas) GAS Build.cs 모듈 의존성(GameplayAbilities/GameplayTags/GameplayTasks)이 문서에 빠져 구현 첫 단계가 모호함 — 설계 보강 필요로 기록
- [2026-06-26] (phase1_gas) PlayerState ASC 소유만 있고 IAbilitySystemInterface 구현 여부가 명시되지 않아 이후 접근 계약이 흔들릴 수 있음 — 설계 보강 필요로 기록
- [2026-06-26] (phase1_gas) Attribute 초기값과 테스트 GE 수치가 추후 튜닝으로만 남아 P0/P1 검증값을 정할 근거가 부족함 — 검증용 임시값 명시 필요로 기록
- [2026-06-26] (phase1_gas) Phase 0 임시 입력은 Character에 있는데 문서는 PlayerController 입력 바인딩처럼 읽혀 구현 위치 해석이 갈림 — Character 입력 확장/RPC 송신 책임 분리 명시 필요로 기록
- [2026-06-26] (phase1_gas) Armor 튜닝값 저장 방식, GAS Attribute 복제 매크로 패턴, 델리게이트 중복 구독 방지, 테스트 GE의 C++/에셋 형태가 덜 명확함 — 세부 구현 계약 보강 필요로 기록
- [2026-06-26] (phase1_gas) 테스트 GE를 BP/DataAsset 인스턴스로 만들라고 했지만 UGameplayEffect 에셋 생성·참조 방식이 구현 관점에서 아직 모호함 — 설계 보강 필요로 기록
- [2026-06-26] (phase1_gas) Armor 고정감소+cap의 정확한 산식과 Health/Shield/Armor/MovementSpeed 클램프 규칙이 빠져 Attribute 계산 결과가 갈릴 수 있음 — 계산 계약 보강 필요로 기록
- [2026-06-26] (phase1_gas) ASC 복제 활성화(SetIsReplicated)와 GAS 모듈 의존성 Public/Private 위치가 명시되지 않아 Build/Replication 구현이 흔들릴 수 있음 — 세부 구현 계약 보강 필요로 기록
- [2026-06-26] (phase1_gas) phase1_gas는 GE 순수 C++를 확정했지만 coding.md에는 GE_ 에셋 관례가 남아 문서 간 계약이 충돌함 — 사람 승인 후 coding.md 보정 필요로 기록
