#include "mlogging.h"
#include "arrfuncs.h"


enum MC2_EquTknType {
	EMPTY, 
	OP_PLUS,
	OP_MINUS,
	OP_MULTIPLIY,
	OP_DIVIDE,
	OP_EXPONENT, 
	L_PAR,
	R_PAR,
	INTEGER,
	DECIMAL,
};


struct MC2_EquationToken {
	enum MC2_EquTknType type;

	union {
		double fvalue;
		long  ivalue;
	};
};


int checkEqu(const char* equ) {
	const char validChars[20] = ".0123456789+-*/^() ";

	for(unsigned i = 0; equ[i] != '\0'; ++i) {
		if( !charInStr(equ[i], validChars) ) return 0;
	}

	return 1;
}


int convertStrToInt(const char *str) {
	unsigned i = 0;
	while( str[i] != '\0') i++;

	unsigned placeValue = 1; int total = 0;
	if( checkStr(str, "0123456789 ") ) {
		for(int j = i - 1; j != -1; --j) {
			if( str[j] != ' ') {                
				total += ( (str[j] - '0') * placeValue );
				placeValue *= 10;
			}
		}
	}

	return total;
}


double convertStrToDouble(const char *str) {
	double total = 0.0;

	if( checkStr(str, " 0123456789.") && (numOfChars('.', str) == 1)) {
		const int decimalIndex = findIndex_str('.', str, findStrLen(str));

		int i = decimalIndex + 1;
		total = 0.0;
		double pv = 0.1;

		while(i < findStrLen(str)) {
			if(str[i] != ' ') {
				total += (str[i] - '0') * pv;
				pv *= 0.1;
				i++;
			} else {
				i++;
			}
		}
		i = decimalIndex - 1;
		pv = 1.0;
		while(i > -1) {
			total += (str[i] - '0') * pv;
			pv *= 10;
			i--;
		}

	} else {
		MLOG_formatted_error("mcalc2.c:convertStrToDouble :: unable to convert \"%s\" to double\n", str);
	}

	return total; 
}


double MC2_pow(const double base, const int power) {
	int temp_base = base;

	for(unsigned i = 0; i < (unsigned) power - 1; ++i) {
		temp_base *= base;
	}

	return temp_base;
}


void MC2_EquationTokenAppend(struct MC2_EquationToken token, struct MC2_EquationToken tokens[]) {
	unsigned i = 0;
	while(tokens[i].type != EMPTY) i++;
	tokens[i] = token;
}


/// @brief tokenizes the equation from a string into MC2_EquationToken structs
/// @param equ must already be checked when passed to this function
/// @param tokens ALL TOKENS MUST BE INITIALIZED TO EMPTY
void MC2_read_equ(const char* equ, struct MC2_EquationToken tokens[]) {
	struct MC2_EquationToken temp_token;
	char temp_num[12] = "          ";
	unsigned short tnPtr = 0;

	for(unsigned i = 0; equ[i] != '\0'; ++i) {
		switch(equ[i]) {
			case '+':
				temp_token.type = OP_PLUS;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			case '-':
				temp_token.type = OP_MINUS;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			case '*':
				temp_token.type = OP_MULTIPLIY;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			case '/':
				temp_token.type = OP_DIVIDE;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			case '^':
				temp_token.type = OP_EXPONENT;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			case ' ':
				break;
			case '(':
				temp_token.type = L_PAR;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			case ')':
				temp_token.type = R_PAR;
				MC2_EquationTokenAppend(temp_token, tokens);
				break;
			default: {                
				short isDecimal = 0;

				while( charInStr(equ[i], "0123456789.") ) {
					if(equ[i] == '.') { isDecimal = 1; }
					
					temp_num[tnPtr] = equ[i];
					tnPtr++;

					i++;
					continue;
				}

				if(isDecimal) {
					temp_token.type = DECIMAL;
					temp_token.fvalue = convertStrToDouble(temp_num);
				} else {
					temp_token.type = INTEGER;
					temp_token.ivalue = convertStrToInt(temp_num);
				} 

				MC2_EquationTokenAppend(temp_token, tokens);
				for(unsigned j = 0; j < tnPtr; ++j) {
					temp_num[j] = ' ';
				}

				i--;
				break;
			}

		}
	}
}


enum OpsNParsType {
	INDEX_TRACKERS = 0,
	LEFT_PAR    = 1,
	RIGHT_PAR   = 2,
	EXPONENT    = 3,
	MULT_N_DIV  = 4,
	ADD_N_SUB   = 5,
};


enum IndexTrackerType {
	PARENTHESIS_INDEX = 0,
	EXPONENT_INDEX    = 1,
	MULT_N_DIV_INDEX  = 2,
	ADD_N_SUB_INDEX   = 3,
};


/// @brief reads in operators and parenthesis for order of operations
void MC2_read_opsNPars(struct MC2_EquationToken tokens[], short opsNPars[6][5]) {	
	unsigned len = 0;
	while(tokens[len].type != EMPTY) len++;

	for(int i = len; i > -1; i--) {
		if(tokens[i].type == L_PAR) {
			unsigned temp = i;
			while(tokens[temp].type != R_PAR) temp++;
			opsNPars[LEFT_PAR][ opsNPars[INDEX_TRACKERS][PARENTHESIS_INDEX] ] = i;
			opsNPars[RIGHT_PAR][ opsNPars[INDEX_TRACKERS][PARENTHESIS_INDEX] ] = temp;
			tokens[i].type = EMPTY;
			tokens[temp].type = EMPTY;
			opsNPars[INDEX_TRACKERS][0]++;
		} else if(tokens[i].type == OP_EXPONENT) {
			opsNPars[EXPONENT][ opsNPars[INDEX_TRACKERS][EXPONENT_INDEX] ] = i;
			opsNPars[INDEX_TRACKERS][EXPONENT_INDEX]++;
		} else if(tokens[i].type == OP_MULTIPLIY || tokens[i].type == OP_DIVIDE) {
			opsNPars[MULT_N_DIV][ opsNPars[INDEX_TRACKERS][MULT_N_DIV_INDEX] ] = i;
			opsNPars[INDEX_TRACKERS][MULT_N_DIV_INDEX]++;
		} else if(tokens[i].type == OP_PLUS || tokens[i].type == OP_MINUS) {
			opsNPars[ADD_N_SUB][ opsNPars[INDEX_TRACKERS][ADD_N_SUB_INDEX] ] = i;
			opsNPars[INDEX_TRACKERS][ADD_N_SUB_INDEX]++;
		}
	}

}


enum OpNumPermutations {
	InI,
	InD,
	DnI,
	DnD,
};


void MC2_subEvaluateOp(enum OpNumPermutations order, struct MC2_EquationToken tokens[],\
                        const int index, const int before, const int after) {

	switch(order) {
		case InI:
			switch(tokens[index].type) {
				case OP_PLUS:
					tokens[index].ivalue = tokens[before].ivalue + tokens[after].ivalue;						
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MINUS:
					tokens[index].ivalue = tokens[before].ivalue - tokens[after].ivalue;						
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MULTIPLIY:
					tokens[index].ivalue = tokens[before].ivalue * tokens[after].ivalue;						
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_DIVIDE:
					tokens[index].ivalue = tokens[before].ivalue / tokens[after].ivalue;						
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_EXPONENT:
					tokens[index].ivalue = MC2_pow(tokens[before].ivalue, tokens[after].ivalue);
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				default: break;
			}
			tokens[index].type = INTEGER;
			break;
		case InD:
			switch(tokens[index].type) {
				case OP_PLUS:
					tokens[index].fvalue = tokens[before].ivalue + tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MINUS:
					tokens[index].fvalue = tokens[before].ivalue - tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MULTIPLIY:
					tokens[index].fvalue = tokens[before].ivalue * tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_DIVIDE:
					tokens[index].fvalue = tokens[before].ivalue / tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_EXPONENT:
					tokens[index].ivalue = MC2_pow(tokens[before].ivalue, tokens[after].fvalue);
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				default: break;
			}
			tokens[index].type = DECIMAL;
			break;
		case DnI:
			switch(tokens[index].type) {
				case OP_PLUS:
					tokens[index].fvalue = tokens[before].fvalue + tokens[after].ivalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MINUS:
					tokens[index].fvalue = tokens[before].fvalue - tokens[after].ivalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MULTIPLIY:
					tokens[index].fvalue = tokens[before].fvalue * tokens[after].ivalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_DIVIDE:
					tokens[index].fvalue = tokens[before].fvalue / tokens[after].ivalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_EXPONENT:
					tokens[index].ivalue = MC2_pow(tokens[before].fvalue, tokens[after].ivalue);
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				default: break;
			}
			tokens[index].type = DECIMAL;
			break;
		case DnD:
			switch(tokens[index].type) {
				case OP_PLUS:
					tokens[index].fvalue = tokens[before].fvalue + tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MINUS:
					tokens[index].fvalue = tokens[before].fvalue + tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_MULTIPLIY:
					tokens[index].fvalue = tokens[before].fvalue + tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_DIVIDE:
					tokens[index].fvalue = tokens[before].fvalue + tokens[after].fvalue;
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				case OP_EXPONENT:
					tokens[index].ivalue = MC2_pow(tokens[before].fvalue, tokens[after].fvalue);
					tokens[before].type = EMPTY; tokens[after].type = EMPTY;
					break;
				default: break;
			}
			tokens[index].type = DECIMAL;
			break;
	}
}


void MC2_evaluateOp(struct MC2_EquationToken tokens[], int index) {
	unsigned before = index;
	unsigned after  = index;

	while( tokens[before].type != INTEGER && tokens[before].type != DECIMAL ) {

		before--;
	}
	while( tokens[after].type != INTEGER && tokens[after].type != DECIMAL ) {
		after++;
	}


	if(tokens[before].type == INTEGER && tokens[after].type == INTEGER) {
		MC2_subEvaluateOp(InI, tokens, index, before, after);

	} else if(tokens[before].type == DECIMAL && tokens[after].type == DECIMAL) {
		MC2_subEvaluateOp(DnD, tokens, index, before, after);

	} else if(tokens[before].type == INTEGER && tokens[after].type == DECIMAL) {
		MC2_subEvaluateOp(InD, tokens, index, before, after);

	} else if(tokens[before].type == DECIMAL && tokens[after].type == INTEGER) {
		MC2_subEvaluateOp(DnI, tokens, index, before, after);

	}
}


/// @brief takes in equation in string format, evaluates it into numerical value
/// @param equation will result in error if any invalid chars are present.
/// valid chars are "0123456789. ()+-*/^"
/// @return computed value of equation, following order of operations
double MC2_evaluate(const char* equation) {
	double total = 0.0;
	struct MC2_EquationToken tokens[25];
	for(unsigned i = 0; i < ARR_SIZE(tokens); ++i) tokens[i].type = EMPTY;
	short ops_n_pars[6][5] = {
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	};

	if( checkEqu(equation) ) {
		MC2_read_equ(equation, tokens);
		MC2_read_opsNPars(tokens, ops_n_pars);
	
		// parenthesis
		for(unsigned i = 0; i <= (unsigned)ops_n_pars[0][0]; ++i) {
			int j = 0;

			j = ops_n_pars[0][1] == 0 ? ops_n_pars[0][1] : ops_n_pars[0][1] - 1; // exponent
			while(ops_n_pars[3][j] > ops_n_pars[1][i] && ops_n_pars[3][j] < ops_n_pars[2][i]) {
				MC2_evaluateOp(tokens, ops_n_pars[3][j]);
				ops_n_pars[3][j] = 0;
				j--;

			}
			j = ops_n_pars[0][2] == 0 ? ops_n_pars[0][2] : ops_n_pars[0][2] - 1; // multiplication & division
			while(ops_n_pars[4][j] > ops_n_pars[1][i] && ops_n_pars[4][j] < ops_n_pars[2][i]) {
				MC2_evaluateOp(tokens, ops_n_pars[4][j]);
				ops_n_pars[4][j] = 0;
				j--;

			}
			j = ops_n_pars[0][3] == 0 ? ops_n_pars[0][3] : ops_n_pars[0][3] - 1; // addition & subtraction
			while(ops_n_pars[5][j] > ops_n_pars[1][i] && ops_n_pars[5][j] < ops_n_pars[2][i]) {
				MC2_evaluateOp(tokens, ops_n_pars[5][j]);
				ops_n_pars[5][j] = 0;
				j--;

			}
		}

		// exponent operators
		for(unsigned i = 0; i < (unsigned) ops_n_pars[0][1]; ++i) 
			if(ops_n_pars[3][i] != 0) {
				MC2_evaluateOp(tokens, ops_n_pars[3][i]);
			}
		// multiplication & division operators
		for(unsigned i = 0; i < (unsigned) ops_n_pars[0][2]; ++i)
			if(ops_n_pars[4][i]) {
				MC2_evaluateOp(tokens, ops_n_pars[4][i]);
			}
		// addition & subtraction operators
		for(unsigned i = 0; i < (unsigned) ops_n_pars[0][3]; ++i)
			if(ops_n_pars[5][i]) {				
				MC2_evaluateOp(tokens, ops_n_pars[5][i]);
			}
	} else {
		MLOG_formatted_error("mcalc2.c:MC2_evaluate::Unable to evaluate equation \"%s\" \n", equation);
	}

	for(unsigned i = 0; i < ARR_SIZE(tokens); ++i) {
		if(tokens[i].type == INTEGER) { total += tokens[i].ivalue; }
		else if(tokens[i].type == DECIMAL) { total += tokens[i].fvalue; }
	}

	return total;
}

