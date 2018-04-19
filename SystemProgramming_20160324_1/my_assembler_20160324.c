/*
 * ȭ�ϸ� : my_assembler_00000000.c 
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 * ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//#include <stdafx.h>

// ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_20160324.h"

/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ���� 
 * ��ȯ : ���� = 0, ���� = < 0 
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�. 
 *		   ���� �߰������� �������� �ʴ´�. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[]) 
{
	if(init_my_assembler()< 0)		// ���ϵ� �� ������ �� Ȯ��
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n"); 
		return -1 ; 
	}

	if(assem_pass1() < 0 ){
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n") ; 
		return -1 ; 
	}
	
	make_opcode_output("output_20160324.txt");

	/*
	* ���� ������Ʈ���� ���Ǵ� �κ�
	*
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n") ; 
		return -1 ; 
	}

	make_objectcode_output("output") ; 
	*/

	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���. 
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void)
{
	int result ; 
	
	if((result = init_inst_file("inst.data")) < 0 )
		return -1 ;
	if((result = init_input_file("input.txt")) < 0 )
		return -1 ; 

	return result ; 
}

/* ----------------------------------------------------------------------------------
 * ���� : �ӽ��� ���� ��� �ڵ��� ����(inst.data)�� �о� ���� ��� ���̺�(inst_table)�� 
 *        �����ϴ� �Լ��̴�. 
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
 *	
 *	===============================================================================
 *		   | �̸� | ���� �ڵ� | ���� | ���۷����� ���� | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char *inst_file)
{
	FILE * file;
	
	int errno;
	char c;
	char buf[2] = { "\0" };
	
	file = fopen(inst_file, "r+");
	
	while(1) {
		
		inst_table[instNum] = (inst*) calloc(1, sizeof(inst));
		inst_table[instNum]->name = (char*) calloc(5, sizeof(char));
		(inst_table[instNum]->operand_format) = (char*) calloc(2, sizeof(char));

		// fscanf(file, "%s\t%s\t%s\t%s\t\s", inst_table[instNum]->name, inst_table[instNum]->opcode, inst_table[instNum]->format, inst_table[instNum]->operand_num, inst_table[instNum]->operand_format);
		fscanf(file, "%s\t", inst_table[instNum]->name);
		fscanf(file, "%X\t", &(inst_table[instNum]->opcode));
		fscanf(file, "%d\t", &(inst_table[instNum]->format));
		fscanf(file, "%d", &(inst_table[instNum]->operand_num));
	
		if (inst_table[instNum]->operand_num == 1) {
			fscanf(file, "\t%s", buf);
			strcpy(inst_table[instNum]->operand_format, buf);
			//printf("%s\t%X\t%d\t%d\t%s\n", inst_table[instNum]->name, inst_table[instNum]->opcode, inst_table[instNum]->format, inst_table[instNum]->operand_num, inst_table[instNum]->operand_format);
		}
		else if (inst_table[instNum]->operand_num == 2) {
			//fscanf(file, "\t%s\t%s", &buf[0], &buf[1]);
			//strcpy(inst_table[instNum]->operand_format, &buf[0]);
			//strcpy(inst_table[instNum]->operand_format+1, &buf[1]);
			fscanf(file, "\t%s", buf);
			strcpy(inst_table[instNum]->operand_format, buf);
			fscanf(file, "\t%s", buf);
			strcpy(inst_table[instNum]->operand_format+1, buf);
			//fscanf("\t%c\t%c", inst_table[instNum]->operand_format, inst_table[instNum]->operand_format + 1);
			//printf("%s\t%X\t%d\t%d\t%s\n", inst_table[instNum]->name, inst_table[instNum]->opcode, inst_table[instNum]->format, inst_table[instNum]->operand_num, inst_table[instNum]->operand_format);
		}
		else{
			fscanf(file, "\t");
			strcpy(inst_table[instNum]->operand_format, "\0");
			//printf("%s\t%X\t%d\t%d\t%s\n", inst_table[instNum]->name, inst_table[instNum]->opcode, inst_table[instNum]->format, inst_table[instNum]->operand_num, inst_table[instNum]->operand_format);
		}
		
		memset(buf, "\0", 2);

		// fgets();

		if (feof(file) != 0)
			break;

		instNum++;
	}
	
	fclose(file);

	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�. 
 * �Ű� : ������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0  
 * ���� : ���δ����� �����Ѵ�.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{
	FILE * file;
	int errno;

	int k = 0;
	char ch = '\0';
	char buffer[100] = { '\0' };
	
	file = fopen(input_file, "r+");
	
	for (inputLineNum = 0; ch != EOF; inputLineNum++) {
		input_data[inputLineNum] = (char*)calloc(100, sizeof(char));
		fgets(buffer, 100, file);
		
		strcpy(input_data[inputLineNum], buffer);
		memset(buffer, '\0', 100); // ���� �ʱ�ȭ

		if (feof(file) != 0)
			break;
	}

	fclose(file);

	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
 *        �н� 1�� ���� ȣ��ȴ�. 
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�  
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
	char buffer[100] = { '\0' };
	char* tok = calloc(50, sizeof(char));
	char* optok = calloc(10, sizeof(char));


	token_table[label_num] = (token*)calloc(1, sizeof(token));
	token_table[label_num]->label = (char*)calloc(5, sizeof(char));
	token_table[label_num]->oper = (char*)calloc(5, sizeof(char));
	token_table[label_num]->comment = (char*)calloc(50, sizeof(char));

	strcpy(buffer, str);
	
	if (buffer[0] == '.') {
		tok = strtok(buffer, "\t");
		strcpy(token_table[label_num]->label, tok);
		tok = strtok(buffer, "\t");
		strcpy(token_table[label_num]->comment, tok);
		return 0;
	}
	else if (buffer[0] != '\t') {
		tok = strtok(buffer, "\t");
		strcpy(token_table[label_num]->label, tok);
		tok = strtok(NULL, "\t");
	}
	else{
		 tok = strtok(buffer, "\t");
	}

	strcpy(token_table[label_num]->oper, tok);

	optok = tok = strtok(NULL, "\t");

	if ((tok = strtok(NULL, "\t")) != NULL)
		strcpy(token_table[label_num]->comment, tok);

	if (optok != NULL) {
		if (optok == "RSUB") {
			strcpy(token_table[label_num]->comment, optok);
		}
		else {
			tok = strtok(optok, ",");
			token_table[label_num]->operand[0] = (char*)calloc(10, sizeof(char));
			strcpy(token_table[label_num]->operand[0], tok);

			for (int t = 1; t < MAX_OPERAND; t++) {
				if ((optok = strtok(NULL, ",")) != NULL) {
					token_table[label_num]->operand[t] = (char*)calloc(10, sizeof(char));
					strcpy(token_table[label_num]->operand[t], optok);
				}
				else {
					break;
				}
			}
		}
	}


	if((tok = strtok(NULL, "\t")) != NULL)
		strcpy(token_table[label_num]->comment, tok);

}

/* ----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
 * �Ű� : ��ū ������ ���е� ���ڿ� 
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
 * ���� : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str)
{
	for (int i = 0; i < instNum; i++) {
		if (str == NULL)
			return -1;
		else if (*str == '+') {
			int len = strlen(str);
			char buffer[10] = {'\0'};
			
			for (int t = 1; t < len; t++) {
				buffer[t-1] = *(str+t);
			}

			if (strcmp(buffer, inst_table[i]->name) == 0)
				return i;
		}

		else if (strcmp(str, inst_table[i]->name) == 0)
			return i;

		//if (strcmp(str, "BYTE" || "WORD" || "RESB" || "RESW" || "START" || "END") == 0)
		//return -1;
	}

	return -1;
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	for (label_num = 0; label_num < inputLineNum; label_num++) {
		token_parsing(input_data[label_num]);
	}
	
	/* input_data�� ���ڿ��� ���پ� �Է� �޾Ƽ� 
	 * token_parsing()�� ȣ���Ͽ� token_unit�� ����
	 */

}


/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 4��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char *file_name)
{
	FILE * file;

	file = fopen(file_name, "w+");

	int k = 0;
	char* tok = calloc(10, sizeof(char));

	for (int i = 0; i < inputLineNum; i++) {
		if (*token_table[i]->label == '.')
			;
		else {
			//fprintf(file, "%s\t", *token_table[i]->label);
			fputs(token_table[i]->label, file);
			fputs("\t", file);

			if (token_table[i]->oper != NULL) {
				token_table[i]->oper = strtok(token_table[i]->oper, "\n");
				fprintf(file, "%s", token_table[i]->oper);
				fputs("\t", file);
			}

			if (token_table[i]->operand != NULL) {
				for (int t = 0; t < MAX_OPERAND; t++) {
					if (token_table[i]->operand[t] != NULL) {
						tok = strtok(token_table[i]->operand[t], "\n");
						fputs(tok, file);
						fputs(" ", file);
					}
					else
						break;
				}
				fputs("\t", file);
			}

			if ((k = search_opcode(token_table[i]->oper)) >= 0)
				fprintf(file, "%02X", inst_table[k]->opcode);

			fputs("\n", file);
		}
	}
	
	fclose(file);
}



/* --------------------------------------------------------------------------------*
* ------------------------- ���� ������Ʈ���� ����� �Լ� --------------------------*
* --------------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */

}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */

}
