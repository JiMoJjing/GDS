# Friction Log

> 상시 기록용. 설계 빈 칸, 스펙 모호, 기존 관례와 충돌 등 거슬렸던 지점을 **한 줄씩** 적는다.
> diff성 증거(무엇을 고쳤는지)는 git이 이미 기록하므로 따로 안 적어도 된다 — 여기는 "왜 거슬렸는지"만.
> `retro.md` 회고 시점에 일괄 처리되고, 처리 후 비운다(durable 기록은 회고 결과로 남은 지침 diff와 커밋).

**형식:** `- [날짜] (scope) 한 줄 설명 — 그 자리에서 어떻게 처리했는지`

---

- [2026-06-25] (lobby/ui) 로비 UI가 필요하다고만 적혀 있고 레디·시작 버튼, 전체 참가자 목록, 자기 자신과 방장 포함 여부 같은 화면 계약이 없어 구현 범위를 확정할 수 없었음 — 사람이 필요한 구성과 활성화 조건을 추가로 결정함
- [2026-06-25] (lobby/ui) 플레이어 목록을 어떤 복제 데이터로 구성하고 각 행을 어떻게 식별할지 정의되지 않았음 — `GameState::PlayerArray`를 목록 원천으로, `PlayerState` 포인터를 내부 식별자로, `PlayerName`과 `PlayerId`를 표시 정보로 결정함
- [2026-06-25] (lobby/ui) 플레이어 추가·제거와 개별 PlayerState 상태 변경을 UI에 전달할 책임 경계가 없었음 — GameState는 목록 변경, PlayerState는 방장·레디 변경, 각 UI 행은 한 PlayerState 구독으로 역할을 분리함
- [2026-06-25] (lobby/ui) UI를 C++ 네이티브 위젯으로 만들지 Widget Blueprint 에셋으로 만들지 산출물 형태가 명시되지 않았음 — 별도 에셋 없이 실행 가능한 C++ UMG 위젯으로 구현함
- [2026-06-25] (lobby/init) 로컬 UI를 어느 수명주기에서 만들고 GameState 준비를 어떻게 감지할지 정의되지 않았음 — 처음에는 Widget `NativeTick` 재시도를 사용했다가 `UWorld::GameStateSetEvent` 구독과 GameState 주입 방식으로 재작업함
- [2026-06-25] (lobby/init) `BeginPlayingState`가 Pawn의 Playing 상태 진입 훅이지 PlayerState/GameState 준비 완료 훅이라는 보장은 없다는 점이 설계에 드러나지 않았음 — 이벤트 등록 직후 현재 GameState도 확인하고 Travel·EndPlay에서 구독을 해제하도록 보완함
- [2026-06-25] (lobby/scope) P0 지시에 “여기까지 하고 멈춤”이 있었지만 사용자의 “작업 시작”이 P1 UI까지 포함하는지 기대가 맞지 않아 UI가 누락된 것처럼 보였음 — P0 커밋 뒤 사용자의 지적으로 P1 전체를 이어서 구현함
- [2026-06-25] (lobby/character) P0 통과 기준에는 캐릭터 이동 검증이 있었지만 Enhanced Input은 후속 Phase 대상으로 적혀 있어 임시 입력 구현 여부가 모호했음 — 사람이 Phase 0에서 이동을 제외하고 스폰 확인만 남김
- [2026-06-25] (lobby/maps) 로비·메인 맵의 실제 에셋 경로와 제작 주체가 정해지지 않아 GameMode 설정과 Travel URL을 연결할 수 없었음 — 사람이 `/Game/Levels/Lobby`, `/Game/Levels/Main` 맵을 만들고 Codex가 설정을 연결함
- [2026-06-25] (lobby/owner) “가장 먼저 접속한 남은 플레이어”에게 방장을 승계한다는 규칙은 있었지만 접속 순서를 보존하는 자료구조가 없었음 — GameMode의 `TArray<TWeakObjectPtr<AGDSPlayerController>>`로 접속 순서를 명시적으로 관리하기로 결정함
- [2026-06-25] (lobby/test) 방장 이탈 인수 조건은 있었지만 PIE에서 한 클라이언트만 종료하는 구체적 절차가 없어 검증 방법을 다시 물어야 했음 — 별도 프로세스 PIE 또는 방장 클라이언트의 `disconnect` 명령으로 확인함
- [2026-06-25] (lobby/build) Dedicated Server 타깃 구현 지시는 있었지만 Epic Launcher 설치형 UE 5.4가 Server 타깃 빌드를 지원하지 않는 환경 전제가 문서에 없었음 — 에디터·Game 타깃 빌드로 코드를 검증하고 실제 Server 바이너리는 소스 빌드 엔진이 필요하다고 보고함
