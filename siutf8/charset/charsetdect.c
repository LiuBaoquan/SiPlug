/*
 * uchardet
 *   [https://github.com/BYVoid/uchardet]
 * or 
 *   [http://code.google.com/p/uchardet]
**/

#include <windows.h>
#include <stdio.h>
#include <uchardet.h>  

/* �������� */  
#define NUMBER_OF_SAMPLES   (64) 

typedef enum {
	CHARSET_ERR     = -1,
	CHARSET_UNKNOWN = 0,
	CHARSET_ANSI    = 1,
	CHARSET_UTF8,
	CHARSET_UTF16_BIGEND,
	CHARSET_UTF16_LITEND,
	CHARSET_OTHER   = 5
}CHARSET_TYPE;

#define PATH_FILE "utf8.txt"

int main(int argc, char *argv[])
{
#if 0
	CHARSET_TYPE cstype;
	char buffer[128] = {0};
	int size;

	FILE *fp;
	fp = fopen(PATH_FILE,"rb");

	size = fread(buffer, 64, 1, fp) ;
	printf("buffer: %s\n", buffer, size);
	if (size == 0) size = 20;
	cstype = chardect(buffer, size);
	printf("charset: %d\n", cstype);
	fclose(fp);
#endif
/*
 * http://blog.csdn.net/turingo/article/details/8136644
 *=========================================================
 * http://blog.csdn.net/a38017032/article/details/28089151
 *=========================================================
 */
    FILE* file;  
    char buf[NUMBER_OF_SAMPLES];  
    int len;  
    uchardet_t ud;  
    const char *charret = NULL;
    char charset[64] = {0};

    /* �򿪱�����ı��ļ�������ȡһ�������������ַ� */  
    file = fopen(PATH_FILE, "rt, ccs=UTF-8");
	if (file == NULL)
		return -1;
    len = fread(buf, sizeof(wchar_t), NUMBER_OF_SAMPLES, file);  
    //fclose(file);  
  
    /* ͨ�������ַ������ı����� */  
    ud = uchardet_new();  
    if(uchardet_handle_data(ud, buf, len) != 0) /* ��������ַ���������ô�п��ܵ��·���ʧ�� */  
    {  
        printf("��������ʧ�ܣ�\n");  
        return -1;  
    }  
    uchardet_data_end(ud);
	charret = uchardet_get_charset(ud);
	strcpy(charset, charret);
    printf("%s - %s\n", PATH_FILE, charset);
    uchardet_delete(ud);  

	int size, result;

    /* ��ȡ�ļ���С */  
    fseek (file , 0 , SEEK_END);  
    size = ftell (file);  
    rewind (file);  
  
    /* �����ڴ�洢�����ļ� */   
    wchar_t *buffer = (wchar_t *)malloc(size);  
    if (buffer == NULL)  
    {  
        fputs ("Memory error",stderr);   
        exit (2);  
    }  
  
    /* ���ļ�������buffer�� */  
    result = fread (buffer,sizeof(wchar_t),size/sizeof(wchar_t),file);
	printf("result: %d\tsize: %d\n", result, size);
    if (result != size/sizeof(wchar_t))  
    {  
        fputs ("Reading error",stderr);  
        exit (3);  
    }  
    /* ���������ļ��Ѿ���buffer�У����ɱ�׼�����ӡ���� */  
    printf("buffer:\n%s", buffer);   

	size_t dwNum = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, FALSE);
    FILE *filenew = fopen("gb.txt", "wt");
    char *buffernew = (char *)malloc(sizeof(char)*dwNum);  
	result = WideCharToMultiByte(CP_ACP, 0, buffer, -1, buffernew, dwNum, NULL, FALSE);
    printf("buffernew:\n%s\n", buffernew);   
	printf("result: %d\tdwNum: %d\n", result, dwNum);
	fwrite(buffernew, dwNum, sizeof(char), filenew);

    /* ������ʾ���ر��ļ����ͷ��ڴ� */  
    fclose (file);  
    free (buffer);  
    fclose (filenew);  
    free (buffernew);  
    return 0;  

}

