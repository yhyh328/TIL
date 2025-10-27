#include <stdio.h>

void main()
{
    FILE *fp;                    // 파일 포인터
    char filename[] = "bintest.dat";  // 바이너리 파일명
    int buf_w[10], buf_r[10];    // 쓰기용 배열, 읽기용 배열 (현재는 읽기용 미사용)
    int i;

    // 배열에 10, 20, 30, ..., 100 값 저장
    for (i = 0; i < 10; i++) { buf_w[i] = (i + 1) * 10; }
    
    // 바이너리 쓰기 모드로 파일 열기 (실패시 프로그램 종료)
    if (!(fp = fopen(filename, "wb"))) { return; }
    
    // 배열 데이터를 바이너리 형태로 파일에 쓰기 (10개 int 데이터)
    // fwrite가 10을 반환하지 않으면 쓰기 실패
    if (fwrite(buf_w, sizeof(int), 10, fp) != 10) { fclose(fp); return; }
    // for문과 같은 기능
    // for (i = 0; i < 10; i++) {
    //     fwrite(&buf_w[i], sizeof(int), 1, fp);  // 1개씩 10번
    // }
    
    fclose(fp);  // 파일 닫기

    if (!(fp = fopen(filename, "rb"))) return;
    if (fread(buf_r, sizeof(int), 10, fp) != 10) { fclose(fp); return; }
    fclose(fp);

    for (i = 0; i < 10; i++) printf("%d ", buf_r[i]);
}