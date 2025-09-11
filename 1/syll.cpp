#include "header.h"

wstring syllabify(const wstring& word) {
    wstring rus_chars = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (wchar_t c : word) {
        if (rus_chars.find(c) == wstring::npos) {
            throw invalid_argument("Found english characters or numbers in the word!");
        }
    }

    if (word.empty()) return L"";
    wstring result;
    wstring vowels = L"аеёиоуыэюяАЕЁИОУЫЭЮЯ";

    for (int i = 0; i < word.length(); i++) {
        result += word[i];
        if (vowels.find(word[i]) != wstring::npos && i < word.length() - 1) {
            result += L"-";
        }
    }

    return result;

}

const wchar_t* syllabify(const wchar_t* word) {
    if (word == nullptr) {
        throw invalid_argument("Input word is NULL");
    }
    wchar_t rus_chars[] = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    wchar_t vowels[] = L"аеёиоуыэюяАЕЁИОУЫЭЮЯ";
    const wchar_t* word_ptr = word;


    while (*word_ptr != '\0') {
        wchar_t* res = wcschr(rus_chars, *word_ptr);
        if (res == NULL) {
            throw invalid_argument("Found english characters or numbers in the word!");
        }
        word_ptr++;
    }

    if (word[0] == L'\0') {
        wchar_t* result = new wchar_t[1];
        result[0] = L'\0';
        return result;
    }

    size_t result_length = 0;
    for (int i = 0; word[i] != L'\0'; i++) {
        result_length++;
        if (wcschr(vowels, word[i]) != NULL && word[i + 1] != L'\0') {
            result_length += 1;
        }
    }

    wchar_t* result = new wchar_t[result_length + 1];
    int result_index = 0;

    for (int i = 0; word[i] != L'\0'; i++) {
        result[result_index++] = word[i];

        if (wcschr(vowels, word[i]) != NULL && word[i + 1] != L'\0') {
            result[result_index++] = L'-';
        }
    }

    result[result_index] = L'\0';
    const wchar_t* res = (const wchar_t*)result;
    // delete[] result;
    return res;
    
}
