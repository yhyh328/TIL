#!/bin/bash

# Docker 이미지를 tar 파일에서 import하는 스크립트
# 사용법: ./import-images.sh [backend-tar-file] [frontend-tar-file]
# 예: ./import-images.sh docker-images/backend-20241101.tar.gz docker-images/frontend-20241101.tar.gz

set -e

BACKEND_TAR=${1:-""}
FRONTEND_TAR=${2:-""}

if [ -z "$BACKEND_TAR" ] || [ -z "$FRONTEND_TAR" ]; then
    echo "📥 Docker 이미지를 tar 파일에서 import합니다..."
    echo ""
    echo "사용법: ./import-images.sh [backend-tar-file] [frontend-tar-file]"
    echo ""
    echo "현재 디렉토리의 docker-images 폴더에서 자동으로 찾기:"
    
    # 현재 디렉토리에서 최신 파일 찾기
    # 먼저 .tar.gz 파일을 찾고, 없으면 .tar 파일을 찾음
    if [ -d "./docker-images" ]; then
        BACKEND_TAR=$(ls -t ./docker-images/backend-*.tar.gz ./docker-images/backend-*.tar 2>/dev/null | head -1)
        FRONTEND_TAR=$(ls -t ./docker-images/frontend-*.tar.gz ./docker-images/frontend-*.tar 2>/dev/null | head -1)
    else
        # 현재 디렉토리에서 찾기
        BACKEND_TAR=$(ls -t ./backend-*.tar.gz ./backend-*.tar 2>/dev/null | head -1)
        FRONTEND_TAR=$(ls -t ./frontend-*.tar.gz ./frontend-*.tar 2>/dev/null | head -1)
    fi
    
    if [ -z "$BACKEND_TAR" ] || [ -z "$FRONTEND_TAR" ]; then
        echo "❌ 이미지 파일을 찾을 수 없습니다."
        echo "   수동으로 파일 경로를 지정하세요."
        echo "   예: ./import-images.sh backend-20241101.tar frontend-20241101.tar"
        exit 1
    fi
fi

echo "사용할 파일:"
echo "  Backend: $BACKEND_TAR"
echo "  Frontend: $FRONTEND_TAR"
echo ""

# 파일 존재 확인
if [ ! -f "$BACKEND_TAR" ]; then
    echo "❌ Backend 파일을 찾을 수 없습니다: $BACKEND_TAR"
    exit 1
fi

if [ ! -f "$FRONTEND_TAR" ]; then
    echo "❌ Frontend 파일을 찾을 수 없습니다: $FRONTEND_TAR"
    exit 1
fi

# 파일 크기 확인
BACKEND_SIZE=$(stat -f%z "$BACKEND_TAR" 2>/dev/null || stat -c%s "$BACKEND_TAR" 2>/dev/null || echo "0")
FRONTEND_SIZE=$(stat -f%z "$FRONTEND_TAR" 2>/dev/null || stat -c%s "$FRONTEND_TAR" 2>/dev/null || echo "0")

if [ "$BACKEND_SIZE" -eq 0 ]; then
    echo "❌ Backend 파일이 비어있거나 손상되었을 수 있습니다: $BACKEND_TAR"
    exit 1
fi

if [ "$FRONTEND_SIZE" -eq 0 ]; then
    echo "❌ Frontend 파일이 비어있거나 손상되었을 수 있습니다: $FRONTEND_TAR"
    exit 1
fi

echo "파일 크기 확인:"
echo "  Backend: $(numfmt --to=iec-i --suffix=B $BACKEND_SIZE 2>/dev/null || echo "${BACKEND_SIZE} bytes")"
echo "  Frontend: $(numfmt --to=iec-i --suffix=B $FRONTEND_SIZE 2>/dev/null || echo "${FRONTEND_SIZE} bytes")"
echo ""

# 압축 해제가 필요한지 확인
BACKEND_TAR_FINAL="$BACKEND_TAR"
FRONTEND_TAR_FINAL="$FRONTEND_TAR"
TEMP_BACKEND=""
TEMP_FRONTEND=""

if [[ "$BACKEND_TAR" == *.gz ]]; then
    echo "📦 Backend 파일 압축 해제 중..."
    TEMP_BACKEND=$(mktemp)
    if gunzip -c "$BACKEND_TAR" > "$TEMP_BACKEND" 2>/dev/null; then
        BACKEND_TAR_FINAL="$TEMP_BACKEND"
        echo "  ✅ 압축 해제 완료"
    else
        echo "⚠️  압축 해제 실패, 원본 파일을 사용합니다..."
        BACKEND_TAR_FINAL="$BACKEND_TAR"
        rm -f "$TEMP_BACKEND"
        TEMP_BACKEND=""
    fi
else
    echo "📦 Backend 파일은 이미 압축 해제되어 있습니다"
fi

if [[ "$FRONTEND_TAR" == *.gz ]]; then
    echo "📦 Frontend 파일 압축 해제 중..."
    TEMP_FRONTEND=$(mktemp)
    if gunzip -c "$FRONTEND_TAR" > "$TEMP_FRONTEND" 2>/dev/null; then
        FRONTEND_TAR_FINAL="$TEMP_FRONTEND"
        echo "  ✅ 압축 해제 완료"
    else
        echo "⚠️  압축 해제 실패, 원본 파일을 사용합니다..."
        FRONTEND_TAR_FINAL="$FRONTEND_TAR"
        rm -f "$TEMP_FRONTEND"
        TEMP_FRONTEND=""
    fi
else
    echo "📦 Frontend 파일은 이미 압축 해제되어 있습니다"
fi

echo ""
echo "📥 이미지 import 중..."

# docker load 실행
if docker load -i "$BACKEND_TAR_FINAL"; then
    echo "  ✅ Backend 이미지 import 완료"
else
    echo "❌ Backend 이미지 import 실패"
    [ -n "$TEMP_BACKEND" ] && rm -f "$TEMP_BACKEND"
    [ -n "$TEMP_FRONTEND" ] && rm -f "$TEMP_FRONTEND"
    exit 1
fi

if docker load -i "$FRONTEND_TAR_FINAL"; then
    echo "  ✅ Frontend 이미지 import 완료"
else
    echo "❌ Frontend 이미지 import 실패"
    [ -n "$TEMP_BACKEND" ] && rm -f "$TEMP_BACKEND"
    [ -n "$TEMP_FRONTEND" ] && rm -f "$TEMP_FRONTEND"
    exit 1
fi

# 임시 파일 정리
if [ -n "$TEMP_BACKEND" ]; then
    rm -f "$TEMP_BACKEND"
fi
if [ -n "$TEMP_FRONTEND" ]; then
    rm -f "$TEMP_FRONTEND"
fi

echo ""
echo "✅ 이미지 import가 완료되었습니다!"
echo ""
echo "이제 ./deploy.sh prod 또는 docker compose -f docker-compose.prod.yaml --env-file .env up -d 를 실행하세요"

