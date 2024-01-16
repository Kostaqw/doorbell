#include <stdio.h>

#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR   3600
#define SECONDS_IN_DAY    86400
#define SECONDS_IN_YEAR   31536000

// Функция для вычисления даты и времени из количества секунд.
void convertSecondsToDateTime(uint32_t seconds, int *year, int *month, int *day, int *hour, int *minute, int *second) {
    *year = 1970; // Начальный год для UNIX-эпохи.

    // Рассчитываем год.
    while (seconds >= SECONDS_IN_YEAR) {
        (*year)++;
        seconds -= SECONDS_IN_YEAR;
    }

    // Рассчитываем месяц и день.
    int daysInMonth;
    for (*month = 1; *month <= 12; (*month)++) {
        switch (*month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                daysInMonth = 31;
                break;
            case 4: case 6: case 9: case 11:
                daysInMonth = 30;
                break;
            case 2:
                // Проверка на високосный год.
                daysInMonth = ((*year % 4 == 0 && *year % 100 != 0) || (*year % 400 == 0)) ? 29 : 28;
                break;
            default:
                daysInMonth = 0; // Некорректный месяц.
        }

        if (seconds < daysInMonth * SECONDS_IN_DAY)
            break;

        seconds -= daysInMonth * SECONDS_IN_DAY;
    }

    // Рассчитываем день.
    *day = seconds / SECONDS_IN_DAY + 1;
    seconds %= SECONDS_IN_DAY;

    // Рассчитываем часы, минуты и секунды.
    *hour = seconds / SECONDS_IN_HOUR;
    seconds %= SECONDS_IN_HOUR;
    *minute = seconds / SECONDS_IN_MINUTE;
    *second = seconds % SECONDS_IN_MINUTE;
}

int main() {
    uint32_t ntpTime = /* полученные с сервера секунды */;

    // Переменные для хранения результата преобразования.
    int year, month, day, hour, minute, second;

    // Вызываем функцию преобразования времени.
    convertSecondsToDateTime(ntpTime, &year, &month, &day, &hour, &minute, &second);

    // Выводим результат.
    printf("Year: %d, Month: %d, Day: %d, Hour: %d, Minute: %d, Second: %d\n", year, month, day, hour, minute, second);

    return 0;
}
