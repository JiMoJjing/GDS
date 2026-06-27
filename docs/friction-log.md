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
