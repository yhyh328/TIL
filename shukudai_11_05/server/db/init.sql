DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM pg_roles WHERE rolname = 'appuser'
    ) THEN
        CREATE ROLE appuser LOGIN PASSWORD 'password';
    END IF;
END
$$;


DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM pg_database WHERE datname = 'shukudai_11_06'
    ) THEN
        PERFORM dblink_exec('dbname=' || current_database(),
                            'CREATE DATABASE shukudai_11_06');
    END IF;
END
$$ LANGUAGE plpgsql;


\connect shukudai_11_06

ALTER DATABASE shukudai_11_06 OWNER TO appuser;
ALTER SCHEMA public OWNER TO appuser;

GRANT ALL PRIVILEGES ON DATABASE shukudai_11_06 TO appuser;
GRANT ALL PRIVILEGES ON SCHEMA public TO appuser;

ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON TABLES    TO appuser;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON SEQUENCES TO appuser;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON FUNCTIONS TO appuser;

DROP TABLE IF EXISTS articles;
DROP TABLE IF EXISTS users;

CREATE TABLE IF NOT EXISTS users (
    user_id VARCHAR(10) PRIMARY KEY,
    user_pw VARCHAR(20) NOT NULL
);

CREATE TABLE IF NOT EXISTS articles (
    article_id SERIAL PRIMARY KEY,
    article_title VARCHAR(50) NOT NULL,
    article_content TEXT NOT NULL,
    article_author VARCHAR(10) REFERENCES users(user_id),
    article_is_deleted BOOLEAN DEFAULT false
);