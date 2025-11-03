#!/bin/bash

# Docker 이미지를 레지스트리에서 받아오는 스크립트
# 사용법: ./pull-images.sh [dockerhub-username] [tag]
# 예: ./pull-images.sh myusername latest

set -e

DOCKERHUB_USER=${1:-""}
IMAGE_TAG=${2:-latest}

if [ -z "$DOCKERHUB_USER" ]; then
    echo "❌ Docker Hub 사용자명이 필요합니다."
    echo "사용법: ./pull-images.sh [dockerhub-username] [tag]"
    echo "예: ./pull-images.sh myusername v1.0.0"
    exit 1
fi

BACKEND_IMAGE="${DOCKERHUB_USER}/fullstack-backend:${IMAGE_TAG}"
FRONTEND_IMAGE="${DOCKERHUB_USER}/fullstack-frontend:${IMAGE_TAG}"

echo "📥 Docker 이미지를 레지스트리에서 받아옵니다..."
echo "사용자: $DOCKERHUB_USER"
echo "태그: $IMAGE_TAG"
echo ""

docker pull "$BACKEND_IMAGE"
docker pull "$FRONTEND_IMAGE"

# 로컬 태그로 변경 (docker-compose에서 사용하기 위해)
echo ""
echo "🏷️  로컬 태그 지정 중..."
docker tag "$BACKEND_IMAGE" docker-fullstack-backend:latest
docker tag "$FRONTEND_IMAGE" docker-fullstack-frontend:latest

echo ""
echo "✅ 이미지 다운로드가 완료되었습니다!"
echo ""
echo "이제 ./deploy.sh prod 또는 docker compose -f docker-compose.prod.yaml --env-file .env up -d 를 실행하세요"

