#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int argv_len = 0;// ��������
    char buf[128] = {'\0'}; // �洢�û������ַ���
    char *new_argv[MAXARG];

    // �����������Ƿ񳬹�������
    if (argc > MAXARG)
    {
        printf("Too many arguments\n");
        exit(1);
    }

    // ��argv�����ݿ�����new_argv
    for (int i = 1; i < argc; i++)
    {
        new_argv[i - 1] = argv[i];
    }

    // ѭ����ȡ�û�������
    while (gets(buf, sizeof(buf)))
    {
        int buf_len = strlen(buf);  // ��ȡ��ȡ�����ַ����ĳ���
        if (buf_len < 1)
            break;
        argv_len = argc - 1;
        buf[buf_len - 1] = '\0'; // ����ȡ�����ַ����еĻ��з��滻Ϊ�ַ���������

        // ��buf�ж�ȡ���û���������ݰ���word��ֵ�ÿ��new_argv��
        for (char *p = buf; *p; p++)
        {
            // ���������Ŀո��ַ���pָ���һ���ǿո��ַ�
            while (*p && (*p == ' '))
            {
                *p++ = '\0';
            }
            // �²���
            if (*p)
            {
                new_argv[argv_len++] = p;
            }
            // ������ǰ����ʣ���ַ���pָ����һ���ո��ַ����ַ���������
            while (*p && (*p != ' '))
            {
                p++;
            }
        }

        new_argv[argv_len] = "\0";

        // �����̵ȴ��ӽ��̽���
        if (fork() > 0)
        {
            wait(0);
        }
        else
        {
            // �޸�exec�����ĵ��÷�ʽ
            exec(new_argv[0], new_argv);
        }
    }

    exit(0);
}
