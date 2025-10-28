#include <stdio.h>
#include <string.h>

typedef struct _CALORIE {
    char name[40];
    float value;
} CALORIE;

int calregist(CALORIE*, int);
float calcalc(CALORIE*, int);

int main() {
    CALORIE cal[500] = {
        {"米食", 150.0},
        {"中華麵", 57.1},
        {"そば", 133.3},
        {"うどん", 100.0},
        {"素麵", 133.3},
        {"食パン", 250.0},
    };
    int cal_num = 6;
    int mode = 0;

    printf("カロリー計算ツール\n");
    while(1) {
        printf("登録は１を、計算は２を、終了は0を入力してください：");
        scanf("%d", &mode);
        if(mode == 0) break;
        else if(mode == 1) cal_num = calregist(cal, cal_num);
        else if(mode == 2) printf("総カロリー：%6.2fkcal\n\n", calcalc(cal, cal_num));
    }
    return 0;
}

/*************************************
calregist() カロリーリストへ登録する
[引数]  pcal -- カロリーリストへのポインター
    　　num -- 登録前のリストの要素数
[戻り値] 登録後のリストの要素数
*************************************/
int calregist(CALORIE *pcal, int num) {
    printf("食品名を入力してください：　");
    scanf("%s", (pcal + num) -> name);
    printf("その食品のカロリーを入力してください[kcal/100g]：　");
    scanf("%f", &((pcal + num) -> value));
    printf("登録しました。\n\n");
    return num + 1;
}

/*************************************
calcalc() カロリーを計算する
[引数]  pcal -- カロリーリストへのポインター
    　　num -- 登録前のリストの要素数
[戻り値] カロリー数
*************************************/
float calcalc(CALORIE *pcal, int num) {
    char name[40];
    float gram;
    float totalcal = 0.0;
    int i;

    printf("--食品名一覧--------------------\n");
    for (i = 0; i < num; i++) printf("%s\t", (pcal + i) -> name);
    printf("\n-----------------------------\n");
    while(1) {
        printf("食品名(endで計算)　：　");
        scanf("%s", name);
        if(strcmp(name, "end") == 0) break;
        printf("グラム数：");
        scanf("%f", &gram);
        for(i = 0; i < num; i++) {
            if(strcmp(name, (pcal + i) -> name) == 0) {
                totalcal += (pcal + i) -> value * gram / 100.0;
                break;
            }
        }
    }
    return totalcal;
}