#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int hasValue;
    int value;
    char* error;
} Promise;

Promise* create_promise(){
    Promise* promise = (Promise*)malloc(sizeof(Promise));
    promise->hasValue = 0;
    promise->value = 0;
    promise->error = NULL;
    return promise;
}

void set_value(Promise* promise, int value) {
    if (!promise->hasValue) {
        promise->value = value;
        promise->hasValue = 1;
    }
}

int has_value(Promise* promise) {
    return promise->hasValue;
}

int get_value(Promise* promise) {
    if (promise->hasValue) {
        return promise->value;
    } else {
        return NULL;
    }
}

void set_error(Promise* promise, char* error) {
    promise->error = error;
}

char* get_error(Promise* promise) {
    return promise->error;
}

int main() {
    Promise* promise = create_promise();

    printf("Value: %d\n", get_value(promise));
    set_value(promise, 8);
    printf("Has value: %d\n", has_value(promise));
    printf("Value: %d\n", get_value(promise));
    set_error(promise, "Error 404");
    printf("Error: %s\n", get_error(promise));

    return 0;
}
