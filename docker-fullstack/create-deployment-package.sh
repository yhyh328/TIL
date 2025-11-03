#!/bin/bash

# 이미지 기반 배포를 위한 최소 패키지 생성 스크립트
# 사용법: ./create-deployment-package.sh [output-directory]

set -e

OUTPUT_DIR=${1:-"./deployment-package"}
TIMESTAMP=$(date +%Y%m%d_%H%M%S)

echo "📦 배포 패키지 생성 중..."

# 출력 디렉토리 생성
mkdir -p "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR/db"

# 필수 파일 복사
echo "📋 필수 파일 복사 중..."
cp docker-compose.prod.images.yaml "$OUTPUT_DIR/"
cp env.example "$OUTPUT_DIR/"
cp import-images.sh "$OUTPUT_DIR/"
cp pull-images.sh "$OUTPUT_DIR/"

# db/init.sql이 있으면 복사
if [ -f "db/init.sql" ]; then
    cp db/init.sql "$OUTPUT_DIR/db/"
    echo "  ✅ db/init.sql 복사됨"
else
    echo "  ⚠️  db/init.sql이 없습니다 (선택사항)"
    # 빈 init.sql 파일 생성 (없으면 docker-compose가 실패할 수 있음)
    touch "$OUTPUT_DIR/db/.gitkeep"
fi

# README 생성
cat > "$OUTPUT_DIR/README.md" << 'EOF'
# 이미지 기반 배포 패키지

이 패키지는 소스 코드 없이 Docker 이미지만으로 애플리케이션을 배포할 수 있습니다.

## 필요한 것

- Docker 및 Docker Compose 설치
- 이미지 파일 (tar.gz) 또는 Docker Hub 접근 권한

## 배포 방법

### 1. 이미지 준비

**방법 A: Docker Hub에서 받기**
```bash
./pull-images.sh [dockerhub-username] [tag]
```

**방법 B: 파일에서 import**
```bash
# 이미지 tar 파일을 이 디렉토리에 복사한 후
./import-images.sh
```

### 2. 환경 변수 설정

```bash
cp env.example .env
# .env 파일을 편집하여 실제 값 설정
```

### 3. 배포 실행

```bash
docker compose -f docker-compose.prod.images.yaml --env-file .env up -d
```

### 4. 서비스 확인

```bash
# 상태 확인
docker compose -f docker-compose.prod.images.yaml ps

# 로그 확인
docker compose -f docker-compose.prod.images.yaml logs -f
```

## 중지 및 제거

```bash
# 서비스 중지
docker compose -f docker-compose.prod.images.yaml down

# 볼륨 포함 삭제 (데이터 삭제됨)
docker compose -f docker-compose.prod.images.yaml down -v
```

## 파일 설명

- `docker-compose.prod.images.yaml`: 이미지 기반 docker-compose 설정
- `env.example`: 환경 변수 예시 파일
- `import-images.sh`: tar 파일에서 이미지 import 스크립트
- `pull-images.sh`: Docker Hub에서 이미지 pull 스크립트
- `db/init.sql`: 데이터베이스 초기화 스크립트 (선택사항)
EOF

chmod +x "$OUTPUT_DIR/import-images.sh"
chmod +x "$OUTPUT_DIR/pull-images.sh"

echo ""
echo "✅ 배포 패키지가 생성되었습니다: $OUTPUT_DIR"
echo ""
echo "패키지 내용:"
ls -lh "$OUTPUT_DIR"
echo ""
echo "배포 방법은 $OUTPUT_DIR/README.md를 참고하세요"

