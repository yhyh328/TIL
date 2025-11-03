#!/bin/bash

# 배포 스크립트
# 사용법: ./deploy.sh [dev|prod]

set -e

ENVIRONMENT=${1:-prod}
COMPOSE_FILE="compose.yaml"
ENV_FILE=".env"

if [ "$ENVIRONMENT" = "prod" ]; then
    COMPOSE_FILE="docker-compose.prod.yaml"
    
    # .env 파일 확인
    if [ ! -f "$ENV_FILE" ]; then
        echo "⚠️  .env 파일이 없습니다."
        echo "env.example을 복사하여 .env 파일을 생성하고 필요한 설정을 입력하세요:"
        echo "  cp env.example .env"
        echo "  # .env 파일을 편집하여 설정을 완료하세요"
        exit 1
    fi
    
    echo "🚀 프로덕션 환경 배포를 시작합니다..."
    docker compose -f "$COMPOSE_FILE" --env-file "$ENV_FILE" up -d --build
    
    echo ""
    echo "✅ 배포가 완료되었습니다!"
    echo ""
    echo "서비스 상태 확인:"
    docker compose -f "$COMPOSE_FILE" ps
    
    echo ""
    echo "로그 확인: docker compose -f $COMPOSE_FILE logs -f"
    echo "중지: docker compose -f $COMPOSE_FILE down"
    
elif [ "$ENVIRONMENT" = "dev" ]; then
    echo "🚀 개발 환경을 시작합니다..."
    docker compose -f "$COMPOSE_FILE" up -d --build
    
    echo ""
    echo "✅ 개발 환경이 시작되었습니다!"
    echo ""
    echo "서비스 상태 확인:"
    docker compose -f "$COMPOSE_FILE" ps
    
    echo ""
    echo "로그 확인: docker compose logs -f"
    echo "중지: docker compose down"
else
    echo "❌ 잘못된 환경입니다. 'dev' 또는 'prod'를 사용하세요."
    echo "사용법: ./deploy.sh [dev|prod]"
    exit 1
fi


