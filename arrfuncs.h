#ifndef __MTECH__ARR_FUNCS_H_
	#define __MTECH__ARR_FUNCS_H_


	#define ARR_SIZE(arr) sizeof( arr ) / sizeof( arr [0] )

static inline int charInStr(const char argChar, const char argStr[]) {
	for(unsigned i = 0; argStr[i] != '\0'; ++i) {
		if(argChar == argStr[i]) return 1;
	}

	return 0;
}


static inline int checkStr(const char* argStr, const char validChars[]) {
	for(unsigned i = 0; argStr[i] != '\0'; ++i) {
		if( !(charInStr(argStr[i], validChars)) ) {
			return 0;
		}
	}

	return 1;
}


static inline int intInArr(const int argInt, const int argArr[], unsigned size) {
	for(unsigned i = 0; i < size; ++i) {
		if(argInt == argArr[i]) return 1;
	}

	return 0;
}

// returns -1 if value isn't found
static inline int findIndex_intArr(const int value, const int argArr[], unsigned long arrLen) {
	for(unsigned i = 0; i < arrLen; ++i) {
		if(argArr[i] == value) return i;
	}

	return -1;
}

static inline int findIndex_str(const char value, const char str[], unsigned long arrLen) {
	for(unsigned i = 0; i < arrLen; ++i) {
		if(str[i] == value) return i;
	}

	return -1;
}

static inline int findStrLen(const char* argStr) {
	unsigned i = 0;
	while(argStr[i] != '\0') i++;
	return i;
} 

static inline int numOfChars(const char argChar, const char* argStr) {
	unsigned count = 0;
	for(unsigned i = 0; i < (unsigned) findStrLen(argStr); ++i) {
		if(argStr[i] == argChar) count++;
	}
	return count;
}

#endif

