#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <windows.h>
#include <locale>

using namespace std;

void setupLocalization()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    setlocale(LC_ALL, "ru_RU.UTF-8");

    setlocale(LC_ALL, "Russian_Russia.65001");

    try
    {
        locale::global(locale("ru_RU.UTF-8"));
        cout.imbue(locale());
    }
    catch (...)
    {
        try
        {
            locale::global(locale("Russian_Russia.65001"));
            cout.imbue(locale());
        }
        catch (...)
        {
            locale::global(locale(""));
        }
    }
}
vector<unsigned char> readBinaryFile(const string &filename)
{
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open())
    {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        throw runtime_error("Ошибка чтения файла");
    }

    return buffer;
}

void printBinaryData(const vector<unsigned char> &data, size_t maxBytes = 256)
{
    cout << "\n========== БИНАРНЫЕ ДАННЫЕ (HEX) ==========\n";
    cout << "Размер файла: " << data.size() << " байт\n";
    cout << "Первые " << min(maxBytes, data.size()) << " байт:\n\n";

    for (size_t i = 0; i < min(maxBytes, data.size()); ++i)
    {
        cout << hex << setw(2) << setfill('0') << (int)data[i] << " ";
        if ((i + 1) % 16 == 0)
            cout << "\n";
    }
    cout << dec << "\n";
}

string hash_fnv1a(const vector<unsigned char> &data)
{
    const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
    const uint64_t FNV_PRIME = 1099511628211ULL;

    uint64_t hash = FNV_OFFSET_BASIS;

    for (size_t i = 0; i < data.size(); ++i)
    {
        hash ^= data[i];
        hash *= FNV_PRIME;
    }

    ostringstream oss;
    oss << hex << setw(16) << setfill('0') << hash;
    return oss.str();
}
string hash_jenkins(const vector<unsigned char> &data)
{
    uint32_t hash = 0;

    for (size_t i = 0; i < data.size(); ++i)
    {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    ostringstream oss;
    oss << hex << setw(8) << setfill('0') << hash;
    return oss.str();
}

int main()
{
    setupLocalization();

    string filename;
    cout << "Введите путь к файлу (.mp4, .mp3, .exe, любой): ";
    getline(cin, filename);

    try
    {
        cout << "\n[1] Чтение бинарного файла...\n";
        vector<unsigned char> fileData = readBinaryFile(filename);
        cout << "✓ Файл успешно загружен!\n";

        cout << "\n[2] Вывод бинарных данных в HEX формате:\n";
        printBinaryData(fileData);

        cout << "\n[3] Хеширование данных:\n";
        cout << "==========================================\n\n";

        cout << "Алгоритм 1: FNV-1a (64-bit)\n";
        cout << "----------------------------\n";
        string fnvHash = hash_fnv1a(fileData);
        cout << "FNV-1a Hash: " << fnvHash << "\n";
        cout << "Длина хеша: " << fnvHash.length() / 2 << " байт (" << fnvHash.length() << " символов)\n\n";

        cout << "Алгоритм 2: Jenkins One-at-a-Time (32-bit)\n";
        cout << "-------------------------------------------\n";
        string jenkinsHash = hash_jenkins(fileData);
        cout << "Jenkins Hash: " << jenkinsHash << "\n";
        cout << "Длина хеша: " << jenkinsHash.length() / 2 << " байт (" << jenkinsHash.length() << " символов)\n\n";

        cout << "==========================================\n";
        cout << "\n✓ Обработка завершена успешно!\n";
    }
    catch (const exception &e)
    {
        cerr << "\n✗ ОШИБКА: " << e.what() << "\n";
        return 1;
    }

    cout << "\nНажмите Enter для выхода...";
    cin.get();
    return 0;
}
