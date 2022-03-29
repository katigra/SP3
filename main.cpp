#include <iostream>
using namespace std;


void print_line()
{
    for (int i = 0; i < 20; i++)
        cout << '-';
    cout << endl;
}

int main()
{

    srand(time(NULL));

    const int n = 5;
    int matrix[n * n];

    // Инициализация матрирцы
    for (int i = 0; i < n * n; i++)
    {
        matrix[i] = rand() % 10;
    }

    // Вывод матрицы
    cout << "MATRIX " << n << "x" << n <<": " << endl;
    for (int i = 0; i< n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << matrix[j + i * n] << "  "; //j-столбец, i-строка//
        }
        cout << endl;
    }

    print_line();

    // Подсчет суммы элеметнов по диагонали
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j || i + j == (n - 1))
                result += matrix[j + i * n];
        }
    }

    cout << "(Lang C++) Sum = " << result << endl;


    int *ptr_mat = matrix;
    int *ptr_res = &result;

    asm(
            "mov %[ptr_mat], %%rax  \n\t"       // Кидаем адрес начала матрицы в rax
            "mov %[n], %%rbx        \n\t"       // Кидаем размероность матрицы в rbx
            "xor %%rcx, %%rcx       \n\t"       // Обнуляем счетчик первого цикла (i-строки)
            "xor %%rdi, %%rdi       \n\t"       // Обнуляем сумму
    
        "ll:                        \n\t"       // Метка первого цикла (который проходит по строкам)
            "xor %%rsi, %%rsi       \n\t"       // Обнуляем счетчик второго цикла (j-стобцов)

        "pp:                        \n\t"       // Метка второго цикла (проходит по каждому элементу строки
            "cmpl %%esi, %%ecx      \n\t"       // Сравнение i == j
            "je sum                 \n\t"       // Если да, то суммируем

            "movl $1, %%edx         \n\t"       // Все это проверяет i + j == n - 1,
            "addl %%esi, %%edx      \n\t"       // можно это изменить на
            "addl %%ecx, %%edx      \n\t"       // i + j + 1 == n
            "cmpl %%edx, %%ebx      \n\t"
            "je sum                 \n\t"       // Если равны, то прыгаем на сумму

            "jmp end_loop           \n\t"       // Перепрыгиваем sum

        "sum:                       \n\t"       // Суммирование значения ячейки суммой
            "addl (%%rax), %%edi    \n\t"
            
        "end_loop:                  \n\t"
            "inc %%rsi              \n\t"       // Увеличение j на 1 (счетчик второго цикла)
            "add $4, %%rax          \n\t"       // Смещение адреса массива на 1 элемент (int = 4 байта)
            "cmpl %%esi, %%ebx      \n\t"       // Дошел ли j до конца строки
            "jne pp                 \n\t"       // Если нет, то прыгаем в начало второго цикла


            "inc %%rcx              \n\t"       // Увеличение i на 1 (счетчик первого цикла)
            "cmpl %%ebx, %%ecx      \n\t"       // Дошел ли i до последней строки
            "jne ll                 \n\t"       // Если нет, то прыгаем в начало первого цикла

            "mov %[ptr_res], %%rax  \n\t"
            "movl %%edi, (%%rax)    \n\t"       // Кидаем результат суммы в result
            :
            :[ptr_mat]"m"(ptr_mat), [n]"m"(n), [ptr_res]"m"(ptr_res) // Используемые переменные из проги
            :"%rax", "%rbx", "%rcx", "%rdx", "%rsi", "%rdi"     // Используемые регистры во вставке
       );


    cout << "(Lang ASM) Sum = " << result << endl;

    return 0;
}
