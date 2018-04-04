#include <stdio.h>
#include <string.h>
 #include<stdlib.h>

#define u32 unsigned int
#define u8 unsigned char

u8 StepRoundIm(u32 *key,u32 *IV)
{
	u8 res=0;
	u8 reg0[1000]={0};
	u8 reg1[1000]={0};
	u8 reg2[1000]={0};
	int round=721;
	for(int i=0;i<80;i++)
	{
		int word=i/32; int bit=i%32;
		reg0[92-i]=((key[word]>>bit)&1);
		reg1[83-i]=((IV[word]>>bit)&1);
	}
	reg2[0]=1;reg2[1]=1;reg2[2]=1;
	for(int i=0;i<round-64;i++)
	{
		reg0[i+93]=reg2[i+45]^(reg2[i+1]&reg2[i+2])^reg2[i]^reg0[i+24];
		reg1[i+84]=reg0[i+27]^(reg0[i+1]&reg0[i+2])^reg0[i]^reg1[i+6];
		reg2[i+111]=reg1[i+15]^(reg1[i+1]&reg1[i+2])^reg1[i]^reg2[i+24];
	}
	res=reg0[93+round-65]^reg0[93+round-92]^reg1[84+round-68]^reg1[84+round-83]^reg2[111+round-65]^reg2[111+round-110];
	u8 result=((reg1[290+84]<<1)|res);
	return result;
}

int main()
{
		u32 key[3]={0x01234567,0x89abcdef,0x0000ffff};
		u32 key1[3]={0x76543210,0xfedcba98,0x00000123};
		u32 key2[3]={0x00112233,0x44556677,0x0000890a};
		u32 key3[3]={0x11223344,0x55667788,0x000090ab};
		u32 key4[3]={0x22334455,0x66778899,0x0000abcd};
		u32 IV[3]={0};
		//The index can be changed according to your choice of cubes.
		int cube_index[32]={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,64,66,68};
	
	int len1=0;int len2=0;int len3=0;
	for(int i=0;i<32;i++)
	{
		if(cube_index[i]<32)
			len1++;
		else if(cube_index[i]<64)
			len2++;
		else
			len3++;
	}
	
	unsigned char res=0;
	for(unsigned int s1=0;s1<(1<<len1);s1++)
	{
		IV[0]=0;
		for(int i1=0;i1<len1;i1++)
			IV[0] |= (((s1>>i1)&1)<<(cube_index[i1]));
		for(unsigned int s2=0;s2<(1<<len2);s2++)
		{
			IV[1]=0;
			for(int i2=0;i2<len2;i2++)
				IV[1] |= (((s2>>i2)&1)<<(cube_index[len1+i2]-32));
			IV[2]=0;
			for(unsigned int s3=0;s3<(1<<len3);s3++)
			{
				for(int i3=0;i3<len3;i3++)
					IV[2] |= (((s3>>i3)&1)<<(cube_index[len1+len2+i3]-64));
				unsigned char r=StepRoundIm(key,IV);//
				unsigned char rr=r&1;
				res ^=((1^((r>>1)&1))&rr);
			}	
		}
	}
	FILE *fp=fopen("test.txt","wb");   
	fprintf(fp,"%d",res);
	fclose(fp);
	return 0;
	
}