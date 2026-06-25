# GDS 설계 로드맵

> 시스템 설계 순서와 우선순위. 설계는 시스템(Phase)마다 새 채팅에서 진행한다.
> 순서 원칙: "증명하고 싶은 것" 순서가 아니라 **의존성 순서**다.
> 최적화가 뒤에 있는 이유 — PvE 부하(Phase 5)가 있어야 최적화 성과(Phase 6)를 측정·증명할 수 있기 때문이다.

---

## Phase 0 — 인프라 / 뼈대
- Dedicated Server 빌드 타깃 + 클라 접속 흐름 (이게 돌아야 이후 네트워크 검증이 가능)
- 코어 프레임워크: GameMode / GameState / PlayerState / PlayerController / Character (서버 권위 전제)

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
- 팀/분대 식별 도입 — 스캐너·분대 단위로 복제 대상을 가를 때 필요한 만큼만 (PlayerState squad id 등). Phase 0에서 미리 만들지 않은 이유: Co-op PvE라 PvP식 팀 개념이 성립하지 않고, 필요한 단위(분대 등)가 이 Phase의 설계에서 결정되므로 그때 같이 잡는다.
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
   - 세부 설계는 Phase 4에서. (팀/분대 식별 단위도 이때 같이 정한다 — 위 Phase 4 항목 참조.)

3. **Pre-Design Gate(주 경로·기존 관례 전수 확인) 도입 시점 — Phase 3~4에서 재평가**
   - 출처: 친구의 OVDR-11673 회고. 기존 코드를 *확장*하다 "주 경로 오인 / 기존 관례 무시하고 새 메커니즘 발명"으로 구조를 3번 재작업한 사고 → 설계 전에 코드 근거로 주 경로·모듈 경계·기존 관례를 전수 확인하는 체크표.
   - **지금(Phase 0~2)은 불필요:** greenfield라 "확인할 기존 코드/관례"가 없다. 내가 그 클래스를 만드는 중이니까. 게이트가 no-op이다.
   - **Phase 3부터 의미가 생김:** 데미지/공격/AI가 GAS·복제 등 *이미 깔린 토대 위에* 붙기 시작한다. 이때부터 "이 프로젝트는 같은 일(태그 등록, 복제 등록, attribute 정의)을 이미 어떻게 하나"를 Codex가 따라야 하고, 평행 패턴 발명 위험이 실재한다.
   - **트리거 조건:** `retro.md` 원칙대로, "Codex가 기존 관례를 무시하고 새 구조를 발명함" 마찰이 **실제로 한 번 나오면** 그때 해당 `architecture/[system].md`에 "주 경로/기존 관례 전수 확인" 절을 넣는다. 선제 도입하지 않는다 (과한 절차 경계).
   - Codex-시점 가드(확장 시 기존 관례 우선)는 이미 `AGENTS.md` 1항에 명시. 여기 결정 포인트는 그 **설계-시점 검증 절차**를 언제 무겁게 켤지에 대한 것.

---

## 진행 상태

- [x] Phase 0
- [ ] Phase 1
- [ ] Phase 2
- [ ] Phase 3
- [ ] Phase 4
- [ ] Phase 5
- [ ] Phase 6

> 한 Phase 설계가 끝나 `architecture/[시스템].md`로 정리되면 체크.
