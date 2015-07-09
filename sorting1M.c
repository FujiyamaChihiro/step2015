//
//  sorting1M.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMREAD 65536
void Qsort(int value[],int left,int right);
void swap(int value[],int i,int j);
void fileNotFound();
void merge0(int *i,int *j,int *id,int tmp1[],int tmp2[],int value1[]);
void merge1(int *i,int *j,int *full,int *id,int tmp1[],int tmp2[],int value1[]);

/*ファイルが見つからなかったときにエラーを表示して終了する関数*/
void fileNotFound(){
    printf("File not found.\n");
    exit(1);
}

/*配列に対してクイックソートを行う関数*/
void Qsort(int value[],int left,int right){
    int i,j;
    int pivot;
    i=left;//ソートする配列の１番小さい要素の添え字
    j=right;//ソートする配列の１番大きい要素の添え字
    pivot=value[(left+right)/2];//配列の基準値を配列の中央付近に取る
    while(1){
        while(value[i]<pivot)i++;//pivotより大きい値が出るまでiを増加させる
        while(pivot<value[j])j--;//pivotより小さい値が出るまでjを減少させる
        if(i>=j)break;
        swap(value,i,j);
        i++,j--;
    }
    if(left<i-1)Qsort(value,left,i-1);//基準値の左に2つ以上要素があれば左の配列をソートする
    if(j+1<right)Qsort(value,j+1,right);//基準値の右に2つ以上要素があれば右の配列をソートする
}

/*配列の要素を交換する*/
void swap(int value[],int i,int j){
    int tmp=value[i];
    value[i]=value[j];
    value[j]=tmp;
}

/*マージする関数(0段階目)*/
void merge0(int *i,int *j,int *id,int tmp1[],int tmp2[],int value1[]){
    int local_id=*id;
    int local_i=*i,local_j=*j;
    while(local_id<NUMREAD){
        if(tmp1[local_i]<=tmp2[local_j]){
            value1[local_id]=tmp1[local_i];
            local_i++,local_id++;
            if(local_i==NUMREAD){
                while(local_j<NUMREAD){
                    value1[local_id]=tmp2[local_j];
                    local_j++,local_id++;
                }
            }
            
        }else if(tmp2[local_j]<tmp1[local_i]){
            value1[local_id]=tmp2[local_j];
            local_j++,local_id++;
            if(local_j==NUMREAD){
                while(local_i<NUMREAD){
                    value1[local_id]=tmp1[local_i];
                    local_i++,local_id++;
                }
            }
        }
    }
    *id=local_id;
    *i=local_i;
    *j=local_j;
}

/*マージする関数(1段階目)*/
void merge1(int *i,int *j,int *full,int *id,int tmp1[],int tmp2[],int value1[]){
    int local_id=*id;
    int local_i=*i,local_j=*j;
    while(local_id<NUMREAD){
        if(tmp1[local_i]<=tmp2[local_j]){
            value1[local_id]=tmp1[local_i];
            local_i++,local_id++;
            if(local_i==NUMREAD){
                *full=1;
                break;
            }
            
        }else if(tmp2[local_j]<tmp1[local_i]){
            value1[local_id]=tmp2[local_j];
            local_j++,local_id++;
            if(local_j==NUMREAD){
                *full=2;
                break;
            }
        }
        if(local_id==NUMREAD)*full=0;
    }
    *id=local_id;
    *i=local_i;
    *j=local_j;
}


int main(){
    int value[NUMREAD];
    FILE* fp;
    FILE* fp1;
    FILE* fp2;
    FILE* fp12;
    fp=fopen("test1","rb");//バイナリモードで読み込み
    if(fp==NULL)fileNotFound();
    for(int i=0;i<4;i++){
        fread(&value,sizeof(int),NUMREAD,fp);
        Qsort(value,0,NUMREAD);//valueを昇順にソート
        //value[]のソート完了
        if(i==0)fp1=fopen("tmp1","wb");//バイナリモードで書き込み
        if(i==1)fp1=fopen("tmp2","wb");//バイナリモードで書き込み
        if(i==2)fp1=fopen("tmp3","wb");//バイナリモードで書き込み
        if(i==3)fp1=fopen("tmp4","wb");//バイナリモードで書き込み
        if(fp1==NULL)fileNotFound();
        fwrite(&value,sizeof(int),NUMREAD,fp1);
        fclose(fp1);
    }
    fclose(fp);
    
    /*ここまででtestファイルを4つのファイルにソートして入れることが完了*/
    /*ここからファイルを2つずつをマージしていく*/
    /*tmp1とtmp2をマージ*/
    int tmp1[NUMREAD],tmp2[NUMREAD];//一時的に格納する配列
    fp1=fopen("tmp1","rb");//バイナリモードで読み込み
    fp2=fopen("tmp2","rb");//バイナリモードで読み込み
    fread(&tmp1,sizeof(int),NUMREAD,fp1);
    fread(&tmp2,sizeof(int),NUMREAD,fp2);
    fclose(fp1);
    fclose(fp2);
    /*マージ*/
    int value1[NUMREAD];
    int i=0,j=0,id=0;
    merge0(&i,&j,&id,tmp1,tmp2,value1);
    fp12=fopen("tmp12","wb");//tmp1とtmp2を途中までマージした結果をtmp12ファイルに書き出す
    if(fp12==NULL)fileNotFound();
    fwrite(&value1,sizeof(int),NUMREAD,fp12);
    id=0;
    merge0(&i,&j,&id,tmp1,tmp2,value1);
    fwrite(&value1,sizeof(int),NUMREAD,fp12);
    fclose(fp12);
    /*ここまででtmp1とtmp2がマージできた*/
    
    /*tmp3とtmp4をマージ*/
    fp1=fopen("tmp3","rb");//バイナリモードで読み込み
    fp2=fopen("tmp4","rb");//バイナリモードで読み込み
    fread(&tmp1,sizeof(int),NUMREAD,fp1);
    fread(&tmp2,sizeof(int),NUMREAD,fp2);
    fclose(fp1);
    fclose(fp2);
    /*マージ*/
    i=0,j=0,id=0;
    merge0(&i,&j,&id,tmp1,tmp2,value1);
    fp12=fopen("tmp34","wb");//tmp1とtmp2を途中までマージした結果をtmp34ファイルに書き出す
    if(fp12==NULL)fileNotFound();
    fwrite(&value1,sizeof(int),NUMREAD,fp12);
    id=0;
    merge0(&i,&j,&id,tmp1,tmp2,value1);
    fwrite(&value1,sizeof(int),NUMREAD,fp12);
    fclose(fp12);
    /*ここまででtmp3とtmp4がマージできた*/
    
    /*tmp12とtmp34をマージする*/
    int full=0;
    fp1=fopen("tmp12","rb");
    fp2=fopen("tmp34","rb");
    fread(&tmp1,sizeof(int),NUMREAD,fp1);
    fread(&tmp2,sizeof(int),NUMREAD,fp2);
    i=0,j=0;
    id=0;
    merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    fp12=fopen("result1M","wb");
    if(fp12==NULL)fileNotFound();
    if(full==0){
        fwrite(&value1,sizeof(int),NUMREAD,fp12);
        id=0;
    }
    merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    if(full==1){
        fread(&tmp1,sizeof(int),NUMREAD,fp1);
        i=0;
        merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    }else if(full==2){
        fread(&tmp2,sizeof(int),NUMREAD,fp2);
        j=0;
        merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    }
    if(full==0){
        fwrite(&value1,sizeof(int),NUMREAD,fp12);//ファイル２回目書き込み
        id=0;
    }
    merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    if(full==0){
        fwrite(&value1,sizeof(int),NUMREAD,fp12);
        id=0;
    }
    if(full==1){
        fread(&tmp1,sizeof(int),NUMREAD,fp1);
        i=0;
        merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    }else if(full==2){
        fread(&tmp2,sizeof(int),NUMREAD,fp2);
        j=0;
        merge1(&i,&j,&full,&id,tmp1,tmp2,value1);
    }
    if(full==0){
        fwrite(&value1,sizeof(int),NUMREAD,fp12);
        id=0;
    }
    merge0(&i,&j,&id,tmp1,tmp2,value1);
    fwrite(&value1,sizeof(int),NUMREAD,fp12);
    
    fclose(fp1);
    fclose(fp2);
    fclose(fp12);
    
}