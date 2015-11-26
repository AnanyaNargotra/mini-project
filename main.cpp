#include<iostream>
using namespace std;
#include<cstdio>
#include<fstream>
#include<cstdlib>
#include<cstring>
int main()
{
  char ar1[10],ar2[10],ar3[15],ar4[15];
  char label[10],opcode[10],operand[10],sym[10];
  char ch;
  int x,i,adrs,adrs2,len,alen,f_adrs,p_adrs,j=0;
  int diff,diff1,sum;
  char mnemonic[15][15]={"LDA","STA","LDCH","STCH","ADD","SUB","MUL","DIV","J","JSUB"};
  int locatn_countr=0x0,s,loc,count=0x0,z=0,bol=0;
  char op_code[15][15]={"00","0C","50","54","28","29","3A","3B","3E","3F"};
  FILE *prog_pointr,*prog_pointr2,*prog_pointr3,*prog_pointr4;
cin>>ar3;
  prog_pointr = fopen("intermediate.txt","w");
  prog_pointr3 = fopen(ar3,"r");
 cout<<"Program:\n";
  ch=fgetc(prog_pointr3);
  while(ch!=EOF)
  {
   cout<<ch;
   ch=fgetc(prog_pointr3);
  }
 rewind(prog_pointr3);
//PASS 1(started)
prog_pointr2=fopen("symbol_table.txt","w");
  fscanf(prog_pointr3,"%s%s%s%X",label,opcode,operand,&adrs);
    if(strcmp(operand,"START")==0)
    {
            s=adrs;
            locatn_countr=s;
            p_adrs=locatn_countr;
            count=adrs;
        fprintf(prog_pointr,"%s\t%s\t%X\n",opcode,operand,adrs);
    }
    else
        locatn_countr=0x0;
fscanf(prog_pointr3,"%s%s%s",label,opcode,operand);
while(strcmp(opcode,"END")!=0)
  {
      if(strcmp(label,"----")!=0)
        fprintf(prog_pointr2,"%s\t%X\n",label,locatn_countr);

        if(strcmp(opcode,"BYTE")==0)
            {
                fprintf(prog_pointr,"%X\t%s\t%s\t%s\t\n",adrs,label,opcode,operand);
                len=strlen(operand);
                adrs+=len-3;
                locatn_countr+=len-3;
            }
            else if(strcmp(opcode,"RESB")==0)
           {
                fprintf(prog_pointr,"%X\t%s\t%s\t%s\n",adrs,label,opcode,operand);
                adrs+=(atoi(operand));
                locatn_countr+=(atoi(operand));
           }
    else if (strcmp(opcode,"RESW")==0)
    {
        fprintf(prog_pointr,"%X\t%s\t%s\t%s\n",adrs,label,opcode,operand);
        adrs+=(3*(atoi(operand)));
         locatn_countr+=(3*(atoi(operand)));
    }
   else
   {
      fprintf(prog_pointr,"%X\t%s\t%s\t%s\n",adrs,label,opcode,operand);
       adrs+=3;
       locatn_countr+=3;
   }
   fscanf(prog_pointr3,"%s%s%s",label,opcode,operand);
  }
  fprintf(prog_pointr,"%X\t%s\t%s\t%s\n",adrs,label,opcode,operand);
  loc=adrs;
  fclose(prog_pointr);
  fclose(prog_pointr2);
  fclose(prog_pointr3);
    cout<<"\nSYMBOL TABLE :\n\n";
  prog_pointr2=fopen("symbol_table.txt","r");
  ch=fgetc(prog_pointr2);
  while(ch!=EOF)
  {
   cout<<ch;
   ch=fgetc(prog_pointr2);
  }
fclose(prog_pointr2);
//pass 1(end)
cout<<"\n\nSTART OF PASS 2 ";
 prog_pointr = fopen("final.txt","w");
  prog_pointr2 = fopen("symbol_table.txt","r");
  prog_pointr3 = fopen("intermediate.txt","r");
  prog_pointr4 = fopen("object_code.txt","w");

  fscanf(prog_pointr3,"%s%s%s",label,opcode,operand);

  while(strcmp(opcode,"END")!=0)
  {
   p_adrs=adrs;
   fscanf(prog_pointr3,"%X%s%s%s",&adrs,label,opcode,operand);
  }

  f_adrs=adrs;
  fclose(prog_pointr3);

 prog_pointr3=fopen("intermediate.txt","r");

  fscanf(prog_pointr3,"%s%s%s",label,opcode,operand);
  if(strcmp(opcode,"START")==0)
  {
   fprintf(prog_pointr,"\t%s\t%s\t%s\n",label,opcode,operand);
   fprintf(prog_pointr4,"H^%s^00%s^00%X\n",label,operand,f_adrs);
   fscanf(prog_pointr3,"%X%s%s%s",&adrs,label,opcode,operand);
   x=adrs;
   diff=p_adrs-x;
    fprintf(prog_pointr4,"T^00%X^%X",adrs,diff);
  }
  while(strcmp(opcode,"END")!=0)
  {
   if(strcmp(opcode,"BYTE")==0)
   {
    fprintf(prog_pointr,"%X\t%s\t%s\t%s\t",adrs,label,opcode,operand);
    len=strlen(operand);
    alen=len-3;
    fprintf(prog_pointr4,"^");
    for(i=2;i<(alen+2);i++)
    {
     itoa(operand[i],ar2,16);
     fprintf(prog_pointr,"%s",ar2);
     fprintf(prog_pointr4,"%s",ar2);
    }
    fprintf(prog_pointr,"\n");
   }
   else if(strcmp(opcode,"WORD")==0)
   {
    len=strlen(operand);
    itoa(atoi(operand),ar1,10);
    fprintf(prog_pointr,"%X\t%s\t%s\t%s\t00000%s\n",adrs,label,opcode,operand,ar1);
    fprintf(prog_pointr4,"^00000%s",ar1);
   }
   else if((strcmp(opcode,"RESB")==0)||(strcmp(opcode,"RESW")==0))
    fprintf(prog_pointr,"%X\t%s\t%s\t%s\n",adrs,label,opcode,operand);
   else
   {
    while(strcmp(opcode,mnemonic[j])!=0)
     j++;
    if(strcmp(operand,"COPY")==0)
     fprintf(prog_pointr,"%X\t%s\t%s\t%s\t%s0000\n",adrs,label,opcode,operand,op_code[j]);
    else
    {
     rewind(prog_pointr2);
     fscanf(prog_pointr2,"%s%X",sym,&sum);
      while(strcmp(operand,sym)!=0)
       fscanf(prog_pointr2,"%s%X",sym,&sum);
     fprintf(prog_pointr,"%X\t%s\t%s\t%s\t%s%X\n",adrs,label,opcode,operand,op_code[j],sum);
     fprintf(prog_pointr4,"^%s%X",op_code[j],sum);
    }
   }
   fscanf(prog_pointr3,"%X%s%s%s",&adrs,label,opcode,operand);
  }
  fprintf(prog_pointr,"%X\t%s\t%s\t%s\n",adrs,label,opcode,operand);
  fprintf(prog_pointr4,"\nE^00%X",x);
  fclose(prog_pointr);
  fclose(prog_pointr2);
  fclose(prog_pointr3);
  fclose(prog_pointr4);
  cout<<("\nFinal Program\n");
  prog_pointr=fopen("final.txt","r");
  ch=fgetc(prog_pointr);
  while(ch!=EOF)
  {
   cout<<ch;
   ch=fgetc(prog_pointr);
  }
  cout<<"\nProgram Length \t \n";
  cout<<loc-count;
  cout<<"\nOBJECT CODE:\n";
  prog_pointr4=fopen("object_code.txt","r");
  ch=fgetc(prog_pointr4);
  while(ch!=EOF)
  {
   cout<<ch;
   ch=fgetc(prog_pointr4);
  }
  fclose(prog_pointr);
  fclose(prog_pointr3);
  fclose(prog_pointr4);
return 0;
}
