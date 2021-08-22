#include "stdio.h"

#include "structstring.h"

int main() {
    string helloWorld;
    string justWorld = stringMakeFromCString("World");
    char toFind[3] = {'?', '?', '?'}; 
    size_t atOffset;


    helloWorld = stringMakeFromCString(",");
    printf("\"%s\"\n", helloWorld.cstr); // ","


    stringInsertCString(&helloWorld, 0, "Hello");
    printf("\"%s\"\n", helloWorld.cstr); // "Hello,"


    stringAppendChar(&helloWorld, ' ');
    printf("\"%s\"\n", helloWorld.cstr); // "Hello, ";


    stringAppendCArray(&helloWorld, toFind, 3);
    printf("\"%s\"\n", helloWorld.cstr); // "Hello, ???";


    stringAppendChar(&helloWorld, '!');
    printf("\"%s\"\n", helloWorld.cstr); // "Hello, ???!";


    atOffset = stringFindCArray(&helloWorld, 0, toFind, 3);
    stringErase(&helloWorld, atOffset, atOffset + 2);
    printf("\"%s\"\n", helloWorld.cstr); // "Hello, !";


    stringInsertString(&helloWorld, atOffset, &justWorld);
    printf("\"%s\"\n", helloWorld.cstr); // "Hello, World!";


    stringCleanup(&helloWorld);
}