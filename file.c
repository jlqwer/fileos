#include "config.h"
int hasfile(char *dirname,struct filetree *finddir)
{
    int i;
    for (i=0;i<finddir->directory->childnum;i++)
    {
        if((strcmp(dirname,finddir->directory->child[i]->name)==0))
        {
            if(finddir->status==1)
            {
                return i;
            }
            else
            {
                return -1;
            }
        }
    }
    return -1;
}
char *rwx(int aqx,char *aqxstr)
{
    switch(aqx)
    {
    case 0:
        aqxstr[0]='-';
        aqxstr[1]='-';
        aqxstr[2]='-';
        break;
    case 1:
        aqxstr[0]='-';
        aqxstr[1]='-';
        aqxstr[2]='x';
        break;
    case 2:
        aqxstr[0]='-';
        aqxstr[1]='w';
        aqxstr[2]='-';
        break;
    case 4:
        aqxstr[0]='r';
        aqxstr[1]='-';
        aqxstr[2]='-';
        break;
    case 3:
        aqxstr[0]='-';
        aqxstr[1]='w';
        aqxstr[2]='x';
        break;
    case 5:
        aqxstr[0]='r';
        aqxstr[1]='-';
        aqxstr[2]='x';
        break;
    case 6:
        aqxstr[0]='r';
        aqxstr[1]='w';
        aqxstr[2]='-';
        break;
    case 7:
        aqxstr[0]='r';
        aqxstr[1]='w';
        aqxstr[2]='x';
        break;
    default:
        aqxstr[0]='-';
        aqxstr[1]='-';
        aqxstr[2]='-';
    }
    aqxstr[3]='\0';
    return aqxstr;
}
char *getauthority(struct filetree *qx,char *qxstr)
{
    char temp[10];
    if(qx->filetype=='d')
    {
        qxstr[0]='d';
    }
    else
    {
        qxstr[0]='-';
    }
    qxstr[1]='\0';
    rwx(qx->authority[0],temp);
    strcat(qxstr,temp);
    qxstr[4]='\0';
    rwx(qx->authority[1],temp);
    strcat(qxstr,temp);
     qxstr[7]='\0';
    rwx(qx->authority[2],temp);
    strcat(qxstr,temp);
    qxstr[10]='\0';
    return qxstr;
}

