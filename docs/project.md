# 프로젝트 개요

## 1. 무엇을 만드는가

**Dedicated Server 기반 Co-op PvE TPS.**

- 플레이어들이 협동하여 웨이브(wave) 단위로 등장하는 AI 적을 처치하는 3인칭 슈터.
- 전용 서버(Dedicated Server) 위에서 동작하며, 모든 게임플레이 판정은 **서버 권위적(server-authoritative)** 으로 처리한다.
- AI는 **의도적으로 단순하게** 유지한다. AI 자체가 이 프로젝트의 자랑거리가 아니라, GAS와 네트워크 작업이 주인공이기 때문이다.

## 2. 이 구조를 택한 이유

- 웨이브 PvE는 **서버 부하를 통제 가능한 형태로** 만들어내서, 실제 최적화 성과를 측정·시연하기 좋다.
- 네트워크 동기화와 팀 구조 요구사항을 자연스럽게 포함한다.
- PvP의 경쟁 시스템 복잡도 없이 기술 초점을 또렷하게 유지한다.

## 3. 이 프로젝트가 증명하려는 기술

이 목록이 프로젝트의 존재 이유다. 모든 구현은 이 중 하나 이상을 증명해야 한다.

### GAS (Gameplay Ability System)
- AttributeSet, GameplayEffect 흐름, 태그 기반 GameplayAbility 상호작용
- ASC Replication Mode 선택과 **그 근거의 문서화**
- GE + GameplayTag 기반 데미지 타입 및 상태이상(status effect) 시스템

### 네트워크
- Dedicated Server에서의 서버 권위적 판정
- `ReplicationCondition`을 활용한 **조건부 선택적 복제** (스캔/마킹한 적의 상세 정보를 스캐너·분대에게만 복제 — Co-op PvE라 PvP식 팀 은닉 대신 이 방향. 상세는 `roadmap.md` 결정 포인트 2)
- 네트워크 최적화: Network Relevancy, Dormancy
- Hitscan 무기를 위한 Lag Compensation

### 게임플레이
- 여러 공격 타입에 걸친 다양한 공격/스킬 구현
- 프로젝타일 Object Pooling
- Enhanced Input System 연동

## 4. 산출물

- **코드** (이 저장소)
- **설계 문서** (`architecture/`) — 이게 곧 포트폴리오 초안이 된다.
- **Velog 글** — 기술적 결정/시스템 구현이 끝나 "쓸 말이 생겼을 때" 작성. 정해진 주기 없음.

## 5. 보조 포트폴리오

별도 프로젝트 "OW"(단일 플레이어 C++ Overwatch 팬 프로젝트, 커스텀 어빌리티 시스템)가 이력서에 함께 올라간다. 이 프로젝트는 그 보완재이며, OW로는 보여주지 못하는 **GAS·네트워크 깊이**를 담당한다.
