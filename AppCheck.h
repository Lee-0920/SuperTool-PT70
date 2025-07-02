
#ifndef APPCHECK_H
#define APPCHECK_H

static char AppCheck(const char * cmd)
{
    FILE *pp = popen(cmd, "r"); //建立管道

    if (!pp)
    {
        qDebug("Failed to obtain status");
    }
    char tmp[256];
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        if (tmp[strlen(tmp) - 1] == '\n')
        {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }
    }
    pclose(pp); //关闭管道

    return tmp[0];
}

#endif

