// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "stdafx.h"

#include <iostream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

int main() {

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

		//define handles and variables
		SQLHANDLE sqlConnHandle;
		SQLHANDLE sqlStmtHandle;
		SQLHANDLE sqlEnvHandle;
		SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

		//initializations
		sqlConnHandle = NULL;
		sqlStmtHandle = NULL;

		//allocations
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
			goto COMPLETED;

		if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			goto COMPLETED;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
			goto COMPLETED;

		//output
		cout << "Attempting connection to SQL Server...";
		cout << "\n";

		//connect to SQL Server  
		//I am using a trusted connection and port 14808
		//it does not matter if you are using default or named instance
		//just make sure you define the server name and the port
		//You have the option to use a username/password instead of a trusted connection
		//but is more secure to use a trusted connection
		switch (SQLDriverConnect(sqlConnHandle,
			NULL,
			(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=211.250.158.116,1433;DATABASE=master;UID=sa;PWD=1234;",
			//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=211.250.158.116, 1433;DATABASE=master;Trusted=true;",
			SQL_NTS,
			retconstring,
			1024,
			NULL,
			SQL_DRIVER_NOPROMPT)) {

		case SQL_SUCCESS:
			cout << "Successfully connected to SQL Server";
			cout << "\n";
			break;

		case SQL_SUCCESS_WITH_INFO:
			cout << "Successfully connected to SQL Server";
			cout << "\n";
			break;

		case SQL_INVALID_HANDLE:
			cout << "Could not connect to SQL Server sql_invalid_handle";
			cout << "\n";
			goto COMPLETED;

		case SQL_ERROR:
			cout << "Could not connect to SQL Server  sql_error";
			cout << "\n";
			goto COMPLETED;

		default:
			break;
		}

		//if there is a problem connecting then exit application
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
			goto COMPLETED;

		//output
		cout << "\n";
		cout << "Executing T-SQL query...";
		cout << "\n";

		//if there is a problem executing the query then exit application
		//else display query result
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT * FROM [MYDB1].[dbo].[STOCK]", SQL_NTS)) {
			cout << "Error querying SQL Server";
			cout << "\n";
			goto COMPLETED;
		}
		else {

			//declare output variable and pointer
			SQLCHAR sqlVersion[SQL_RESULT_LEN];
			SQLINTEGER ptrSqlVersion;

			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

				//display query result
				cout << "\nQuery Result:\n\n";
				cout << sqlVersion << endl;
			}
		}

		//close connection and free resources
	COMPLETED:
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		SQLDisconnect(sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

		//pause the console window - exit when key is pressed
		cout << "\nPress any key to exit...";
		getchar();
}


// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
