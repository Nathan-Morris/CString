#ifndef _STRUCT_STRING_
#define _STRUCT_STRING_

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define STRING_NPOS SIZE_MAX

typedef struct {
    char* cstr;
    size_t len;
} string, *pstring;



//
// String Makers, (constructors)
//
static string stringMakeFromCArray(char* carr, size_t carrLen) {
    string str;

    if (carrLen == SIZE_MAX) {
        carrLen -= 1;
    }

    str.cstr = (char*)malloc(carrLen + 1);
    str.len = 0;

    if (str.cstr != NULL) {
        memcpy(str.cstr, carr, carrLen);
        str.cstr[carrLen] = '\0';
        str.len = carrLen;
    }

    return str;
}

static string stringMakeFill(size_t len, char fill) {
    string str;

    if (len == SIZE_MAX) {
        len -= 1;
    }

    str.cstr = (char*)malloc(len + 1);
    str.len = 0;

    if (str.cstr != NULL) {
        str.len = len;
        str.cstr[len] = '\0';

        while (len--) {
           str.cstr[len] = fill; 
        }
    }

    return str;
}

static string stringMakeFromCString(const char* cstr) {
    return stringMakeFromCArray((char*)cstr, strlen(cstr));
}

static string stringMakeCopy(pstring str) {
    return stringMakeFromCArray(str->cstr, str->len);
}

static string stringMake(size_t len) {
    return stringMakeFill(len, ' ');
}



//
// Appends
//
static pstring stringAppendCArray(pstring str, char* carr, size_t carrLen) {
    char* newCstr;
    size_t newLen;

    if (carrLen == 0) {
        return str;
    }

    // todo, check for integer overflow
    newLen = str->len + carrLen;
    newCstr = (char*)malloc(newLen + 1);

    if (newCstr != NULL) {
        memcpy(newCstr, str->cstr, str->len);
        memcpy(&newCstr[str->len], carr, carrLen);

        free(str->cstr);

        newCstr[newLen] = '\0';

        str->len = newLen;
        str->cstr = newCstr;
    }

    return str;
}

static pstring stringAppendCString(pstring str, const char* cstr) {
    return stringAppendCArray(str, (char*)cstr, strlen(cstr));
}

static pstring stringAppendChar(pstring str, char c) {
    return stringAppendCArray(str, &c, 1);
}

static pstring stringAppendString(pstring str, pstring toAppend) {
    return stringAppendCArray(str, toAppend->cstr, toAppend->len);
}



//
// Inserts
//
static pstring stringInsertCArray(pstring str, size_t at, char* carr, size_t carrLen) {
    char* newCstr;
    size_t newLen;

    if (carrLen == 0) {
        return str;
    }
    
    if (at >= str->len) {
        return stringAppendCArray(str, carr, carrLen);
    }

    newLen = str->len + carrLen;
    newCstr = (char*)malloc(newLen + 1);

    if (newCstr != NULL) {
        memcpy(newCstr, str->cstr, at);
        memcpy(&newCstr[at], carr, carrLen);
        memcpy(&newCstr[at + carrLen], &str->cstr[at], str->len - at);

        free(str->cstr);

        newCstr[newLen] = '\0';

        str->len = newLen;
        str->cstr = newCstr;
    }

    return str;
}

static pstring stringInsertCString(pstring str, size_t at, const char* cstr) {
    return stringInsertCArray(str, at, (char*)cstr, strlen(cstr));
}

static pstring stringInsertChar(pstring str, size_t at, char c) {
    return stringInsertCArray(str, at, &c, 1);
}

static pstring stringInsertString(pstring str, size_t at, pstring toInsert) {
    return stringInsertCArray(str, at, toInsert->cstr, toInsert->len);
}



//
// Erases
//
// erases [begin:end], inclusively
static pstring stringErase(pstring str, size_t begin, size_t end) {
    char* newCstr;
    size_t newLen;

    if (str->len == 0) {
        return str;
    }

    if (end < begin) {
        newLen = end;
        end = begin;
        begin = newLen;
    }

    if (end >= str->len) {
        end = str->len - 1;
    }

    if (begin >= str->len) {
        begin = str->len - 1;
    }

    newLen = str->len - (end - begin + 1);
    newCstr = (char*)malloc(newLen + 1);

    if (newCstr != NULL) {
        memcpy(newCstr, str->cstr, begin);
        memcpy(&newCstr[begin], &str->cstr[end + 1], newLen - begin);

        free(str->cstr);

        newCstr[newLen] = '\0';

        str->len = newLen;
        str->cstr = newCstr;
    }

    return str;
}



//
// Finds
//
static size_t stringFindCArray(pstring str, size_t beginOffset, char* carr, size_t carrLen) {
    if (beginOffset >= str->len || str->len == 0) {
        return STRING_NPOS;
    }

    for (; beginOffset < str->len - carrLen; beginOffset++) {
        if (memcmp(&str->cstr[beginOffset], carr, carrLen) == 0) {
            return beginOffset;
        }
    }
    return STRING_NPOS;
}

static size_t stringFindCString(pstring str, size_t beginOffset, const char* cstr) {
    return stringFindCArray(str, beginOffset, (char*)cstr, strlen(cstr));
}

static size_t stringFindChar(pstring str, size_t beginOffset, char c) {
    for (; beginOffset < str->len; beginOffset++) {
        if (str->cstr[beginOffset] == c) {
            return beginOffset;
        }
    }
    return STRING_NPOS;
}

static size_t stringFindString(pstring str, size_t beginOffset, pstring toFind) {
    return stringFindCArray(str, beginOffset, toFind->cstr, toFind->len);
}



//
// Equals
//
static char stringEqualsCArray(pstring str, char caseInsensitive, char* carr, size_t carrLen) {
    if (carrLen != str->len) {
        return 0;
    }

    if (caseInsensitive == 0) { // compare case
        while (carrLen--) {
            if (str->cstr[carrLen] != carr[carrLen]) {
                return 0;
            }
        }
    } else { // dont compare case
        while (carrLen--) {
            if (tolower((unsigned char)str->cstr[carrLen]) != tolower((unsigned char)carr[carrLen])) {
                return 0;
            }
        }
    }

    return 1;
}

static char stringEqualsCString(pstring str, char caseInsensitive, const char* cstr) {
    return stringEqualsCArray(str, caseInsensitive, (char*)cstr, strlen(cstr));
}

static char stringEqualsString(pstring str, char caseInsensitive, pstring toCheckEquivalence) {
    return stringEqualsCArray(str, caseInsensitive, toCheckEquivalence->cstr, toCheckEquivalence->len);
}



//
// CaseChange
//
static pstring stringToUpper(pstring str) {
    for (size_t i = 0; i != str->len; i++) {
        str->cstr[i] = (char)toupper((unsigned char)str->cstr[i]);
    }
}

static pstring stringToLower(pstring str) {
    for (size_t i = 0; i != str->len; i++) {
        str->cstr[i] = (char)tolower((unsigned char)str->cstr[i]);
    }
}



//
// Substring
//
static string stringSubstring(pstring str, size_t begin, size_t end) {
    if (str->len == 0) {
        return *str;
    }

    if (end < begin) {
        size_t tmp;
        tmp = end;
        end = begin;
        begin = tmp;
    }

    if (end >= str->len) {
        end = str->len - 1;
    }

    if (begin >= str->len) {
        begin = str->len - 1;
    }

    return stringMakeFromCArray(&str->cstr[begin], end - begin + 1);
}



//
// Cleanup
//
static void stringCleanup(pstring str) {
    free(str->cstr); 
    str->len = 0;
}

#endif