#!/bin/bash

# 배포 문제 해결 스크립트

echo "🔍 배포 문제 진단 중..."
echo ""

# 1. .env 파일 확인
echo "1️⃣ .env 파일 확인:"
if [ ! -f ".env" ]; then
    echo "   ❌ .env 파일이 없습니다!"
    echo "   해결: cp env.example .env 후 편집하세요"
    exit 1
fi

echo "   ✅ .env 파일 존재"

# 2. 필수 환경 변수 확인
echo ""
echo "2️⃣ 필수 환경 변수 확인:"
MISSING_VARS=()

if ! grep -q "^DB_PASSWORD=" .env || [ -z "$(grep "^DB_PASSWORD=" .env | cut -d'=' -f2)" ]; then
    MISSING_VARS+=("DB_PASSWORD")
fi

if [ ${#MISSING_VARS[@]} -gt 0 ]; then
    echo "   ❌ 다음 환경 변수가 누락되었습니다: ${MISSING_VARS[*]}"
    echo "   해결: .env 파일을 열어 다음을 설정하세요:"
    for var in "${MISSING_VARS[@]}"; do
        echo "   $var=your-value-here"
    done
    exit 1
fi

echo "   ✅ 필수 환경 변수 모두 설정됨"

# 3. Docker 이미지 확인
echo ""
echo "3️⃣ Docker 이미지 확인:"
if ! docker images | grep -q "docker-fullstack-backend"; then
    echo "   ❌ Backend 이미지를 찾을 수 없습니다"
    echo "   해결: ./import-images.sh 또는 ./pull-images.sh 실행"
    exit 1
fi

if ! docker images | grep -q "docker-fullstack-frontend"; then
    echo "   ❌ Frontend 이미지를 찾을 수 없습니다"
    echo "   해결: ./import-images.sh 또는 ./pull-images.sh 실행"
    exit 1
fi

echo "   ✅ 필요한 이미지 모두 존재"

# 4. 포트 확인
echo ""
echo "4️⃣ 포트 사용 확인:"
BACKEND_PORT=$(grep "^BACKEND_PORT=" .env 2>/dev/null | cut -d'=' -f2 || echo "8080")
FRONTEND_PORT=$(grep "^FRONTEND_PORT=" .env 2>/dev/null | cut -d'=' -f2 || echo "80")
DB_PORT=$(grep "^DB_PORT=" .env 2>/dev/null | cut -d'=' -f2 || echo "5432")

if netstat -tuln 2>/dev/null | grep -q ":$BACKEND_PORT " || ss -tuln 2>/dev/null | grep -q ":$BACKEND_PORT "; then
    echo "   ⚠️  Backend 포트 ($BACKEND_PORT)가 이미 사용 중입니다"
fi

if netstat -tuln 2>/dev/null | grep -q ":$FRONTEND_PORT " || ss -tuln 2>/dev/null | grep -q ":$FRONTEND_PORT "; then
    echo "   ⚠️  Frontend 포트 ($FRONTEND_PORT)가 이미 사용 중입니다"
fi

if netstat -tuln 2>/dev/null | grep -q ":$DB_PORT " || ss -tuln 2>/dev/null | grep -q ":$DB_PORT "; then
    echo "   ⚠️  Database 포트 ($DB_PORT)가 이미 사용 중입니다"
fi

echo "   ✅ 포트 확인 완료"

# 5. db 폴더 확인
echo ""
echo "5️⃣ db 폴더 확인:"
if [ ! -d "db" ]; then
    echo "   ⚠️  db 폴더가 없습니다 (생성 중...)"
    mkdir -p db
    echo "   ✅ db 폴더 생성됨"
else
    echo "   ✅ db 폴더 존재"
fi

# 6. 이전 컨테이너 정리 제안
echo ""
echo "6️⃣ 이전 컨테이너 확인:"
if docker compose -f docker-compose.prod.images.yaml ps 2>/dev/null | grep -q "pg-prod\|springboot-prod\|react-prod"; then
    echo "   ⚠️  이전 컨테이너가 실행 중일 수 있습니다"
    echo "   해결 명령어:"
    echo "   docker compose -f docker-compose.prod.images.yaml down"
fi

echo ""
echo "✅ 진단 완료!"
echo ""
echo "배포 시도:"
echo "  docker compose -f docker-compose.prod.images.yaml --env-file .env up -d"
echo ""
echo "문제가 계속되면 로그 확인:"
echo "  docker compose -f docker-compose.prod.images.yaml logs db"

