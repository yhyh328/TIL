#!/bin/bash

# Docker 이미지를 레지스트리에 푸시하는 스크립트
# 사용법: ./push-images.sh [dockerhub-username]
# 예: ./push-images.sh myusername

set -e

DOCKERHUB_USER=${1:-""}
IMAGE_TAG=${2:-latest}

if [ -z "$DOCKERHUB_USER" ]; then
    echo "❌ Docker Hub 사용자명이 필요합니다."
    echo "사용법: ./push-images.sh [dockerhub-username] [tag]"
    echo "예: ./push-images.sh myusername v1.0.0"
    exit 1
fi

echo "🚀 Docker 이미지를 빌드하고 레지스트리에 푸시합니다..."
echo "사용자: $DOCKERHUB_USER"
echo "태그: $IMAGE_TAG"
echo ""

# 먼저 이미지 빌드
echo "📦 이미지 빌드 중..."
docker compose -f docker-compose.prod.yaml build

# 이미지 태그 지정
BACKEND_IMAGE="${DOCKERHUB_USER}/fullstack-backend:${IMAGE_TAG}"
FRONTEND_IMAGE="${DOCKERHUB_USER}/fullstack-frontend:${IMAGE_TAG}"

echo ""
echo "🏷️  이미지 태그 지정 중..."
docker tag docker-fullstack-backend:latest "$BACKEND_IMAGE"
docker tag docker-fullstack-frontend:latest "$FRONTEND_IMAGE"

echo ""
echo "📤 Docker Hub에 로그인 중..."
echo "Docker Hub 비밀번호를 입력하세요:"
docker login -u "$DOCKERHUB_USER"

echo ""
echo "📤 이미지 푸시 중..."
docker push "$BACKEND_IMAGE"
docker push "$FRONTEND_IMAGE"

echo ""
echo "✅ 이미지 푸시가 완료되었습니다!"
echo ""
echo "다른 컴퓨터에서 다음 명령으로 이미지를 받을 수 있습니다:"
echo "  docker pull $BACKEND_IMAGE"
echo "  docker pull $FRONTEND_IMAGE"
echo ""
echo "또는 ./pull-images.sh $DOCKERHUB_USER $IMAGE_TAG 를 사용하세요"

