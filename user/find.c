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
	// ����ж��ļ����Ͳ���Ŀ¼ 
    if (st.type == T_FILE|| st.type == T_DEVICE) {
        fprintf(2, "find: %s is not a dir\n", dir);
        close(fd);
        return;
    }

    // ��Ŀ¼��·������洢���� 
    memmove(buf, dir, strlen(dir));
    p = buf + strlen(dir);
    *p++ = '/'; // ��ĩβ��Ӽ���� 
   
    // ������ǰĿ¼�������ļ�
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)    continue; //����Ŀ¼��û��ʹ�� 
        if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)    continue;

        // ����ǰ�������ļ���ӵ��洢���� 
        memmove(p, de.name, strlen(de.name));
        p[strlen(de.name)] = '\0';

        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR) {// ���������Ŀ¼���ظ�find���� 
            find(buf, path);
            continue;
        }
        if (strcmp(path, de.name) == 0) {// ����������ļ����ж��Ƿ�����Ҫ���ҵ��ļ� 
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
