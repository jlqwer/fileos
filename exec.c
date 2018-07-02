#include "config.h"
void showblock()
{
    int k=0;
    for (k=0;k<5000;k++)
    {
        printf("%d",block[k]);
        if((k+1)%100==0)
            printf("\n");
    }
    printf("\n");
    printf("used： %d/5000\n",disk);
}
int rm()
{
    int fileid = hasfile(acmd.cmd[1],curdir);
    if(fileid>=0)
    {
        if(curdir->directory->child[fileid]->filetype=='f')
        {
            curdir->directory->child[fileid]->status=0;
        }
        else
        {
            printf("%s 是目录！\n",acmd.cmd[1]);
            return 0;
        }
    }
    else
    {
        printf("rm:没有这个文件！\n");
        return -1;
    }
    return -1;
}
int rmdir()
{
    int fileid = hasfile(acmd.cmd[1],curdir);
    if(fileid>=0)
    {
        if(curdir->directory->child[fileid]->filetype=='d')
        {
            int child=childnum(fileid);
            if(child>0)
            {
                char yes[10];
                printf("该目录非空，是否要删除该目录及目录下所有文件?(yes/no default:no):");
                scanf("%s",yes);
                if(strcmp(yes,"yes")==0)
                {
                    curdir->directory->child[fileid]->status=0;
                }
                return 0;
            }
            else
            {
                curdir->directory->child[fileid]->status=0;
                return 0;
            }
        }
        else
        {
            printf("%s 不是目录！\n",acmd.cmd[1]);
            return 0;
        }
    }
    else
    {
        printf("rm:没有这个文件或目录！\n");
        return -1;
    }

}
void chmod()
{
    int fileid=hasfile(acmd.cmd[1],curdir);
    if(fileid>=0)
    {
        curdir->directory->child[fileid]->authority[0]=acmd.cmd[2][0]-'0';
        curdir->directory->child[fileid]->authority[1]=acmd.cmd[2][1]-'0';
        curdir->directory->child[fileid]->authority[2]=acmd.cmd[2][2]-'0';
    }
    else
    {
        printf("文件不存在!\n");
    }
}
void ls()
{
    int i=0;
    if(check_rPermission(curdir)==0)
        return;
    for(i=0;i<curdir->directory->childnum;i++)
    {
        if(curdir->directory->child[i]->status!=1)
        {
            continue;
        }
        if(curdir->directory->child[i]->filetype=='d')
        {
            printf("*%s ",curdir->directory->child[i]->name);
        }
        else
        {
            printf("%s ",curdir->directory->child[i]->name);
        }
    }
    printf("\n");
}
void ll()
{
    int i=0;
    char qxstr[20];
    char name[200];
    char timestr[50];
    int file_size;
    if(check_rPermission(curdir)==0)
        return;
    for(i=0;i<curdir->directory->childnum;i++)
    {
        if(curdir->directory->child[i]->status==1)
        {
            gettimestr(curdir->directory->child[i]->create_time,timestr);
            strcpy(name,curdir->directory->child[i]->name);
            if(curdir->directory->child[i]->filetype=='d')
            {
                strcat(name,"/");
                file_size=0;
            }
            else
            {
                file_size=curdir->directory->child[i]->file->file_size;
            }
            getauthority(curdir->directory->child[i],qxstr);
            printf("%s\t%s\t%d\t%s\t%s\n",qxstr,user[curdir->directory->child[i]->user_id].name,file_size,timestr,name);
        }
        //printf("*%s ",curdir->directory->child[i]->name);
    }
    printf("\n");
}
int cat(char *filename)
{
    int i=0;
    for (i=0;i<curdir->directory->childnum;i++)
    {
        if((strcmp(filename,curdir->directory->child[i]->name)==0) && curdir->directory->child[i]->filetype=='f'/* && curdir->directory->child[i]->status==1*/)
        {
            if(check_rPermission(curdir->directory->child[i])==0)
                return -1;
            int j=0;
            for(j=0;j<curdir->directory->child[i]->file->pos;j++)
            {
                putchar(curdir->directory->child[i]->file->content[j]);
            }
            printf("\n");
            return 1;
        }
    }
    printf("没有文件%s!\n",filename);
    return 0;
}
int modcwd(char *dirname)
{
    if(strcmp(dirname,".")==0)
    {
        if(check_rPermission(curdir)==0)
            return -1;
        curdir=curdir;
        return 1;
    }
    else if(strcmp(dirname,"..")==0)
    {
        if(curdir!=rootdir)
        {
            if(check_rPermission(curdir->pre)==0)
                        return -1;
            curdir=curdir->pre;
        }
        return 1;
    }
    else if(strcmp(dirname,"~")==0)
    {
        curdir=user[uid].home;
        return 1;
    }
    else if(strcmp(dirname,"/")==0)
    {
        if(check_rPermission(rootdir)==0)
            return -1;
        curdir=rootdir;
        return 1;
    }
    else
    {
        strcpy(cutpaths.paths,dirname);
        cutpath();
        //int i=0;
        if(check_rPermission(cutpaths.cd)==0)
            return -1;
        curdir=cutpaths.cd;
        return 1;
    }
}
int mkdir(struct filetree *pre, struct directory *fpath,char *dirname)
{
    addblock(4);
    //其实fpatch等于pre->directory
    if(hasfile(dirname,pre)>=0)
    {
        printf("已存在名为%s的目录或文件！\n",dirname);
        return 0;
    }
    fpath->child[fpath->childnum]=(struct filetree*)malloc(sizeof(struct filetree));

    fpath->child[fpath->childnum]->authority[0]=7;
    fpath->child[fpath->childnum]->authority[1]=5;
    fpath->child[fpath->childnum]->authority[2]=1;
    fpath->child[fpath->childnum]->create_time=curtime();
    fpath->child[fpath->childnum]->filetype='d';
    fpath->child[fpath->childnum]->directory=(struct directory*)malloc(sizeof(struct directory));
    fpath->child[fpath->childnum]->status=1;
    fpath->child[fpath->childnum]->directory->childnum=0;
    strcpy((fpath->child[fpath->childnum]->name),dirname);
    fpath->child[fpath->childnum]->pre=pre;
    fpath->child[fpath->childnum]->user_id=uid;
    fpath->childnum++;
    return fpath->childnum-1;
}

struct file* createfile(struct filetree *pre,struct directory *fpath,char *filename)
{
    fpath->child[fpath->childnum]=(struct filetree*)malloc(sizeof(struct filetree));
    fpath->child[fpath->childnum]->filetype='f';
    fpath->child[fpath->childnum]->file=(struct file*)malloc(sizeof(struct file));
    fpath->child[fpath->childnum]->status=1;
    fpath->child[fpath->childnum]->file->file_size=0;
    strcpy(fpath->child[fpath->childnum]->file->content,"");
    strcpy(fpath->child[fpath->childnum]->name,filename);
    fpath->child[fpath->childnum]->file->pos=0;
    fpath->child[fpath->childnum]->authority[0]=6;
    fpath->child[fpath->childnum]->authority[1]=4;
    fpath->child[fpath->childnum]->authority[2]=4;
    fpath->child[fpath->childnum]->create_time=curtime();
    fpath->child[fpath->childnum]->pre=pre;
    fpath->child[fpath->childnum]->user_id=uid;
    fpath->childnum++;
    return fpath->child[fpath->childnum-1]->file;
}
int cp(char *yname,char *mname)
{
    int yfileid=hasfile(yname,curdir);
    if(yfileid<0)
    {
        printf("源文件不存在!\n");
        return 0;
    }
    else
    {
        int mfileid=hasfile(mname,curdir);
        if(mfileid<0)
        {
            struct file* mfile;
            mfile=createfile(curdir,curdir->directory,mname);
            strcpy(mfile->content,curdir->directory->child[yfileid]->file->content);
            mfile->pos=curdir->directory->child[yfileid]->file->pos;
            mfile->file_size=curdir->directory->child[yfileid]->file->pos;
            addblock(mfile->file_size);
            return 1;
        }
        else
        {
            char yes[10];
            printf("目标文件已经存在，是否覆盖？yes/no default:no :");
            scanf("%s",&yes);
            if(strcmp(yes,"yes")==0)
            {
                strcpy(curdir->directory->child[mfileid]->file->content,curdir->directory->child[yfileid]->file->content);
                curdir->directory->child[mfileid]->file->pos=curdir->directory->child[yfileid]->file->pos;
                curdir->directory->child[mfileid]->file->file_size=curdir->directory->child[yfileid]->file->pos;
                addblock(curdir->directory->child[mfileid]->file->file_size);
            }
            return 1;
        }
    }
}


int mv(char *yname,char *mname)
{
    int yfileid=hasfile(yname,curdir);
    if(yfileid<0)
    {
        printf("源文件不存在!\n");
        return 0;
    }
    else
    {
        int mfileid=hasfile(mname,curdir);
        if(mfileid<0)
        {
            strcpy(curdir->directory->child[yfileid]->name,mname);
            return 1;
        }
        else
        {
            strcpy(curdir->directory->child[yfileid]->name,mname);
            strcat(curdir->directory->child[yfileid]->name,"2");
            printf("目标文件已经存在，已重命名为%s！\n",curdir->directory->child[yfileid]->name);
            return -1;
        }
    }
}


int vi(struct filetree *pre,struct directory *fpath,char *filename)
{
    if(hasfile(filename,curdir)>=0)
    {
        printf("已存在名为%s的目录或文件！\n",filename);
        return 0;
    }
    struct file* vifile=createfile(pre,fpath,filename);
    printf("%s:\n",filename);
    char ch;
    while((ch=getchar())!=EOF)
    {
        vifile->content[vifile->pos]=ch;
        vifile->pos++;
    }
    vifile->content[vifile->pos]='\0';
    vifile->file_size=vifile->pos;
    addblock(vifile->pos);
    return 1;
}
int passwd()
{
    int puid= hasuser(acmd.cmd[1]);

    char oldpasswd[100];
    char newpasswd[100];
    char againpasswd[100];

    if(puid>=0)
    {
        if(uid==0)
        {
            printf("Pleace Enter %s's NEW Password:",acmd.cmd[1]);
            strcpy(newpasswd,inputpassword(newpasswd));
            printf("\nPleace Enter %s's NEW Password again:",acmd.cmd[1]);
            strcpy(againpasswd,inputpassword(againpasswd));

            if(strcmp(againpasswd,newpasswd)==0)
            {
                strcpy(user[puid].passowrd,newpasswd);
                printf("\n修改成功!\n");
                return 1;
            }
            else
            {
                printf("\n两次输入的密码不一致！\n");
                return -1;
            }
        }
        else
        {
            printf("Pleace Enter %s's OLD Password:",acmd.cmd[1]);
            strcpy(oldpasswd,inputpassword(oldpasswd));

            if(strcmp(oldpasswd,user[puid].passowrd)!=0)
            {
                printf("\n您输入的密码不正确！\n");
                return -1;
            }
            printf("\nPleace Enter %s's NEW Password:",acmd.cmd[1]);
            strcpy(newpasswd,inputpassword(newpasswd));

            printf("\nPleace Enter %s's NEW Password again:",acmd.cmd[1]);
            strcpy(againpasswd,inputpassword(againpasswd));

            if(strcmp(againpasswd,newpasswd)==0)
            {
                strcpy(user[puid].passowrd,newpasswd);
                printf("\n修改成功!\n");
                return 1;
            }
            else
            {
                printf("\n两次输入的密码不一致！\n");
                return -1;
            }
        }
    }
    else
    {
        printf("账户不存在！\n");
        return -1;
    }
}
int adduser()
{
    if(isadmin(cmdusername)!='#')
    {
        printf("您没有权限添加用户！\n");
        return 0;
    }
    if(hasuser(acmd.cmd[1])<0)
    {
        strcpy(user[usernum].name,acmd.cmd[1]);
        strcpy(user[usernum].passowrd,PASSWORD);
        user[usernum].status=1;
        int homeid;
        homeid=mkdir(home,home->directory,acmd.cmd[1]);
        user[usernum].home=home->directory->child[homeid];
        user[usernum].home->user_id=usernum;
        usernum++;
        return 1;
    }
    else
    {
        printf("该用户名已存在，添加失败！\n");
        return 0;
    }
}
int deluser()
{
    int duid=hasuser(acmd.cmd[1]);
    if(duid>0)
    {
        user[duid].status=0;
        printf("已删除该用户！\n");
        return 1;
    }
    else
    {
        printf("该用户不存在，或您无权删除此用户！\n");
        return 0;
    }
}

int login(char *loginname)
{
    char userpasswd[100];
    int luid;
    luid=hasuser(loginname);
    if(luid>=0)
    {
        printf("Pleace Enter %s's Password:",loginname);
        strcpy(userpasswd,inputpassword(userpasswd));
        if(strcmp(userpasswd,user[luid].passowrd)==0)
        {
            strcpy(cmdusername,loginname);
            return luid;
        }
        else
        {
            printf("密码错误！\n");
            return -1;
        }
    }
    else
    {
        printf("用户不存在或已禁用！\n");
        return -2;
    }
}
int su(char *uname)
{
    int suid=0;
    while((suid=login(uname))!=-3)
    {
        if(suid>=0)
        {

            printf("\n登录成功！\n");
            passworderrornum=0;
            uid=suid;
            return 1;
        }
        else if(suid==-1)
        {
            passworderrornum++;
            if(passworderrornum==3)
            {
                printf("您已经连续3次输入错误密码,系统即将退出！\n");
                passworderrornum=0;
                return 0;
            }
        }
        else if(suid==-2)
        {
            passworderrornum=0;
            return 0;
        }
    }
    return 0;
}

void showmenu()
{
    printf("\t\t\t*****************************************************************\n");
    printf("\t\t\t*                                                               *\n");
	printf("\t\t\t*      ls           列出目录                                    *\n");
	printf("\t\t\t*      ll           列出详细目录                                *\n");
	printf("\t\t\t*      adduer       添加用户                                    *\n");
	printf("\t\t\t*      su           切换用户                                    *\n");
	printf("\t\t\t*      deluser      删除用户                                    *\n");
	printf("\t\t\t*      passwd       修改用户密码                                *\n");
	printf("\t\t\t*      cd           切换目录                                    *\n");
	printf("\t\t\t*      help         显示帮助信息（菜单）                        *\n");
	printf("\t\t\t*      vi           编辑文件                                    *\n");
	printf("\t\t\t*      cat          查看文件                                    *\n");
	printf("\t\t\t*      rm           删除文件                                    *\n");
	printf("\t\t\t*      rmdir        删除文件夹                                  *\n");
	printf("\t\t\t*      poweroff     关闭                                        *\n");
	printf("\t\t\t*      chmod        修改文件权限                                *\n");
	printf("\t\t\t*      cp           复制文件                                    *\n");
	printf("\t\t\t*      mv           重命名                                      *\n");
	printf("\t\t\t*      disk         查看磁盘占用                                *\n");
	printf("\t\t\t*****************************************************************\n");
}

