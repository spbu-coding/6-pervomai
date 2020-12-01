#include "sortings.h"
#include <string.h>

#define ASCII_NUM 128

void swap(char* string_1[], char* string_2[]){
    char* buf = *string_1;
    *string_1 = *string_2;
    *string_2 = buf;
}

void bubble(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator){
    for (size_t i = 0; i < array_size; i++){
        for (size_t j = 0; j < array_size - i - 1; j++){
            if (comparator(strings_array[j], strings_array[j + 1]) > 0){
                swap(&strings_array[j],&strings_array[j + 1]);
            }
        }
    }
}

void insertion(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator){
    for (size_t i = 1; i < array_size; i++) {
        char* buf = strings_array[i];
        size_t j = i;
        while (j > 0 && comparator(strings_array[j - 1],strings_array[i]) > 0) {
            strings_array[j] = strings_array[j - 1];
            j--;
        }
        strings_array[j] = buf;
    }
}

void sub2_merge_sort(size_t left, size_t mid,size_t right,strings_array_t strings_array, comparator_func_t comparator){
   char* temp[right - left + 1];
   size_t i = left, j = mid + 1, k = 0;
   while (i <= mid && j <= left){
       if (comparator(strings_array[i], strings_array[j]) > 0){
           temp[k] = strings_array[i];
           k++;
           j++;
       }
   }

   while(i <= mid){
       temp[k] = strings_array[i];
       k++;
       i++;
   }
    while(j <= right){
        temp[k] = strings_array[j];
        k++;
        j++;
    }
    for (i = left; i <= right; i++){
        strings_array[i] = temp[i - left];
    }

}
void sub1_merge_sort(size_t left, size_t right, strings_array_t strings_array, comparator_func_t comparator){
    if (left < right){
        size_t mid = left + (right - left) / 2;
        sub1_merge_sort(left, mid, strings_array, comparator);
        sub1_merge_sort(mid + 1, right, strings_array, comparator);
        sub2_merge_sort(left, mid, right, strings_array, comparator);
    }
}
void merge (strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator){
    sub1_merge_sort(0, array_size -1, strings_array, comparator);
}

void quick (strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator){
    size_t pivot = array_size / 2;
    size_t left = 0;
    size_t right = array_size - 1;
    do {
        while (comparator(strings_array[left],strings_array[pivot]) < 0){
            left++;
        }
        while (comparator(strings_array[pivot], strings_array[right]) < 0){
            right--;
        }
        if (left <= right){
            swap(&strings_array[left],&strings_array[right]);
            left++;
            right--;
        }
    } while (left <= right);

    if (right > 0){
        quick(strings_array, right + 1, comparator);
    }

    if (left < array_size){
        quick(strings_array + 1, array_size - 1, comparator);
    }
}


void count_sort(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator, size_t digit) {
    char *output_array[array_size];
    size_t i;
    size_t count[ASCII_NUM] = {0};

    for (i = 0; i < array_size; i++) {
        count[strings_array[i][digit]]++;
    }

    for (i = 1; i < ASCII_NUM; i++) {
        count[i] += count[i - 1];
    }

    for (i = array_size - 1; (int) i >= 0; i--) {
        output_array[count[strings_array[i][digit]] - 1] = strings_array[i];
        count[strings_array[i][digit]]--;
    }

    for (i = 0; i < array_size; i++) {
        strings_array[i] = output_array[i];
    }

    for(i = 0; i < array_size / 2; i++ ) {
        if (comparator(strings_array[i], strings_array[array_size - i - 1]) > 0) {
            swap(&strings_array[i],&strings_array[array_size - i - 1]);
        }
    }
}
void radix(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator){
    size_t max_string_length = 0;
    for (size_t i = 0; i < array_size; i++ ){
        if (strlen(strings_array[i]) > max_string_length){
            max_string_length = strlen(strings_array[i]);
        }
    }

    for (size_t digit = max_string_length - 1; (int) digit >= 0; digit--) {
        count_sort(strings_array, array_size, comparator, digit);
    }
}



