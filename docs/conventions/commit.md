# 커밋 메시지 규칙

git history가 이 프로젝트의 **작업 로그이자 포트폴리오 타임라인**이다. 별도 보고서를 쓰지 않는 대신, 커밋 메시지를 제대로 쓴다.

---

## 1. 형식

```
<type>(<scope>): <subject>

<body — 왜 이렇게 했는지>
```

- 첫 줄(subject)은 50자 이내, 명령형, 마침표 없음.
- body는 한 줄 비우고 작성. **무엇을 했는지보다 "왜"를 적는다.** (무엇을 했는지는 diff가 말해준다.)
- 한글로 작성한다. type/scope 키워드만 영어.

## 2. type

| type | 의미 |
|---|---|
| `feat` | 새 기능/시스템 구현 |
| `fix` | 버그 수정 |
| `refactor` | 동작 변경 없는 구조 개선 |
| `perf` | 성능 최적화 (Relevancy, Dormancy, Pooling 등 — 이 프로젝트의 핵심) |
| `net` | 네트워크/리플리케이션 관련 작업 |
| `docs` | architecture/지침 등 문서 작업 |
| `chore` | 빌드/설정/잡일 |

> `perf`, `net`을 따로 둔 이유: 이 프로젝트의 증명 포인트라서, 나중에 history를 훑을 때 최적화·네트워크 작업이 한눈에 모이게 하려는 것.

## 3. scope

작업한 시스템. `architecture/` 파일명과 맞추면 추적이 쉽다.
예: `gas`, `network`, `weapon`, `ai`, `input`, `pool`

## 4. 예시

```
feat(gas): CombatAttributeSet에 Health/Shield 추가

GE 기반 데미지 처리를 위한 기반 Attribute. 직접 수정 금지,
모든 변경은 GameplayEffect를 통하도록 PreAttributeChange에서 클램프.
```

```
net(network): 적 위치 정보에 ReplicationCondition 적용

같은 팀에게만 미니맵 정보가 복제되도록 COND_ 조건 지정.
팀 기반 정보 은닉 요구사항 충족.
```

```
perf(pool): 프로젝타일 Object Pooling 도입

웨이브당 다수 발사체 생성/파괴로 인한 GC 스파이크 제거.
스폰 비용을 미리 확보한 풀에서 재사용으로 대체.
```

## 5. 단위

- 커밋은 **의미 있는 작업 단위**로 끊는다. "WIP" 떡칠을 피한다.
- 시스템 하나가 끝나면 history만 봐도 그 시스템이 어떤 결정으로 만들어졌는지 읽히도록 한다.
