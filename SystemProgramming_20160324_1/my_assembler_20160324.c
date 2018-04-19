/*
 * 화일명 : my_assembler_00000000.c 
 * 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
 * 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
 * 파일 내에서 사용되는 문자열 "00000000"에는 자신의 학번을 기입한다.
 */

/*
 *
 * 프로그램의 헤더를 정의한다. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//#include <stdafx.h>

// 파일명의 "00000000"은 자신의 학번으로 변경할 것.
#include "my_assembler_20160324.h"

/* ----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일 
 * 반환 : 성공 = 0, 실패 = < 0 
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다. 
 *		   또한 중간파일을 생성하지 않는다. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[]) 
{
	if(init_my_assembler()< 0)		// 파일들 잘 읽히는 지 확인
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n"); 
		return -1 ; 
	}

	if(assem_pass1() < 0 ){
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n") ; 
		return -1 ; 
	}
	
	make_opcode_output("output_20160324.txt");

	/*
	* 추후 프로젝트에서 사용되는 부분
	*
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n") ; 
		return -1 ; 
	}

	make_objectcode_output("output") ; 
	*/

	return 0;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0 , 에러 발생 = -1
 * 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 
 *		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
 *		   구현하였다. 
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
 * 설명 : 머신을 위한 기계 코드목록 파일(inst.data)을 읽어 기계어 목록 테이블(inst_table)을 
 *        생성하는 함수이다. 
 * 매계 : 기계어 목록 파일
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : 기계어 목록파일 형식은 자유롭게 구현한다. 예시는 다음과 같다.
 *	
 *	===============================================================================
 *		   | 이름 | 기계어 코드 | 형식 | 오퍼랜드의 갯수 | NULL|
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
 * 설명 : 어셈블리 할 소스코드를 읽어 소스코드 테이블(input_data)를 생성하는 함수이다. 
 * 매계 : 어셈블리할 소스파일명
 * 반환 : 정상종료 = 0 , 에러 < 0  
 * 주의 : 라인단위로 저장한다.
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
		memset(buffer, '\0', 100); // 버퍼 초기화

		if (feof(file) != 0)
			break;
	}

	fclose(file);

	return errno;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 
 *        패스 1로 부터 호출된다. 
 * 매계 : 파싱을 원하는 문자열  
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다. 
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
 * 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 
 * 매계 : 토큰 단위로 구분된 문자열 
 * 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0 
 * 주의 : 
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
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	for (label_num = 0; label_num < inputLineNum; label_num++) {
		token_parsing(input_data[label_num]);
	}
	
	/* input_data의 문자열을 한줄씩 입력 받아서 
	 * token_parsing()을 호출하여 token_unit에 저장
	 */

}


/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 명령어 옆에 OPCODE가 기록된 표(과제 4번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*        또한 과제 4번에서만 쓰이는 함수이므로 이후의 프로젝트에서는 사용되지 않는다.
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
* ------------------------- 추후 프로젝트에서 사용할 함수 --------------------------*
* --------------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */

}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 object code (프로젝트 1번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */

}
