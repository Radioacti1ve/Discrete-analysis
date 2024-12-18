#include <iostream>    // Подключаем библиотеку для ввода/вывода
#include <vector>      // Подключаем библиотеку для работы с динамическими массивами (вектор)
#include <climits>     // Подключаем библиотеку для работы с ограничениями типов данных (например, LONG_MAX)
#include <algorithm>   // Подключаем библиотеку для стандартных алгоритмов (например, std::min)

const long INF = LONG_MAX;  // Определяем константу INF как максимальное значение для типа long

// Класс для представления ребра графа
class Edge {
public:
    int from, to;        // Стартовая и конечная вершины ребра
    long long cost;      // Стоимость ребра

    // Конструктор для инициализации ребра
    Edge(int from, int to, long long cost)
        : from(from), to(to), cost(cost) {}
};

// Класс для представления графа
class Graph {
public:
    std::vector<Edge> edges;  // Список всех ребер графа
    int numVertices;          // Число вершин в графе

    // Конструктор графа
    Graph(int numVertices) : numVertices(numVertices + 1) {}

    // Метод для добавления ребра в граф
    void addEdge(int from, int to, long long cost) {
        edges.emplace_back(from, to, cost);  // Добавляем ребро в список ребер
    }
};

// Функция для выполнения алгоритма Беллмана-Форда
std::pair<bool, long long*> BellmanFord(Graph* graph) { // O(V⋅E)
    // Массив для хранения кратчайших расстояний от последней вершины
    auto* distances = new long long[graph->numVertices];
    // Инициализация всех расстояний значением INF
    std::fill(distances, distances + graph->numVertices, INF);
    distances[graph->numVertices - 1] = 0;  // Расстояние до самой последней вершины равно 0

    // Основной цикл алгоритма Беллмана-Форда для обновления расстояний
    for (int i = 0; i < graph->numVertices - 1; ++i) {
        for (const auto& edge : graph->edges) {  // Проходим по всем ребрам
            if (distances[edge.from] < INF) {  // Если вершина "from" доступна
                // Обновляем расстояние до вершины "to" через ребро
                distances[edge.to] = std::min(distances[edge.from] + edge.cost, distances[edge.to]);
            }
        }
    }

    // Проверка на наличие отрицательных циклов
    for (const auto& edge : graph->edges) {
        if (distances[edge.from] < INF && distances[edge.from] + edge.cost < distances[edge.to]) {
            delete[] distances;  // Освобождаем память
            return {false, nullptr};  // Возвращаем ошибку, если найден отрицательный цикл
        }
    }

    // Корректировка стоимости ребер с учетом полученных расстояний
    for (auto& edge : graph->edges) {
        edge.cost = edge.cost + distances[edge.from] - distances[edge.to];
    }

    return {true, distances};  // Возвращаем результат алгоритма
}

// Алгоритм Дейкстры с учетом корректировки стоимостей ребер
void Dijkstra(const Graph* graph, const long long* baseDistances) { // O(V⋅E)+O(V^3 +V⋅E)
    // Для каждой вершины выполняем поиск кратчайшего пути с помощью алгоритма Дейкстры
    for (int startVertex = 0; startVertex < graph->numVertices - 1; ++startVertex) {
        long long distances[graph->numVertices];  // Массив для хранения кратчайших расстояний
        std::fill(distances, distances + graph->numVertices, INF);  // Инициализируем все расстояния как INF
        distances[startVertex] = 0;  // Расстояние до начальной вершины равно 0
        
        bool visited[graph->numVertices] = {false};  // Массив для отслеживания посещенных вершин

        // Основной цикл алгоритма Дейкстры
        for (int i = 0; i < graph->numVertices - 1; ++i) {
            int minVertex = -1;
            // Поиск вершины с минимальным расстоянием
            for (int j = 0; j < graph->numVertices; ++j) {
                if (!visited[j] && (minVertex == -1 || distances[j] < distances[minVertex])) {
                    minVertex = j;
                }
            }

            if (distances[minVertex] == INF) break;  // Прерываем, если все оставшиеся вершины недостижимы
            visited[minVertex] = true;  // Отмечаем вершину как посещенную

            // Обновление расстояний для соседей минимальной вершины
            for (const auto& edge : graph->edges) {
                if (edge.from == minVertex) {
                    distances[edge.to] = std::min(distances[edge.to], distances[edge.from] + edge.cost);
                }
            }
        }

        // Выводим расстояния для всех вершин от текущей начальной вершины
        for (int i = 0; i < graph->numVertices - 1; ++i) {
            if (distances[i] == INF) {
                std::cout << "inf ";  // Если вершина недостижима, выводим "inf"
            } else {
                // Корректируем расстояние с учетом базовых расстояний
                std::cout << distances[i] - baseDistances[startVertex] + baseDistances[i] << ' ';
            }
        }
        std::cout << '\n';  // Переходим на новую строку
    }
}

int main() {
    int numVertices, numEdges;
    std::cin >> numVertices >> numEdges;  // Вводим количество вершин и ребер

    Graph graph(numVertices);  // Создаем граф с заданным количеством вершин

    int u, v, weight;
    for (int i = 0; i < numEdges; ++i) {
        std::cin >> u >> v >> weight;  // Вводим ребра графа
        graph.addEdge(u - 1, v - 1, weight);  // Добавляем ребра в граф, индексы вершин сдвигаем на 1
    }

    // Добавляем фиктивное ребро от последней вершины ко всем остальным с нулевой стоимостью
    for (int i = 0; i < numVertices; ++i) {
        graph.addEdge(numVertices, i, 0);
    }

    auto bfResult = BellmanFord(&graph);  // Запускаем алгоритм Беллмана-Форда

    // Проверяем результат работы алгоритма
    if (!bfResult.first) {
        std::cout << "Negative cycle";  // Если найден отрицательный цикл, выводим сообщение
    } else {
        Dijkstra(&graph, bfResult.second);  // Иначе, запускаем алгоритм Дейкстры с результатами Беллмана-Форда
    }

    return 0;  
}