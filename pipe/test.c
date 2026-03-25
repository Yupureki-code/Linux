#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pipe(fd);

    if (fork() == 0) 
    { // 子进程:向管道内写入数据
        close(fd[0]);// 关闭读端
        char buffer[] = "hello world";
        write(fd[1],buffer,sizeof(buffer));
        return 1;
    }
    else
    {// 父进程:从管道捏读取数据
        sleep(1);//等待子进程写入完毕
        close(fd[1]);//关闭写端
        char buffer[1024];
        read(fd[0],buffer,sizeof(buffer) - 1);
        printf("child say:%s\n",buffer);
    }
    waitpid(-1,NULL,0);
    return 0;
}