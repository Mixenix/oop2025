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
    wstring wrd = word;
    wstring rus_chars = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (wchar_t c : wrd) {
        if (rus_chars.find(c) == wstring::npos) {
            throw invalid_argument("Found english characters or numbers in the word!");
        }
    }

    if (wrd.empty()) return L"";
    wstring result;
    wstring vowels = L"аеёиоуыэюяАЕЁИОУЫЭЮЯ";

    for (int i = 0; i < wrd.length(); i++) {
        result += wrd[i];
        if (vowels.find(word[i]) != wstring::npos && i < wrd.length() - 1) {
            result += L"-";
        }
    }
    return result.c_str();
}
