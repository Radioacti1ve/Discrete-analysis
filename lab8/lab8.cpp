#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

// Максимальное количество элементов
const int MAX_ITEMS = 100;

int main() {
    int itemCount, maxWeight;
    cin >> itemCount >> maxWeight;

    vector<int> weights(itemCount);
    vector<long long> values(itemCount);

    // Ввод данных о весах и ценностях предметов
    for (int i = 0; i < itemCount; ++i) {
        cin >> weights[i] >> values[i];
    }

    // Динамические таблицы для хранения промежуточных результатов
    vector<vector<long long>> dpPrev(itemCount + 1, vector<long long>(maxWeight + 1));
    vector<vector<bitset<MAX_ITEMS>>> selectedPrev(itemCount + 1, vector<bitset<MAX_ITEMS>>(maxWeight + 1));

    long long maxResult = 0;
    bitset<MAX_ITEMS> bestCombination;

    // Основной цикл для вычисления оптимального решения
    for (int i = 1; i <= itemCount; ++i) { // Цикл по предметам (индекс i от 1 до itemCount)
        for (int j = 1; j <= maxWeight; ++j) { // Цикл по возможным весам рюкзака от 1 до maxWeight
            dpPrev[i][j] = dpPrev[i - 1][j]; // Копируем значение из предыдущего шага (если текущий предмет не выбран)
            selectedPrev[i][j] = selectedPrev[i - 1][j]; // Копируем выбранные предметы из предыдущего шага (если текущий предмет не выбран)

            // Проверяем, можно ли добавить текущий предмет в рюкзак
            // Условие: вес текущего предмета меньше или равен текущему весу рюкзака (j >= weights[i - 1]),
            // и если ценность этого предмета больше, чем текущая ценность для данного веса (values[i - 1] > dpPrev[i][j])
            if (j >= weights[i - 1] && values[i - 1] > dpPrev[i][j]) {
                dpPrev[i][j] = values[i - 1]; // Обновляем максимальную ценность для этого веса, если добавление предмета дает большую ценность
                selectedPrev[i][j].reset(); // Сбрасываем все биты в битсете (чтобы начать заново для нового предмета)
                selectedPrev[i][j][i - 1] = 1; // Устанавливаем бит для текущего предмета в 1 (это значит, что он выбран)
            }

            // Если текущая ценность для этого веса больше, чем максимальная найденная ценность,
            // обновляем максимальную ценность и запоминаем комбинацию предметов
            if (dpPrev[i][j] > maxResult) {
                maxResult = dpPrev[i][j]; // Обновляем максимальную ценность
                bestCombination = selectedPrev[i][j]; // Сохраняем лучшую комбинацию выбранных предметов
            }
        }
    }


    vector<vector<long long>> dpCurrent(itemCount + 1, vector<long long>(maxWeight + 1));
    vector<vector<bitset<MAX_ITEMS>>> selectedCurrent(itemCount + 1, vector<bitset<MAX_ITEMS>>(maxWeight + 1));

    // Дополнительные итерации для улучшения результата
    for (long long multiplier = 2; multiplier <= itemCount; ++multiplier) { // Цикл по множителю: начинаем с 2 и продолжаем до количества предметов
        for (int i = 1; i <= itemCount; ++i) { // Цикл по предметам (индекс i от 1 до itemCount)
            for (int j = 1; j <= maxWeight; ++j) { // Цикл по возможным весам рюкзака от 1 до maxWeight
                dpCurrent[i][j] = dpCurrent[i - 1][j]; // Сохраняем значение из предыдущего состояния (не берём текущий предмет)
                selectedCurrent[i][j] = selectedCurrent[i - 1][j]; // Сохраняем выбранные предметы из предыдущего состояния

                // Проверяем, если предмет можно положить в рюкзак (j >= weights[i - 1]) и
                // если при добавлении этого предмета к текущей конфигурации ценность будет больше нуля
                if (j >= weights[i - 1] && dpPrev[i - 1][j - weights[i - 1]] > 0) {
                    // Вычисляем новую ценность, если мы добавляем текущий предмет несколько раз (учитываем multiplier)
                    long long newValue = multiplier * (values[i - 1] + dpPrev[i - 1][j - weights[i - 1]] / (multiplier - 1));
                    
                    // Если новая ценность больше текущей для этого веса, обновляем таблицу
                    if (newValue > dpCurrent[i][j]) {
                        dpCurrent[i][j] = newValue; // Обновляем максимальную ценность для данного веса
                        selectedCurrent[i][j] = selectedPrev[i - 1][j - weights[i - 1]]; // Обновляем выбор предметов
                        selectedCurrent[i][j][i - 1] = 1; // Помечаем, что текущий предмет выбран
                    }
                }

                // Если текущая ценность превышает максимальный результат, обновляем его
                if (dpCurrent[i][j] > maxResult) {
                    maxResult = dpCurrent[i][j]; // Обновляем максимальную найденную ценность
                    bestCombination = selectedCurrent[i][j]; // Сохраняем лучшую комбинацию выбранных предметов
                }
            }
        }

        // После завершения обработки текущего множителя, меняем местами текущие и предыдущие таблицы
        swap(dpCurrent, dpPrev); // Обновляем таблицу dpPrev для следующей итерации
        swap(selectedCurrent, selectedPrev); // Обновляем таблицу selectedPrev для следующей итерации
    }

    // Вывод результата
    cout << maxResult << endl;
    for (int i = 0; i < itemCount; ++i) {
        if (bestCombination[i]) {
            cout << i + 1 << ' ';
        }
    }
    cout << endl;

    return 0;
}