#!/bin/bash

# Docker 이미지를 tar 파일로 export하는 스크립트
# 사용법: ./export-images.sh

set -e

OUTPUT_DIR="./docker-images"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

echo "📦 Docker 이미지를 tar 파일로 export합니다..."

# 출력 디렉토리 생성
mkdir -p "$OUTPUT_DIR"

# 먼저 이미지 빌드 (없는 경우)
echo "🔨 이미지 빌드 확인 중..."
docker compose -f docker-compose.prod.yaml build

echo ""
echo "📤 이미지 export 중..."
docker save docker-fullstack-backend:latest -o "${OUTPUT_DIR}/backend-${TIMESTAMP}.tar"
docker save docker-fullstack-frontend:latest -o "${OUTPUT_DIR}/frontend-${TIMESTAMP}.tar"

# 압축
echo ""
echo "🗜️  이미지 압축 중..."
gzip "${OUTPUT_DIR}/backend-${TIMESTAMP}.tar"
gzip "${OUTPUT_DIR}/frontend-${TIMESTAMP}.tar"

echo ""
echo "✅ 이미지 export가 완료되었습니다!"
echo ""
echo "생성된 파일:"
ls -lh "${OUTPUT_DIR}/"*.tar.gz
echo ""
echo "다른 컴퓨터로 전송 방법:"
echo "  scp ${OUTPUT_DIR}/*.tar.gz user@remote:/path/to/destination/"
echo "또는 USB 등으로 전송 후 import-images.sh를 실행하세요"

