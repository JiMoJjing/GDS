# 로비 시스템 (Lobby)

## 1. 목적

- 플레이어 접속 → 레디 토글 → 방장이 시작 → 메인 레벨로 전환하는 Phase 0 인프라.
- `project.md`의 네트워크 증명 목표 중 **서버 권위적 판정**, **RPC 구조(Server RPC + validation)**, **복제(OnRep 기반 동기화)**를 담당. GAS는 해당 없음(Phase 1).

## 2. 설계 의도 / 핵심 결정

- **방장 = 첫 번째로 접속한 플레이어.** `PostLogin`에서 서버가 결정 (서버 권위).
- **"전원 레디" 판단의 이중 구조:** 방장 클라이언트가 `OnRep_bIsReady` 수신 시점마다 로컬에서 먼저 판단해 시작 버튼을 활성화하고(반응성 — RPC 왕복을 기다리지 않음), 실제 시작 트리거는 서버가 `ServerRequestStart`에서 **재검증**해야만 발생한다. Prediction은 아니지만 "낙관적 UI + 서버 최종 확정"이라는 같은 정신이다.
  - 택하지 않은 대안: 서버가 "전원 레디" 상태를 별도 알림으로 통지. → 알림 RPC 왕복이 추가될 뿐 서버는 어차피 시작 시점에 재검증해야 하므로 이득이 없어 탈락.
- **방장 본인의 `bIsReady`도 "전원 레디" 판정에 포함.** 제외하면 방장이 레디 안 해도 시작 가능해지는 게 더 부자연스럽다.
- **레벨 전환은 Seamless Travel (`bUseSeamlessTravel = true`).** `bIsRoomOwner`/`bIsReady`(PlayerState)와 PlayerController를 메인 레벨까지 그대로 들고 가야 하는데, Hard Travel은 재접속을 일으켜 이 둘이 새로 생성된다.
  - 택하지 않은 대안: Hard `ServerTravel`. → PlayerState/PlayerController 보존이 안 되어 로비 상태가 전부 날아감. 탈락.
- **방장 승계 시 `bIsReady`는 그대로 둔다(해제하지 않음).** `bIsRoomOwner`와 `bIsReady`는 독립적인 상태라 권한 이전이 레디 해제로 이어질 인과관계가 없다.
  - 택하지 않은 대안: 승계 시 레디 강제 해제. → 설명되지 않는 예외 규칙이 하나 더 생기고, 증명 목표에 기여 없는 로비 UX 디테일로 스코프만 늘린다. 탈락.

## 3. 구조

- `AGDSLobbyGameMode` — `PostLogin`에서 방장 지정(최초 접속자), 접속 순서를 `TArray<TWeakObjectPtr<AGDSPlayerController>>`로 관리, `Logout`에서 방장 이탈 감지 + 승계, 시작 요청 재검증 + `ServerTravel` 트리거
- `AGDSLobbyGameState` — `PlayerArray`의 추가·제거를 감지해 로비 UI에 플레이어 목록 변경을 알림
- `AGDSMainGameMode` — 뼈대
- `AGDSMainGameState` — 뼈대 (Phase 5에서 웨이브 상태로 채워질 자리)
- `AGDSPlayerState` — `bIsRoomOwner`, `bIsReady` (둘 다 복제 + `OnRep_`). 각 `OnRep_`에서 상태 변경 델리게이트를 발생시킴. Seamless Travel에 보존됨
- `AGDSPlayerController` — 레디 토글 요청(`ServerSetReady` 송신), 시작 요청(`ServerRequestStart` 송신), 로컬에서 GameState 준비 감지와 LobbyWidget 생성·제거 담당. Seamless Travel에 보존됨
- `AGDSCharacter` — 로비·게임플레이 공용 스폰, 전환 시 재스폰. Phase 0 검증용으로 WASD 평면 이동과 마우스 Yaw/Pitch 회전만 제공
- `ULobbyWidget` — 레디 버튼, 시작 버튼, 전체 플레이어 목록 소유. `AGDSLobbyGameState`의 목록 변경을 구독하고 `PlayerArray`를 기준으로 목록과 시작 버튼 상태를 갱신
- `ULobbyPlayerEntryWidget` — 하나의 `AGDSPlayerState`를 구독해 `PlayerName`·`PlayerId`, 방장 여부, 레디 여부를 표시

**소스 배치:** GameFramework 클래스는 역할별로 `GameFramework/Character`, `GameFramework/GameMode`, `GameFramework/GameState`, `GameFramework/PlayerController`, `GameFramework/PlayerState`에 둔다. 로비 UI는 `UI/Lobby`에 둔다.

**관계:** GameMode가 PlayerState의 `bIsRoomOwner`를 설정(서버 권위). PlayerController는 자신의 PlayerState를 통해 레디를 요청한다. 로컬 PlayerController는 `UWorld::GameStateSetEvent`를 구독하고 로비 GameState가 준비되면 해당 인스턴스를 LobbyWidget 생성 시 주입한다. LobbyWidget은 주입받은 GameState의 `PlayerArray`와 각 PlayerState의 상태 변경 델리게이트를 구독해 목록을 표시하고, 방장 클라이언트에서 전원 레디를 판정한다. 이벤트 등록 전에 GameState가 준비됐을 수 있으므로 등록 직후 현재 GameState도 확인하고, UI 바인딩 직후 현재 값을 직접 읽어 최초 상태를 동기화한다.

## 4. 데이터 / 실행 흐름

- **접속:** 클라 접속 → `GameMode::PostLogin` → 첫 접속자면 해당 `PlayerState::bIsRoomOwner = true`(서버) → 복제 → 모든 클라 UI에 방장 표시
- **UI 준비:** 로컬 PlayerController가 Playing 상태 진입 → `UWorld::GameStateSetEvent` 구독 + 현재 GameState 즉시 확인 → `AGDSLobbyGameState`라면 LobbyWidget 생성 후 GameState 주입
- **플레이어 목록:** `GameState::AddPlayerState`/`RemovePlayerState` → 목록 변경 델리게이트 → 각 클라 LobbyWidget이 `PlayerArray`를 기준으로 행 추가·제거 및 상태 바인딩
- **레디:** 클라 입력 → `PlayerController::ServerSetReady(bool)` → 서버가 `PlayerState::bIsReady` 변경 → 복제 → 각 클라 `OnRep_bIsReady` → 상태 변경 델리게이트 → LobbyWidget/PlayerEntryWidget 갱신 → 방장 클라는 전원 레디 재평가 → 만족 시 시작 버튼 로컬 활성화(서버 미통지)
- **시작:** 방장 클라 입력 → `PlayerController::ServerRequestStart()` → 서버가 (a) 요청자가 현재 방장인지 (b) 전원 `bIsReady`인지 재검증 → 통과 시 `ServerTravel`(Seamless) 호출, 실패 시 거절
- **방장 이탈:** 클라 종료 → `GameMode::Logout` → 접속 순서 배열에서 이탈자와 무효 참조 제거 → 이탈자가 방장이었다면 배열의 첫 번째 유효한 남은 플레이어의 `PlayerState::bIsRoomOwner = true`로 전환(그 유저의 `bIsReady`는 유지) → 복제로 전파
- **전환:** Seamless Travel → `AGDSMainGameMode`/`AGDSMainGameState` 로드 → PlayerState/PlayerController 보존된 채로 `AGDSCharacter` 재스폰

## 5. 네트워크 측면

- **서버 권위:** 방장 지정(`PostLogin`), `bIsReady` 변경(`ServerSetReady` 처리), 시작 트리거 최종 결정(`ServerRequestStart` 재검증), 방장 승계(`Logout`)
- **복제:** `bIsRoomOwner`, `bIsReady` 모두 조건 없이 전체 클라이언트에 복제(`COND_None`). 로비 UI에서 모든 참가자의 방장/레디 상태를 전원이 봐야 하므로 숨길 이유가 없다. `ReplicationCondition`은 이 시스템의 증명 대상이 아님(Phase 4가 그 자리).
- **RPC:**
  - `ServerSetReady(bool)` — Server RPC. Validation: 호출자 자신의 PlayerState만 변경 가능(다른 플레이어 것을 인자로 받아 바꾸는 형태 금지).
  - `ServerRequestStart()` — Server RPC. Validation: 호출자가 방장인지 + 전원 레디인지 서버가 재검사.
- **클라이언트 예측:** 없음. 방장 클라의 "전원 레디" 선반영은 prediction이 아니라 UI 반응성 목적의 로컬 판단이며, 실제 권위(시작 트리거)는 전부 서버 재검증 후에만 발생한다.
- **최적화:** 해당 없음(Phase 0 스코프, 트래픽 미미). Relevancy/Dormancy는 Phase 6 대상.

## 6. 시스템 고유 제약

- `bIsRoomOwner`/`bIsReady` 둘 다 `OnRep_` 콜백을 가져야 한다. 생성자/`BeginPlay`에서 복제값을 기다리지 않는다(`constraints.md` 3항).
- `ServerSetReady`는 호출자 자신의 PlayerState만 변경 가능하도록 서버에서 검증한다.
- 플레이어 목록은 엔진이 관리하는 `GameState::PlayerArray`를 사용하고 별도 복제 배열을 만들지 않는다.
- `PlayerState` 포인터를 UI 행의 내부 식별자로 사용한다. 표시는 기본 복제되는 `PlayerName`과 `PlayerId`를 함께 사용하며, 상태 변경 델리게이트에 별도 식별 데이터를 복제하거나 복사하지 않는다.
- LobbyWidget은 `GameState::PlayerArray`의 실제 PlayerState 값을 읽어 시작 버튼을 판정한다. 조건은 로컬 PlayerState가 방장이고, 목록이 비어 있지 않으며, 방장을 포함한 모든 PlayerState가 레디인 경우다.
- LobbyWidget은 GameState를 매 프레임 탐색하지 않는다. PlayerController가 `UWorld::GameStateSetEvent`로 준비를 감지해 주입하며, Travel·`EndPlay`에서 이벤트 구독과 위젯을 정리한다.
- Phase 0 캐릭터 입력은 C++에서 생성한 임시 Enhanced Input Mapping Context로 WASD와 `Mouse2D`만 바인딩한다. 에셋 기반 입력 구성과 어빌리티 활성 바인딩은 Phase 2에서 별도로 설계하며, 이 임시 입력 구조를 확장하지 않는다.
- Seamless Travel 사용 시 `GameMode::bUseSeamlessTravel = true` 설정이 필요하며, 전환 후 `AGDSMainGameMode` 쪽 초기화 순서가 "PlayerState/Controller가 이미 보존되어 있다"는 전제를 깨지 않아야 한다. (Phase 1에서 ASC 초기화 타이밍과의 상호작용 재점검 필요 — 9항 참조.)

## 7. 구현 지시 (Codex용)

### P0 — DS + 클라 2 접속, 코어 프레임워크 뼈대, Character 스폰/이동 확인
- **만들 것 (순서):** 1) 프로젝트 모듈 + `LogGDS` 카테고리 → 2) Dedicated Server 타깃 → 3) `AGDSLobbyGameMode`/`AGDSLobbyGameState`, `AGDSMainGameMode`/`AGDSMainGameState` 뼈대 클래스 → 4) `AGDSPlayerState`(`bIsRoomOwner`, `bIsReady` 선언 + 복제 등록 + `OnRep_` 빈 구현) → 5) `AGDSPlayerController` 뼈대 → 6) `AGDSCharacter`(WASD 이동 + 마우스 회전만 제공하는 기본 폰) → 7) `/Game/Levels/Lobby`, `/Game/Levels/Main` 맵과 GameMode 연결
- **멈춤 지점:** 여기까지 하고 멈춰 보고한다.
- **통과 기준 (사람 확인):** 컴파일 통과 + PIE에서 DS 1 + 클라 2 접속 + 각 클라에서 `AGDSCharacter` 스폰 확인 + WASD 이동과 마우스 회전 입력이 동작하고 이동이 서버 및 다른 클라이언트에 반영되는지 확인

### P1 — 로비 흐름 전체(레디 → 시작 → Seamless Travel) + 방장 승계
- **만들 것 (순서):** 1) `PostLogin` 방장 지정 + 접속 순서 배열 관리 → 2) `ServerSetReady` RPC + validation + PlayerState 상태 변경 델리게이트 → 3) GameState의 플레이어 목록 변경 델리게이트 → 4) LobbyWidget/PlayerEntryWidget과 레디·시작·전체 플레이어 상태 목록 → 5) 방장 클라의 전원 레디 재평가 + 로컬 시작 버튼 활성화 → 6) `ServerRequestStart` RPC + 서버 재검증(방장 여부 + 전원 레디) → 7) 통과 시 Seamless Travel로 `/Game/Levels/Main` 이동(`bUseSeamlessTravel = true`) → 8) `Logout`에서 방장 이탈 감지 + 접속 순서상 가장 앞선 유효 플레이어에게 승계(`bIsReady` 유지)
- **멈춤 지점:** 여기까지 하고 멈춰 보고한다.
- **통과 기준 (사람 확인):** 클라 2 PIE에서 (a) 레디 토글 시 양쪽 클라 UI 모두 갱신 확인 (b) 방장만 시작 가능, 전원 레디 전엔 버튼 비활성 확인 (c) 시작 시 양쪽 클라 모두 메인 레벨로 전환되고 `PlayerState`의 `bIsRoomOwner`/`bIsReady` 값이 유지됨을 로그로 확인 (d) 방장이 먼저 나간 뒤 남은 클라가 새 방장으로 지정되고 기존 `bIsReady` 값이 유지됨을 로그로 확인

## 8. 시스템 인수 검증 (전체)

- **결과:** 2026-06-25 통과. DS 1 + 클라 2 PIE에서 레디 동기화, 방장 전용 시작 조건, Seamless Travel 상태 보존, 방장 이탈 승계를 확인함.
- **추가 확인 필요:** 같은 날 추가한 Phase 0 최소 이동(WASD + 마우스 회전)의 입력 동작과 CharacterMovement 복제는 사람의 PIE 확인이 남아 있음.
- **시나리오:** DS 1 + 클라 2 PIE → 두 클라 접속 → 방장 자동 지정 확인 → 양쪽 레디 토글 → 방장 시작 버튼 활성화 확인 → 방장이 시작 → Seamless Travel로 메인 레벨 전환 + PlayerState 값 보존 로그 확인 → (별도 케이스) 방장 접속 종료 → 남은 클라가 새 방장으로 승계되고 레디 상태가 유지되는지 확인
- **서버-클라 검증:** 로그에 `[Server]`/`[Client]` 태그로 방장 지정/레디 변경/시작 검증/Travel 트리거 시점을 모두 남겨, PIE 멀티 인스턴스에서 교차 확인한다.
- **증명 목표 시연 여부:** 서버 권위(방장 지정·시작 검증), RPC 구조(Server RPC + validation), 복제(OnRep 기반 상태 동기화)를 시연한다. `ReplicationCondition`/Lag Compensation 등은 이 시스템 스코프가 아니다(Phase 4/이후).

## 9. 미해결 / 추후

- 게임플레이 진입 후(메인 레벨)에서 방장이 나가는 경우의 처리 — 로비 스코프 밖, 추후 별도 결정
- `ServerRequestStart` 검증 실패 시 클라에 실패 사유를 알릴지 여부 — 현재는 무응답 처리, 추후 UX 필요 시 결정
- Seamless Travel 환경에서 PlayerState 복제 타이밍과 Phase 1 ASC 초기화 시점의 상호작용 — Phase 1 설계에서 재점검 필요
- `AGDSMainGameState`의 실제 책임 — 현재 뼈대만 존재하며 Phase 5(웨이브)에서 채워짐
