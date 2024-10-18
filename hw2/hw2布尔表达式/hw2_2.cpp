#include<iostream>
#include<string>
using namespace std;

struct MyStack {
	char* elem;
	int size;
};

int Push_Stack(MyStack& S, char elem)
{
	S.size++;
	char* sp = (char*)realloc(S.elem, (S.size) * sizeof(char));
	if (sp == NULL)
		return -1;
	S.elem = sp;

	S.elem[S.size - 1] = elem;

	return 0;
}

int Pop_Stack(MyStack& S)
{
	S.size--;
	if (S.size > 0) {
		char* sp = (char*)realloc(S.elem, (S.size) * sizeof(char));
		if (sp == NULL)
			return -1;
		S.elem = sp;
	}
	else {
		free(S.elem);
		S.elem = NULL;
		S.size = 0;
	}
	return 0;
}

int Precede(char opr1, char opr2)
{
	switch (opr1) {
		case '(':
			if (opr2 == ')')
				return 0;
			else
				return -1;
		case '|':
			switch (opr2) {
				case '|':
				case ')':
					return 1;
				case '&':
				case '!':
				case '(':
					return -1;
			}
		case '&':
			switch (opr2) {
			case '|':
			case ')':
			case '&':
				return 1;
			
			case '!':
			case '(':
				return -1;
			}
		case '!':
			switch (opr2) {
			case '|':
			case ')':
			case '&':
				return 1;
			case '!':
			case '(':
				return -1;
			}
		case ')':
			return 1;
	}	
	return -2;
}

char Operate(MyStack num, char opr)
{
	
	switch (opr) {
		case '!':
			if (num.elem[num.size - 1] == 'V')
				return 'F';
			else
				return 'V';
		case '&':
			if (num.elem[num.size - 2] == 'V') {
				if (num.elem[num.size - 1] == 'V')
					return 'V';
				else
					return 'F';
			}
			else
				return 'F';
		case '|':
			if (num.elem[num.size - 2] == 'F') {
				if (num.elem[num.size - 1] == 'V')
					return 'V';
				else
					return 'F';
			}
			else
				return 'V';
	}
	return 0;
}

int main()
{
	string Input[21];
	int istr = 0;
	while (getline(cin, Input[istr])) {
		if (Input[istr].empty())
			break;
		istr++;
	}
	for (int i = 0; i < istr; i++) {
		MyStack bnum, sign;
		bnum.size = 0;
		bnum.elem = NULL;
		sign.size = 0;
		sign.elem = NULL;
		for (int j = 0; j < Input[i].length();) {
			if (Input[i][j] == 'V' || Input[i][j] == 'F') {
				Push_Stack(bnum, Input[i][j]);
				j++;
			}				
			else if (Input[i][j] != ' ') {
				if (sign.size) {
					int priority = Precede(sign.elem[sign.size - 1], Input[i][j]);
					switch (priority) {
					case -1:
						Push_Stack(sign, Input[i][j]);
						j++;
						break;
					case 0:
						Pop_Stack(sign);
						j++;
						break;
					case 1:
						char result = Operate(bnum, sign.elem[sign.size - 1]);						
						Pop_Stack(bnum);
						if (sign.elem[sign.size - 1] != '!')
							Pop_Stack(bnum);
						Pop_Stack(sign);
						Push_Stack(bnum, result);						
					}
				}
				else {
					Push_Stack(sign, Input[i][j]);
					j++;
				}					
			}
			else
				j++;
			/*cout << "运算" << j + 1 << endl;
			cout << "bnum: ";
			for (int c = 0; c < bnum.size; c++)
				cout << bnum.elem[c];
			cout << endl;
			cout << "sign: ";
			for (int c = 0; c < sign.size; c++)
				cout << sign.elem[c];
			cout << endl;
			cout << endl;*/
		}
		//读取后运算
		while (sign.size) {
			char result = Operate(bnum, sign.elem[sign.size - 1]);
			Pop_Stack(bnum);
			if (sign.elem[sign.size - 1] != '!')
				Pop_Stack(bnum);
			Pop_Stack(sign);
			Push_Stack(bnum, result);
		}
		char final_result = bnum.elem[bnum.size - 1];
		printf("Expression %d: %c\n", i + 1, final_result);
		if (sign.size || bnum.size > 1)
			cout << "error";
		else
			free(bnum.elem);
	}
	return 0;
}