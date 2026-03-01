#include "processbar.h"
#include <string.h>

#define NUM 101//表示进度条
#define STYLE '='//进度条填充

// verison2
void FlushProcess(double total, double current)//一共要下载多少数据，现在下载了多少
{
    char buffer[NUM];
    memset(buffer, 0, sizeof(buffer));
    const char *lable="|/-\\";
    int len = strlen(lable);
    static int cnt = 0;
    // 不需要自己循环,填充#
    int num = (int)(current*100/total); // 11.0 / 1000
    int i = 0;
    for(; i < num; i++)
    {
        buffer[i] = STYLE;
    }
    double rate = current/total;
    cnt %= len;
    printf("[%-100s][%.1f%%][%c]\r", buffer, rate*100, lable[cnt]);
    cnt++;
    fflush(stdout);
}