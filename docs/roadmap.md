# GDS 설계 로드맵

> 시스템 설계 순서와 우선순위. 설계는 시스템(Phase)마다 새 채팅에서 진행한다.
> 순서 원칙: "증명하고 싶은 것" 순서가 아니라 **의존성 순서**다.
> 최적화가 뒤에 있는 이유 — PvE 부하(Phase 5)가 있어야 최적화 성과(Phase 6)를 측정·증명할 수 있기 때문이다.

---

## Phase 0 — 인프라 / 뼈대
- Dedicated Server 빌드 타깃 + 클라 접속 흐름 (이게 돌아야 이후 네트워크 검증이 가능)
- 코어 프레임워크: GameMode / GameState / PlayerState / PlayerController / Character (서버 권위 전제)
- 팀 식별 스켈레톤 (PlayerState team id 정도, 가볍게)

## Phase 1 — GAS 척추
- ASC 위치 (Character vs PlayerState) + Replication Mode 선택 + **근거 문서화**
- AttributeSet 설계 (Health/Shield 등)
- GameplayEffect + GameplayTag 기본 흐름 (effect 적용 → attribute 변경)

> Phase 0과 1은 밀접하다. 첫 설계 세션에서 "프레임워크 + ASC 위치"를 같이 다루게 될 수 있다.

## Phase 2 — 입력 + 검증용 첫 어빌리티
- Enhanced Input → 어빌리티 활성 바인딩
- 단순 어빌리티 1개를 end-to-end로 구현해 GAS 파이프라인이 서버 너머로 도는지 검증

## Phase 3 — 전투 콘텐츠
- 데미지 / 상태이상 시스템 (GE + GameplayTag, 데미지 타입, status effect)
- 공격 타입 다양화: hitscan / projectile / 스킬 몇 개

## Phase 4 — 전투 네트워크 정합성
- Lag Compensation (hitscan 전제)
- ReplicationCondition 팀 정보 은닉 — **방향 A 확정** (아래 결정 포인트 참조)

## Phase 5 — PvE 콘텐츠 (부하 생성원)
- 단순 AI 적 (GAS 기반 체력/피격, 의도적으로 단순하게 유지)
- 웨이브 스폰 시스템

## Phase 6 — 최적화 패스 (헤드라인 증명)
- 프로젝타일 Object Pooling
- Network Relevancy
- Dormancy

---

## 결정 포인트

1. **Pooling 시점** — Phase 3에서 프로젝타일을 풀 전제로 만들지, Phase 6에서 "도입 전후 GC 스파이크 비교" 스토리로 보여줄지.
   - 잠정 방향: 구조는 처음부터 풀 친화적으로 짜되, on/off 토글로 성능 델타를 측정. (확정 아님 — Phase 3에서 재검토)

2. **팀 정보 은닉 (Phase 4) = 방향 A 확정**
   - Co-op PvE라 PvP식 "적 팀에게서 숨기기"가 성립하지 않음. 그래서 증명 대상을 "조건부 선택적 복제" 역량으로 재정의.
   - 방향 A: **스캐너/마킹 기반 선택적 복제.** 특정 플레이어가 적을 스캔/마킹하면 그 적의 상세 정보(체력바, 약점, 위치 마커)가 스캔한 플레이어(또는 그 분대)에게만 복제된다.
   - `ReplicationCondition`으로 "이 정보를 누구에게 보낼지" 제어하는 것을 시연. 동시에 대역폭 최적화 효과(모두에게 안 뿌림)도 가져 최적화 논지와 시너지.
   - 세부 설계는 Phase 4에서.

---

## 진행 상태

- [ ] Phase 0
- [ ] Phase 1
- [ ] Phase 2
- [ ] Phase 3
- [ ] Phase 4
- [ ] Phase 5
- [ ] Phase 6

> 한 Phase 설계가 끝나 `architecture/[시스템].md`로 정리되면 체크.
