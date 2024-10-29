#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "AquesTalk2.h"

void * file_load(const char * file, int * psize);

// メイン関数
int main(int argc, char **argv){
    int size;
    char message[1024];

    // 標準入力からテキストを読み込む
    if(fgets(message, 1024-1, stdin) == 0) return 0;

    // 改行文字を削除
    message[strcspn(message, "\n")] = '\0';

    // 音響モデルファイルを読み込む
    void *pPhont = file_load("aq_yukkuri.phont", &size);
    if(pPhont == 0) return -2;

    // 音声データを生成
    unsigned char *wav = AquesTalk2_Synthe_Utf8(message, 100, &size, pPhont);
    if(wav == 0) {
        fprintf(stderr, "ERR %d", size);
        return -1;
    }

    // 音響モデルデータを解放
    free(pPhont);

    // 音声データを標準出力に出力
    fwrite(wav, 1, size, stdout);

    // 音声データを解放
    AquesTalk2_FreeWave(wav);
    return 0;
}

// ファイルを読み込む関数
void * file_load(const char * file, int * psize){
    FILE *fp;
    char *data;
    struct stat st;
    *psize = 0;

    if(stat(file, &st) != 0) return NULL;

    if((data = (char *)malloc(st.st_size)) == NULL){
        fprintf(stderr, "メモリを確保できません (file_load)\n");
        return NULL;
    }

    if((fp = fopen(file, "rb")) == NULL) {
        free(data);
        perror(file);
        return NULL;
    }

    if(fread(data, 1, st.st_size, fp) < (unsigned)st.st_size) {
        fprintf(stderr, "データを読み込めません (file_load)\n");
        free(data);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    *psize = st.st_size;
    return data;
}