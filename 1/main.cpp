#include "header.h"



int main() {
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    while (true) {
        wcin.clear();
        cin.clear();
        wcout << L"Выберите перегрузку (0 - const char * | 1 - string)" << endl;
        int choice;
        if (!(wcin >> choice) or wcin.bad()) {
            if (wcin.eof() or cin.eof()) {
                wcout << L"Выход..." << endl;
                return 0;
            }
            wcin.clear();
           	cin.clear();
            wcout << L"Неверный ввод, попробуйте ещё раз.." << endl;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            wcin.ignore((numeric_limits<streamsize>::max)(), '\n');
            continue;
        }
        
        switch (choice) {
        case 0: {
            wcout << L"Введите ваше слово" << endl;
            wchar_t* word = new wchar_t[100];
            wcin >> word;
            if (wcin.eof() or wcin.bad()) {
                delete[] word;
                wcout << L"Выход..." << endl;
                return 0;
            }
            const wchar_t* wordin = (const wchar_t*)word;
            try {
                const wchar_t* wordout = syllabify(wordin);
                wcout << wordout << endl;
                delete[] word;
                delete[] wordout;
            }
            catch (const exception& ex) {
                delete[] word;
                wcout << L"Произошла ошибка" << ex.what() << endl;
            }
            
            break;
        }
        case 1: {
            wcout << L"Введите ваше слово" << endl;
            wstring wordin;
            wcin >> wordin;
            if (wcin.eof() or wcin.bad()) {
                wcout << L"Выход..." << endl;
                return 0;
            }
            try {
                wstring wordout = syllabify(wordin);
                wcout << wordout << endl;
            }
            catch (const exception& ex) {
                wcout << L"Произошла ошибка" << ex.what() << endl;
            }
            break;
        }
        default: {
            wcout << L"Неверный ввод, попробуйте ещё раз.." << endl;
            break;
        }
        }



    }
    
    return 0;
}


