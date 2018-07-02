#include "config.h"
long long int curtime()
{
    time_t t;
    return time(&t);
}
int check_rPermission(struct filetree *curfile)
{
    if(uid==0 || uid==curfile->user_id)
    {
        return 1;
    }
    else
    {
        if(curfile->authority[2]<4)//先写读权限
        {
            printf("Permission denied!\n");
            return 0;
        }
        return 1;
    }
}

void init()
{
    strcpy(user[0].name,"root");
    strcpy(user[0].passowrd,PASSWORD);
    user[0].status=1;
    usernum=1;
    uid=0;
    disk=0;
    passworderrornum=0;
    //根目录
    rootdir=(struct filetree*)malloc(sizeof(struct filetree));
    strcpy(rootdir->name,"/");
    rootdir->authority[0]=7;
    rootdir->authority[1]=5;
    rootdir->authority[2]=1;
    rootdir->create_time=curtime();
    rootdir->filetype='d';
    rootdir->pre=NULL;
    rootdir->user_id=0;
    rootdir->directory=(struct directory*)malloc(sizeof(struct directory));
    rootdir->directory->childnum=0;
    rootdir->status=1;
    curdir=rootdir;
    //家目录
    int homeid=mkdir(rootdir,rootdir->directory,"home");
    home=rootdir->directory->child[homeid];
    int roothomeid=mkdir(rootdir,rootdir->directory,"root");
    user[0].home=rootdir->directory->child[roothomeid];
    //在这里加模拟linux的几个文件夹
    // bin
    int binid=mkdir(rootdir,rootdir->directory,"bin");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"ll");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"ls");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"su");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"passwd");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"cd");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"help");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"vi");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"cat");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"rm");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"rmdir");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"poweroff");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"chmod");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"cp");
    createfile(rootdir->directory->child[binid],rootdir->directory->child[binid]->directory,"mv");

    //sbin
    int sbinid=mkdir(rootdir,rootdir->directory,"sbin");
    createfile(rootdir->directory->child[sbinid],rootdir->directory->child[sbinid]->directory,"adduer");
    createfile(rootdir->directory->child[sbinid],rootdir->directory->child[sbinid]->directory,"deluser");
    createfile(rootdir->directory->child[sbinid],rootdir->directory->child[sbinid]->directory,"poweroff");
    //没用的
    mkdir(rootdir,rootdir->directory,"etc");
    mkdir(rootdir,rootdir->directory,"boot");
    mkdir(rootdir,rootdir->directory,"dev");
    mkdir(rootdir,rootdir->directory,"lib");
    mkdir(rootdir,rootdir->directory,"mnt");
    mkdir(rootdir,rootdir->directory,"usr");
    mkdir(rootdir,rootdir->directory,"var");
    mkdir(rootdir,rootdir->directory,"proc");
    mkdir(rootdir,rootdir->directory,"opt");
    mkdir(rootdir,rootdir->directory,"run");
    getcwd();
}
int hasuser(char *loginname)
{
    int i;
    for(i=0;i<=usernum;i++)
    {
        if(strcmp(loginname,user[i].name)==0 && user[i].status==1)
        {
            return i;
        }
    }
    return -1;
}
char* inputpassword(char *passwd)
{
    char ch;
    int pi=0;
    while(ch=getch())
    {
        if(ch==13 || ch=='\n')
        {
            passwd[pi]='\0';
            break;
        }
        else if(ch=='\b')
        {
            if(pi>0)
            {
                passwd[pi]='\0';
                pi--;
                printf("\b \b");
            }
        }
        else
        {
            passwd[pi]=ch;
            putchar('*');
            pi++;
        }
    }
    return passwd;
}


char isadmin(char * username)
{
    if((strcmp(username,"jlqwer")==0)||(strcmp(username,"root")==0))
    {
        return '#';
    }
    else
    {
        return '$';
    }
    return '$';
}
int getcmd()
{
    char ch;
    char commend[200];
    getcwd();
    printf("%s@%s:%s%c",cmdusername,HOSTNAME,cwdstr,isadmin(cmdusername));
    int pos=0;
    while((ch=getchar()))
    {
        if(ch=='\n')
        {
            commend[pos]='\0';
            strcpy(acmd.cmds,commend);
            break;
        }
        commend[pos]=ch;
        pos++;
    }
    return 1;
}
int cutcmd()
{
    int pos=0;
    acmd.cmdn=0;
    int cmdpos=0;
    if(acmd.cmds[0]=='\0')
    {
        return 0;
    }
    if(acmd.cmds[0]==' ')
    {
        acmd.cmdn--;
    }
    while(acmd.cmds[pos]!=EOF)
    {
        if(acmd.cmds[pos]=='\0')
        {
            acmd.cmd[acmd.cmdn][cmdpos]='\0';
            break;
        }
        if(acmd.cmds[pos]==' ')
        {
            if(acmd.cmds[pos+1]==' ')
            {
                pos++;
                continue;
            }
            if(acmd.cmds[pos+1]=='\n')
            {
                acmd.cmd[acmd.cmdn][cmdpos]='\0';
                pos++;
                continue;
            }
            acmd.cmd[acmd.cmdn][cmdpos]='\0';
            acmd.cmdn++;
            cmdpos=0;
        }
        else
        {
            acmd.cmd[acmd.cmdn][cmdpos]=acmd.cmds[pos];
            cmdpos++;
        }
        pos++;
    }
    if(acmd.cmdn<0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//cd多级目录
int cutpath()
{
    int pos=0;
    struct filetree *temp=curdir;
    cutpaths.pathn=0;
    int pathpos=0;
    if(cutpaths.paths[0]=='\0')
    {
        return 0;
    }
    else if(cutpaths.paths[0]=='~')
    {
        temp=user[uid].home;
        pos++;
        //printf("从家目录开始\n");
    }
    else if(cutpaths.paths[0]=='/')  //绝对路径
    {
        temp=rootdir;
        //printf("这是绝对路径！\n");
        pos++;
    }
    while(cutpaths.paths[pos]!=EOF)
    {
        if(cutpaths.paths[pos]=='\0')
        {
            cutpaths.path[cutpaths.pathn][pathpos]='\0';
            break;
        }
        if(cutpaths.paths[pos]=='/')
        {
            if(cutpaths.paths[pos+1]=='/')
            {
                pos++;
                continue;
            }

            if(cutpaths.paths[pos+1]=='\n')
            {
                cutpaths.path[cutpaths.pathn][pathpos]='\0';
                pos++;
                continue;
            }
            if(cutpaths.paths[pos+1]=='\0')
            {
                cutpaths.path[cutpaths.pathn][pathpos]='\0';
                pos++;
                continue;
            }
            cutpaths.path[cutpaths.pathn][pathpos]='\0';
            cutpaths.pathn++;
            pathpos=0;
        }
        else
        {
            cutpaths.path[cutpaths.pathn][pathpos]=cutpaths.paths[pos];
            pathpos++;
        }
        pos++;
    }
    int i=0;
    int dirid=0;
    if(1==1)
    {
        //printf("%d\n",cutpaths.pathn);
        for(i=0;i<=cutpaths.pathn;i++)
        {
            //printf("第%d步，在%s下寻找%s\n",i,temp->name,cutpaths.path[i]);
            if(strcmp(cutpaths.path[i],".")==0)
            {
                temp=temp;
            }
            else if(strcmp(cutpaths.path[i],"..")==0)
            {
                if(temp!=rootdir)
                {
                    temp=temp->pre;
                }
            }
            else if((dirid=hasfile(cutpaths.path[i],temp))>=0)
            {
                temp=temp->directory->child[dirid];
            }
            else
            {
                printf("目录不存在!\n");
                return -1;
            }
        }
    }
    /*
    else
    {
        printf("相对路径！%d\n",cutpaths.pathn);
        for(i=cutpaths.pathn;i>=0;i--)
        {
            if((dirid=hasfile(cutpaths.path[i],curdir))>0)
            {
                temp=temp->directory->child[dirid];
            }
            else
            {
                return -1;
            }
        }
    }
    */
    /*else
    {
        dirid=hasfile(cutpaths.path[0],curdir);
        temp=temp->directory->child[dirid];
    }*/
    cutpaths.pathn=0;
    cutpaths.cd=temp;
    if(cutpaths.pathn<0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void addblock(int pos)
{
    int k=0;
    for(k=0;k<pos;k++)
    {
        block[disk]=1;
        disk++;
    }
    disk++;
}
int exec()
{
    if(strcmp(acmd.cmd[0],"ll")==0)
    {
        ll();
    }
    else if(strcmp(acmd.cmd[0],"ls")==0)
    {
        ls();
    }
    else if(strcmp(acmd.cmd[0],"disk")==0)
    {
        showblock();
    }
    else if(strcmp(acmd.cmd[0],"adduser")==0)
    {
        if(acmd.cmdn==1)
        {
            adduser();
        }
        else
        {
            printf("adduser <username>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"deluser")==0)
    {
        if(acmd.cmdn==1)
        {
            deluser();
        }
        else
        {
            printf("deluser <username>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"su")==0)
    {
        if(acmd.cmdn==1)
        {
            su(acmd.cmd[1]);
        }
        else
        {
            printf("su <username>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"passwd")==0)
    {
        if(acmd.cmdn==1)
        {
            passwd();
        }
        else
        {
            printf("passwd <username>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"cd")==0)
    {
        if(acmd.cmdn!=1)
        {
            printf("cd <dirname>\n");
            return 1;
        }
        int tt;
        tt=modcwd(acmd.cmd[1]);
        if(tt==1)
        {
            //正常

        }
        else if(tt==0)
        {
            printf("文件夹不存在！\n");
        }
        else if(tt==-1)
        {
            printf("打开失败！\n");
        }
        else
        {
            printf("其他错误！\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"help")==0)
    {
        showmenu();
    }
    else if(strcmp(acmd.cmd[0],"vi")==0)
    {
        if(acmd.cmdn==1)
        {
            vi(curdir,curdir->directory,acmd.cmd[1]);
        }
        else
        {
            printf("vi <filename>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"cat")==0)
    {
        if(acmd.cmdn==1)
        {
            cat(acmd.cmd[1]);
        }
        else
        {
            printf("cat <filename>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"rm")==0)
    {
        if(acmd.cmdn==1)
        {
            rm();
        }
        else
        {
            printf("rm <filename>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"rmdir")==0)
    {
        if(acmd.cmdn==1)
        {
            rmdir();
        }
        else
        {
            printf("rmdir <dirname>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"mkdir")==0)
    {
        if(acmd.cmdn==1)
        {
            mkdir(curdir,curdir->directory,acmd.cmd[1]);
        }
        else
        {
            printf("mkdir <dirname>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"chmod")==0)
    {
        if(acmd.cmdn==2)
        {
            chmod();
        }
        else
        {
            printf("chmod <filename> <quanxian>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"gcc")==0)
    {
        printf("[/-_-\\] 没有这个功能\n");
    }
    else if(strcmp(acmd.cmd[0],"cp")==0)
    {
        if(acmd.cmdn==2)
        {
            cp(acmd.cmd[1],acmd.cmd[2]);
        }
        else
        {
            printf("cp <filename> <newfile>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"mv")==0)
    {
        if(acmd.cmdn==2)
        {
            mv(acmd.cmd[1],acmd.cmd[2]);
        }
        else
        {
            printf("mv <oldname> <newname>\n");
        }
    }
    else if(strcmp(acmd.cmd[0],"cls")==0)
    {
        CLS
    }
    else
    {
        printf("%s : command not found\n",acmd.cmd[0]);
    }
    return 1;
}
void gettimestr(long long int t,char *timestr)
{
    char temp[50];
    time_t tt;
    tt=t;
    struct tm *info;
    info=gmtime(&tt);
    strftime(temp, sizeof(temp), "%Y-%m-%d %H:%M:%S", info);
    strcpy(timestr,temp);
}
void getcwd()
{
    char tempstr[500];
    struct filetree *temp;
    temp=curdir;
    if(curdir==rootdir)
    {
        strcpy(cwdstr,"/");
    }
    else if(curdir==user[uid].home)
    {
        strcpy(cwdstr,"~");
    }
    else
    {
        strcpy(cwdstr,temp->name);
        while((temp=temp->pre)!=rootdir)
        {
            strcpy(tempstr,cwdstr);
            strcpy(cwdstr,temp->name);
            strcat(cwdstr,"/");
            strcat(cwdstr,tempstr);
        }
        strcpy(tempstr,cwdstr);
        strcpy(cwdstr,"/");
        strcat(cwdstr,tempstr);
    }
}
