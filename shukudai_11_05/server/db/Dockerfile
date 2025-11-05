# 1️⃣ 베이스 이미지
FROM postgres:16

# 2️⃣ 환경변수 설정
ENV POSTGRES_USER=myuser
ENV POSTGRES_PASSWORD=mypass
ENV POSTGRES_DB=shukudai_11_06

# 3️⃣ 컨테이너 시작 시 실행될 초기 SQL 복사
COPY init.sql /docker-entrypoint-initdb.d/