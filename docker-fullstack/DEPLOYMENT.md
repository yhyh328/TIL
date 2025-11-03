# 배포 가이드 (Deployment Guide)

이 문서는 Docker를 사용한 풀스택 애플리케이션의 배포 방법을 설명합니다.

## 목차

1. [개요](#개요)
2. [사전 요구사항](#사전-요구사항)
3. [환경 설정](#환경-설정)
4. [로컬 개발 환경](#로컬-개발-환경)
5. [프로덕션 배포](#프로덕션-배포)
6. [다른 컴퓨터로 이미지 전송](#다른-컴퓨터로-이미지-전송)
7. [배포 시나리오](#배포-시나리오)
8. [보안 고려사항](#보안-고려사항)
9. [모니터링 및 로그](#모니터링-및-로그)
10. [트러블슈팅](#트러블슈팅)

---

## 개요

이 프로젝트는 다음과 같이 구성되어 있습니다:
- **Frontend**: React + Vite (Nginx로 서빙)
- **Backend**: Spring Boot (Java 21)
- **Database**: PostgreSQL 16

Docker Compose를 사용하여 개발 환경과 프로덕션 환경 모두를 지원합니다.

---

## 사전 요구사항

### 필수 설치
- Docker 20.10 이상
- Docker Compose V2 이상

### 확인 방법

```bash
# Docker 버전 확인
docker --version

# Docker Compose 버전 확인
docker compose version
```

### 권장 사양 (프로덕션)
- CPU: 2 코어 이상
- RAM: 4GB 이상
- 디스크: 20GB 이상 여유 공간

---

## 환경 설정

### 1. 환경 변수 파일 생성

프로덕션 배포를 위해 환경 변수 파일을 생성합니다:

```bash
# env.example을 복사하여 .env 파일 생성
cp env.example .env
```

### 2. .env 파일 수정

`.env` 파일을 열어 다음 항목들을 실제 환경에 맞게 수정합니다:

```env
# 데이터베이스 설정
DB_USER=appuser
DB_PASSWORD=강력한-비밀번호-입력  # ⚠️ 반드시 변경 필요
DB_NAME=appdb
DB_PORT=5432

# 백엔드 설정
BACKEND_PORT=8080
SPRING_PROFILE=prod
SPRING_DDL_AUTO=update  # 첫 배포: update (테이블 자동 생성), 이후 운영: validate (스키마 검증만)
SPRING_SHOW_SQL=false

# 프론트엔드 설정
FRONTEND_PORT=80
API_URL=http://your-server-ip:8080  # 또는 https://api.yourdomain.com

# CORS 설정 (프론트엔드 도메인, 쉼표로 구분)
ALLOWED_ORIGINS=http://localhost:80,https://yourdomain.com,https://www.yourdomain.com
```

**중요한 보안 주의사항:**
- `DB_PASSWORD`는 반드시 강력한 비밀번호로 변경하세요
- `.env` 파일은 절대 버전 관리에 포함하지 마세요 (`.gitignore` 확인)
- 프로덕션 환경에서는 환경 변수를 직접 관리하는 것을 권장합니다

---

## 로컬 개발 환경

로컬 개발을 위해서는 기본 `compose.yaml`을 사용합니다:

```bash
# 개발 환경 시작
docker compose up -d

# 로그 확인
docker compose logs -f

# 개발 환경 중지
docker compose down
```

개발 환경에서는:
- 데이터베이스 포트: `localhost:5432`
- 백엔드 포트: `localhost:8080`
- 프론트엔드 포트: `localhost:5173`

---

## 프로덕션 배포

### 빠른 배포 (권장)

배포 스크립트를 사용하여 간편하게 배포할 수 있습니다:

```bash
# 프로덕션 배포
./deploy.sh prod

# 개발 환경 시작
./deploy.sh dev
```

### 수동 배포

#### 1. 빌드 및 배포

```bash
# 프로덕션 환경으로 빌드 및 시작
docker compose -f docker-compose.prod.yaml --env-file .env up -d --build

# 빌드 없이 시작 (이미 빌드된 이미지 사용)
docker compose -f docker-compose.prod.yaml --env-file .env up -d

# 로그 확인
docker compose -f docker-compose.prod.yaml logs -f

# 특정 서비스 로그만 확인
docker compose -f docker-compose.prod.yaml logs -f backend
```

### 2. 서비스 상태 확인

```bash
# 실행 중인 컨테이너 확인
docker compose -f docker-compose.prod.yaml ps

# 서비스 상태 확인 (헬스체크 포함)
docker compose -f docker-compose.prod.yaml ps --format json
```

### 3. 서비스 중지 및 제거

```bash
# 서비스 중지 (컨테이너만 중지, 볼륨은 유지)
docker compose -f docker-compose.prod.yaml stop

# 서비스 중지 및 컨테이너 제거 (볼륨은 유지)
docker compose -f docker-compose.prod.yaml down

# 서비스 중지 및 모든 리소스 제거 (볼륨 포함 - ⚠️ 데이터 삭제됨)
docker compose -f docker-compose.prod.yaml down -v
```

### 4. 재시작 및 업데이트

```bash
# 애플리케이션 재시작
docker compose -f docker-compose.prod.yaml restart

# 특정 서비스만 재시작
docker compose -f docker-compose.prod.yaml restart backend

# 새로운 코드로 업데이트
docker compose -f docker-compose.prod.yaml up -d --build
```

---

## 다른 컴퓨터로 이미지 전송

빌드된 Docker 이미지를 다른 컴퓨터에서 사용하는 방법은 두 가지가 있습니다:

### 방법 비교

| 방법 | 스크립트 | 사용 시나리오 | 장점 | 단점 |
|------|---------|--------------|------|------|
| **Docker Hub 레지스트리** | `push-images.sh`<br>`pull-images.sh` | 인터넷 연결 가능, 여러 컴퓨터에서 사용 | 간편, 버전 관리 용이, 공유 쉬움 | Docker Hub 계정 필요, 인터넷 필수 |
| **파일 전송** | `export-images.sh`<br>`import-images.sh` | 인터넷 없음, 오프라인 환경, 보안 중요 | 인터넷 불필요, 완전한 오프라인 가능 | 파일 크기가 큼, 수동 전송 필요 |

### 방법 1: Docker Hub 레지스트리 사용 (권장)

인터넷이 연결된 환경에서 가장 간편한 방법입니다.

#### 1-1. 이미지 빌드 및 푸시 (개발 컴퓨터)

```bash
# Docker Hub에 이미지 푸시
./push-images.sh [dockerhub-username] [tag]

# 예시
./push-images.sh myusername v1.0.0
# 또는 기본 태그(latest) 사용
./push-images.sh myusername
```

**동작 과정:**
1. 로컬에서 이미지 빌드
2. Docker Hub 사용자명으로 태그 지정
3. Docker Hub에 로그인
4. 이미지를 Docker Hub에 업로드

**필요 사항:**
- Docker Hub 계정 (https://hub.docker.com 에서 무료 가입)
- Docker Hub 로그인 권한

#### 1-2. 다른 컴퓨터에서 이미지 받기

```bash
# Docker Hub에서 이미지 다운로드
./pull-images.sh [dockerhub-username] [tag]

# 예시
./pull-images.sh myusername v1.0.0
```

**동작 과정:**
1. Docker Hub에서 이미지 다운로드
2. 로컬 태그로 변경 (docker-compose에서 사용 가능하도록)

#### 1-3. 이미지 기반으로 배포

이미지를 받은 후에는 소스 코드 없이도 배포할 수 있습니다:

```bash
# 환경 변수 설정 (필수)
cp env.example .env
# .env 파일 편집

# 이미지 기반 docker-compose로 실행 (빌드 없이)
docker compose -f docker-compose.prod.images.yaml --env-file .env up -d
```

### 방법 2: 파일로 전송 (오프라인 환경)

인터넷이 없는 환경이나 보안이 중요한 경우 사용합니다.

#### 2-1. 이미지 export (개발 컴퓨터)

```bash
# 이미지를 tar 파일로 저장
./export-images.sh
```

**생성되는 파일:**
- `docker-images/backend-YYYYMMDD_HHMMSS.tar.gz`
- `docker-images/frontend-YYYYMMDD_HHMMSS.tar.gz`

**전송 방법:**
```bash
# SCP로 전송
scp docker-images/*.tar.gz user@remote-server:/path/to/destination/

# 또는 USB, 외장 하드 등으로 복사
```

#### 2-2. 이미지 import (다른 컴퓨터)

```bash
# tar 파일에서 이미지 로드
./import-images.sh

# 또는 파일 경로 직접 지정
./import-images.sh docker-images/backend-20241101.tar.gz docker-images/frontend-20241101.tar.gz
```

**동작 과정:**
1. tar 파일 압축 해제
2. Docker 이미지로 로드
3. 임시 파일 정리

#### 2-3. 이미지 기반으로 배포

```bash
# 환경 변수 설정 (필수)
cp env.example .env
# .env 파일 편집

# 이미지 기반 docker-compose로 실행
docker compose -f docker-compose.prod.images.yaml --env-file .env up -d
```

### 스크립트별 상세 설명

#### `push-images.sh` - Docker Hub에 업로드
- **용도**: 빌드된 이미지를 Docker Hub에 업로드
- **입력**: Docker Hub 사용자명, 태그(선택)
- **출력**: Docker Hub에 업로드된 이미지
- **필요**: Docker Hub 계정

#### `pull-images.sh` - Docker Hub에서 다운로드
- **용도**: Docker Hub에서 이미지를 다운로드
- **입력**: Docker Hub 사용자명, 태그(선택)
- **출력**: 로컬에 다운로드된 이미지
- **필요**: 인터넷 연결

#### `export-images.sh` - 이미지를 파일로 저장
- **용도**: 로컬 이미지를 tar.gz 파일로 저장
- **입력**: 없음 (자동으로 빌드된 이미지 사용)
- **출력**: `docker-images/` 폴더에 압축된 tar 파일
- **필요**: 로컬에 이미 빌드된 이미지

#### `import-images.sh` - 파일에서 이미지 로드
- **용도**: tar 파일에서 Docker 이미지로 로드
- **입력**: tar 파일 경로 (자동 감지 가능)
- **출력**: 로컬에 로드된 이미지
- **필요**: tar 파일

### 전체 워크플로우 예시

#### 시나리오 A: Docker Hub 사용 (인터넷 연결)

**개발 컴퓨터:**
```bash
# 1. 이미지 빌드 및 푸시
./push-images.sh myusername v1.0.0
```

**프로덕션 서버:**
```bash
# 1. 프로젝트 클론 (docker-compose 파일만 필요)
git clone <repository-url>
cd docker-fullstack

# 2. 환경 변수 설정
cp env.example .env
# .env 편집

# 3. 이미지 다운로드
./pull-images.sh myusername v1.0.0

# 4. 배포 (빌드 없이)
docker compose -f docker-compose.prod.images.yaml --env-file .env up -d
```

#### 시나리오 B: 파일 전송 (오프라인)

**개발 컴퓨터:**
```bash
# 1. 이미지 export
./export-images.sh

# 2. 파일 전송 (SCP, USB 등)
scp docker-images/*.tar.gz user@production:/tmp/
```

**프로덕션 서버:**
```bash
# 1. 프로젝트 클론
git clone <repository-url>
cd docker-fullstack

# 2. 이미지 파일 복사
mkdir -p docker-images
cp /tmp/*.tar.gz docker-images/

# 3. 이미지 import
./import-images.sh

# 4. 환경 변수 설정
cp env.example .env
# .env 편집

# 5. 배포 (빌드 없이)
docker compose -f docker-compose.prod.images.yaml --env-file .env up -d
```

### 주의사항

1. **환경 변수 설정**: 다른 컴퓨터에서도 반드시 `.env` 파일을 생성하고 설정해야 합니다.
2. **네트워크 설정**: `API_URL`과 `ALLOWED_ORIGINS`를 새로운 서버 환경에 맞게 수정해야 합니다.
3. **데이터베이스**: 데이터베이스는 이미지가 아니라 볼륨에 저장되므로, 데이터 마이그레이션이 별도로 필요할 수 있습니다.
4. **이미지 크기**: export된 파일은 보통 수백 MB ~ 수 GB 정도입니다. 전송 시간을 고려하세요.

---

## 배포 시나리오

### 시나리오 1: 단일 서버 배포

가장 간단한 배포 방법입니다. 모든 서비스가 한 서버에서 실행됩니다.

**단계:**

1. 서버에 프로젝트 업로드
   ```bash
   # Git을 사용하는 경우
   git clone <repository-url>
   cd docker-fullstack
   
   # 또는 SCP/FTP로 파일 업로드
   ```

2. 환경 변수 설정
   ```bash
   cp env.example .env
   # .env 파일 수정
   ```

3. 배포 실행
   ```bash
   docker compose -f docker-compose.prod.yaml --env-file .env up -d --build
   ```

4. 방화벽 설정 (필요한 경우)
   ```bash
   # Ubuntu/Debian 예시
   sudo ufw allow 80/tcp
   sudo ufw allow 8080/tcp
   ```

### 시나리오 2: Nginx 리버스 프록시와 함께 배포

프로덕션 환경에서는 Nginx를 리버스 프록시로 사용하는 것을 권장합니다.

**Nginx 설정 예시 (`/etc/nginx/sites-available/app`):**

```nginx
# 프론트엔드 프록시
server {
    listen 80;
    server_name yourdomain.com;

    location / {
        proxy_pass http://localhost:80;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}

# 백엔드 API 프록시
server {
    listen 80;
    server_name api.yourdomain.com;

    location / {
        proxy_pass http://localhost:8080;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
```

**SSL/TLS 설정 (Let's Encrypt 사용):**

```bash
# Certbot 설치
sudo apt-get update
sudo apt-get install certbot python3-certbot-nginx

# SSL 인증서 발급 및 자동 설정
sudo certbot --nginx -d yourdomain.com -d api.yourdomain.com
```

`.env` 파일에서 `ALLOWED_ORIGINS`와 `API_URL`을 HTTPS 도메인으로 업데이트:

```env
API_URL=https://api.yourdomain.com
ALLOWED_ORIGINS=https://yourdomain.com,https://www.yourdomain.com
```

### 시나리오 3: 클라우드 배포 (AWS, Azure, GCP 등)

#### AWS EC2 배포

1. EC2 인스턴스 생성 및 Docker 설치
   ```bash
   # EC2 인스턴스에 접속 후
   sudo yum update -y
   sudo yum install docker -y
   sudo systemctl start docker
   sudo systemctl enable docker
   
   # Docker Compose 설치
   sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
   sudo chmod +x /usr/local/bin/docker-compose
   ```

2. 보안 그룹 설정
   - HTTP (80), HTTPS (443) 포트 열기
   - 필요시 백엔드 포트 (8080) 열기

3. 프로젝트 배포
   ```bash
   git clone <repository-url>
   cd docker-fullstack
   cp env.example .env
   # .env 파일 수정
   docker compose -f docker-compose.prod.yaml --env-file .env up -d --build
   ```

#### Docker Swarm 또는 Kubernetes

더 복잡한 배포가 필요한 경우:
- **Docker Swarm**: 여러 서버에 걸쳐 컨테이너 오케스트레이션
- **Kubernetes**: 더 복잡한 컨테이너 오케스트레이션 필요 시

이 경우 별도의 설정 파일이 필요합니다.

---

## 보안 고려사항

### 1. 데이터베이스 보안

- ✅ 강력한 비밀번호 사용
- ✅ 데이터베이스 포트를 외부에 직접 노출하지 않기 (Docker 네트워크 내부 통신만 사용)
- ✅ 정기적인 백업 수행

### 2. 환경 변수 보안

```bash
# .env 파일 권한 설정
chmod 600 .env

# 프로덕션에서는 환경 변수를 직접 전달 (파일 사용 안 함)
docker compose -f docker-compose.prod.yaml \
  -e DB_PASSWORD=secure-password \
  -e ALLOWED_ORIGINS=https://yourdomain.com \
  up -d
```

### 3. 네트워크 보안

- 내부 통신은 Docker 네트워크 사용
- 외부 노출 포트 최소화
- 방화벽 설정으로 불필요한 포트 차단

### 4. 이미지 보안

- 정기적으로 베이스 이미지 업데이트
- 불필요한 패키지 설치 최소화
- 멀티 스테이지 빌드 사용 (현재 적용됨)

---

## 모니터링 및 로그

### 로그 확인

```bash
# 모든 서비스 로그
docker compose -f docker-compose.prod.yaml logs -f

# 특정 서비스 로그
docker compose -f docker-compose.prod.yaml logs -f backend
docker compose -f docker-compose.prod.yaml logs -f frontend
docker compose -f docker-compose.prod.yaml logs -f db

# 최근 100줄만 보기
docker compose -f docker-compose.prod.yaml logs --tail=100

# 특정 시간 이후 로그
docker compose -f docker-compose.prod.yaml logs --since 2024-01-01T00:00:00
```

### 리소스 모니터링

```bash
# 컨테이너 리소스 사용량 확인
docker stats

# 특정 컨테이너만 확인
docker stats springboot-prod react-prod pg-prod
```

### 헬스 체크 확인

```bash
# 서비스 헬스 상태 확인
docker compose -f docker-compose.prod.yaml ps

# 백엔드 헬스 체크 직접 확인
curl http://localhost:8080/hello

# 데이터베이스 연결 확인
docker compose -f docker-compose.prod.yaml exec db pg_isready -U appuser
```

---

## 트러블슈팅

### 문제 1: 컨테이너가 시작되지 않음

**해결 방법:**
```bash
# 로그 확인
docker compose -f docker-compose.prod.yaml logs

# 컨테이너 상태 확인
docker compose -f docker-compose.prod.yaml ps -a

# 컨테이너 재생성
docker compose -f docker-compose.prod.yaml up -d --force-recreate
```

### 문제 2: 데이터베이스 연결 실패

**원인:**
- 데이터베이스가 아직 준비되지 않음
- 잘못된 연결 정보

**해결 방법:**
```bash
# 데이터베이스 상태 확인
docker compose -f docker-compose.prod.yaml exec db pg_isready -U appuser

# 환경 변수 확인
docker compose -f docker-compose.prod.yaml config

# 백엔드 재시작
docker compose -f docker-compose.prod.yaml restart backend
```

### 문제 3: CORS 에러

**원인:**
- `ALLOWED_ORIGINS` 환경 변수가 프론트엔드 도메인과 일치하지 않음

**해결 방법:**
```bash
# .env 파일에서 ALLOWED_ORIGINS 확인 및 수정
# 예: ALLOWED_ORIGINS=https://yourdomain.com

# 백엔드 재시작
docker compose -f docker-compose.prod.yaml restart backend
```

### 문제 4: 포트 충돌

**원인:**
- 다른 애플리케이션이 같은 포트 사용

**해결 방법:**
```bash
# 포트 사용 확인
sudo netstat -tulpn | grep :80
sudo netstat -tulpn | grep :8080

# .env 파일에서 포트 변경
# FRONTEND_PORT=8080
# BACKEND_PORT=8081
```

### 문제 5: 빌드 실패

**해결 방법:**
```bash
# 캐시 없이 다시 빌드
docker compose -f docker-compose.prod.yaml build --no-cache

# 특정 서비스만 재빌드
docker compose -f docker-compose.prod.yaml build --no-cache backend
```

### 문제 6: 디스크 공간 부족

**해결 방법:**
```bash
# 사용하지 않는 이미지 제거
docker image prune -a

# 사용하지 않는 볼륨 제거 (⚠️ 데이터 삭제됨)
docker volume prune

# 사용하지 않는 컨테이너 제거
docker container prune
```

---

## 백업 및 복구

### 데이터베이스 백업

```bash
# 백업 생성
docker compose -f docker-compose.prod.yaml exec db pg_dump -U appuser appdb > backup_$(date +%Y%m%d_%H%M%S).sql

# 압축 백업
docker compose -f docker-compose.prod.yaml exec db pg_dump -U appuser appdb | gzip > backup_$(date +%Y%m%d_%H%M%S).sql.gz
```

### 데이터베이스 복구

```bash
# 백업 파일로 복구
docker compose -f docker-compose.prod.yaml exec -T db psql -U appuser appdb < backup_20240101_120000.sql

# 압축 백업 복구
gunzip < backup_20240101_120000.sql.gz | docker compose -f docker-compose.prod.yaml exec -T db psql -U appuser appdb
```

---

## 추가 리소스

- [Docker 공식 문서](https://docs.docker.com/)
- [Docker Compose 문서](https://docs.docker.com/compose/)
- [Spring Boot 프로덕션 가이드](https://docs.spring.io/spring-boot/docs/current/reference/html/deployment.html)
- [PostgreSQL 관리 가이드](https://www.postgresql.org/docs/)

---

## 질문 및 지원

문제가 발생하거나 도움이 필요한 경우:
1. 로그를 확인하세요
2. 이 문서의 트러블슈팅 섹션을 참고하세요
3. 환경 설정을 다시 확인하세요

