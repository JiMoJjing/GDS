#!/usr/bin/env bash
# 회고용 diff 묶음 생성기 (retro.md 4항 입력 생성)
# 마지막 회고 이후의 git log -p + 마찰 핫스팟 + friction-log 를 한 파일로 묶어
# Claude 회고 triage 입력을 만든다. GDS 소스가 아니라 회고 보조 도구다.
set -eu

RETRO_TAG_PREFIX="retro/"
FRICTION_LOG="docs/friction-log.md"
OUT="retro-bundle.md"

usage() {
    cat <<'EOF'
사용법:
  retro-bundle.sh [-- <경로...>]   마지막 회고 이후 묶음 생성 -> retro-bundle.md
  retro-bundle.sh --mark           회고 완료 표시 (HEAD 에 retro/<날짜시각> 태그)
  retro-bundle.sh -h | --help      도움말

옵션:
  -- <경로...>   특정 시스템 파일로 범위 제한 (예: -- Source/GDS/Abilities)
EOF
}

MODE="bundle"
PATHS=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        --mark) MODE="mark"; shift ;;
        -h|--help) usage; exit 0 ;;
        --) shift; PATHS=("$@"); break ;;
        *) echo "알 수 없는 인자: $1" >&2; usage; exit 1 ;;
    esac
done

cd "$(git rev-parse --show-toplevel)"

if [[ "$MODE" == "mark" ]]; then
    TAG="${RETRO_TAG_PREFIX}$(date +%Y%m%d-%H%M%S)"
    git tag "$TAG"
    echo "회고 시점 표시: $TAG (HEAD=$(git rev-parse --short HEAD))"
    exit 0
fi

# 경로 제한 인자 구성 (빈 배열 안전 처리)
PATHSPEC=()
if [[ ${#PATHS[@]} -gt 0 ]]; then PATHSPEC=(-- "${PATHS[@]}"); fi

# 범위: 마지막 retro 태그 .. HEAD (없으면 첫 커밋부터)
LAST_RETRO_TAG="$(git tag --list "${RETRO_TAG_PREFIX}*" --sort=-creatordate | head -n1)"
if [[ -n "$LAST_RETRO_TAG" ]]; then
    RANGE="${LAST_RETRO_TAG}..HEAD"
    SINCE_DESC="마지막 회고(${LAST_RETRO_TAG}) 이후"
else
    RANGE="HEAD"
    SINCE_DESC="첫 회고 (전체 히스토리)"
fi

if [[ -z "$(git rev-list "$RANGE" ${PATHSPEC[@]+"${PATHSPEC[@]}"})" ]]; then
    echo "범위에 커밋 없음: $RANGE" >&2
    exit 0
fi

{
    echo "# 회고 묶음 — $(date '+%Y-%m-%d %H:%M')"
    echo
    echo "- 범위: \`${RANGE}\` (${SINCE_DESC})"
    if [[ ${#PATHS[@]} -gt 0 ]]; then echo "- 경로 제한: ${PATHS[*]}"; fi
    echo

    echo "## 1. 커밋 요약 (모양 보기)"
    echo '```'
    git log "$RANGE" --oneline ${PATHSPEC[@]+"${PATHSPEC[@]}"}
    echo '```'
    echo

    echo "## 2. 마찰 핫스팟 (자동 추출 — 신호일 뿐, 판단은 사람)"
    echo
    echo "### 2a. feat 이후 같은 scope 에 fix/refactor/perf 가 온 경우"
    git log "$RANGE" --reverse --pretty='%h%x09%s' ${PATHSPEC[@]+"${PATHSPEC[@]}"} | awk -F'\t' '
    {
        hash=$1; subj=$2;
        op=index(subj,"("); cp=index(subj,")");
        if (op>1 && cp>op && substr(subj,cp,2)=="):") {
            type=substr(subj,1,op-1);
            scope=substr(subj,op+1,cp-op-1);
            if (type=="feat") { feat[scope]=feat[scope] hash " "; }
            else if (type=="fix" || type=="refactor" || type=="perf") {
                if (scope in feat) {
                    print "- [" scope "] " type ": " hash "  \"" subj "\"  (이전 feat: " feat[scope] ")";
                    found=1;
                }
            }
        }
    }
    END { if (!found) print "_해당 없음_"; }'
    echo
    echo "### 2b. 가장 자주 수정된 파일 (churn 상위)"
    echo '```'
    git log "$RANGE" --name-only --pretty=format: ${PATHSPEC[@]+"${PATHSPEC[@]}"} | sed '/^$/d' | sort | uniq -c | sort -rn | head -n 15
    echo '```'
    echo

    echo "## 3. 마찰 로그 (비-diff 신호)"
    if [[ -s "$FRICTION_LOG" ]]; then
        echo '```'
        cat "$FRICTION_LOG"
        echo '```'
    else
        echo "_friction-log.md 비어 있음 또는 없음_"
    fi
    echo

    echo "## 4. 전체 diff (git log -p)"
    echo '```diff'
    git log "$RANGE" -p ${PATHSPEC[@]+"${PATHSPEC[@]}"}
    echo '```'
} > "$OUT"

echo "생성됨: $OUT  (Claude 회고에 통째로 붙여넣어라)"