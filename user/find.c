#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void
find(char *dir, char *path)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }
	// 如果判断文件类型不是目录 
    if (st.type == T_FILE|| st.type == T_DEVICE) {
        fprintf(2, "find: %s is not a dir\n", dir);
        close(fd);
        return;
    }

    // 将目录的路径放入存储当中 
    memmove(buf, dir, strlen(dir));
    p = buf + strlen(dir);
    *p++ = '/'; // 在末尾添加间隔符 
   
    // 遍历当前目录下所有文件
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)    continue; //当该目录项没有使用 
        if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)    continue;

        // 将当前读出的文件添加到存储当中 
        memmove(p, de.name, strlen(de.name));
        p[strlen(de.name)] = '\0';

        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR) {// 如果读出是目录，重复find函数 
            find(buf, path);
            continue;
        }
        if (strcmp(path, de.name) == 0) {// 如果读出是文件，判断是否是需要查找的文件 
            printf("%s\n", buf);
        }
    }
    close(fd);
}

int 
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("argc is %d and it is less then 2\n", argc);
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
