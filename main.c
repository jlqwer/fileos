#include "config.h"
int main()
{
    init();
    if(su("root"))
    {
        showmenu();
        while(getcmd())
        {
            if(cutcmd())    //分割命令和参数
            {
                if(strcmp(acmd.cmd[0],"poweroff")==0)
                {
                    if(isadmin(cmdusername)=='#')
                    {
                        break;
                    }
                    else
                    {
                        printf("您无权关闭计算机！\n");
                        continue;
                    }
                }
                if(exec()<0)
                {
                    printf("System error!\n");
                }
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
