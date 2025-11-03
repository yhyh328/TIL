-- (선택) appuser가 없을 수 있는 환경도 고려
DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_roles WHERE rolname = 'appuser') THEN
        CREATE ROLE appuser LOGIN PASSWORD 'password';
    END IF;
END
$$;

-- 이미 POSTGRES_DB=appdb 로 생성되어 있으니 '접속만' 바꿉니다
\connect appdb

-- 소유자 및 권한 정리 (DB/스키마/디폴트 권한까지)
ALTER DATABASE appdb OWNER TO appuser;
ALTER SCHEMA public OWNER TO appuser;

GRANT ALL PRIVILEGES ON DATABASE appdb TO appuser;
GRANT ALL PRIVILEGES ON SCHEMA public TO appuser;

-- 앞으로 생성될 객체에 대한 기본 권한
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON TABLES    TO appuser;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON SEQUENCES TO appuser;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON FUNCTIONS TO appuser;