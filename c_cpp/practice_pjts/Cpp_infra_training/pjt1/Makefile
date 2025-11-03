# ---------------------------
# 📄 Makefile : C / C++ 자동 빌드 스크립트
# ---------------------------

# 최종적으로 만들어질 실행 파일 이름
TARGET := sysmon

# main.c, sysinfo.c 가 있으면 C 모드로 빌드
C_SRCS   := $(wildcard main.c sysinfo.c)
# main.cpp, sysinfo.cpp 가 있으면 C++ 모드로 빌드
CPP_SRCS := $(wildcard main.cpp sysinfo.cpp)

# C 소스가 비어있다면 (즉 main.c/sysinfo.c가 없으면)
ifeq ($(strip $(C_SRCS)),)
  # C++ 소스도 없다면 에러 표시
  ifeq ($(strip $(CPP_SRCS)),)
    $(error ❌ No valid sources found (need main.c+sysinfo.c or main.cpp+sysinfo.cpp))
  else
    # C++ 소스가 있으면 C++ 모드로 설정
    LANG := c++
  endif
else
  # C 소스가 있으면 C 모드로 설정
  LANG := c
endif

# C와 C++ 공통 컴파일 옵션 설정
# -Wall : 모든 경고 표시
# -O2   : 최적화 옵션
CFLAGS   ?= -Wall -O2
CXXFLAGS ?= -Wall -O2

# 항상 실행되는(파일이름이 아님) 규칙 지정
.PHONY: all clean

# make 명령어 실행 시 기본 타깃
# ⇒ $(TARGET) = sysmon 을 만든다
all: $(TARGET)

# ---------------------------
# 🧩 C 모드 (main.c, sysinfo.c 있을 때)
# ---------------------------
ifeq ($(LANG),c)
# 컴파일러를 gcc로 지정
CC   := gcc
# 소스 목록
SRCS := $(C_SRCS)
# .c → .o 변환 규칙
OBJS := $(SRCS:.c=.o)

# sysmon 실행파일 생성 규칙
# $@ : 타깃 이름(sysmon)
# $^ : 의존 파일들(main.o sysinfo.o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 개별 .c 파일을 .o로 만드는 규칙
# $< : 첫 번째 의존 파일(즉, 현재 .c)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
endif

# ---------------------------
# 🧩 C++ 모드 (main.cpp, sysinfo.cpp 있을 때)
# ---------------------------
ifeq ($(LANG),c++)
# 컴파일러를 g++로 지정
CXX  := g++
# 소스 목록
SRCS := $(CPP_SRCS)
# .cpp → .o 변환 규칙
OBJS := $(SRCS:.cpp=.o)

# sysmon 실행파일 생성 규칙 (C++ 전용)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 개별 .cpp 파일을 .o로 만드는 규칙
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
endif

# ---------------------------
# 🧹 정리(clean) 규칙
# ---------------------------
# make clean 시 .o 파일과 실행파일 삭제
clean:
	rm -f $(OBJS) $(TARGET)
