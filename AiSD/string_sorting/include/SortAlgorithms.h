#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace Algorithms {
    // Глобальный счетчик для подсчета количества посимвольных сравнений
    extern size_t char_comp_count;

    // Сброс счетчика сравнений перед каждым тестом
    inline void resetComparisons() {
        char_comp_count = 0;
    }

    // Компаратор для стандартных сортировок (подсчитывает посимвольные сравнения)
    inline bool trackingCompare(const std::string& a, const std::string& b) {
        size_t min_len = std::min(a.length(), b.length());
        for (size_t i = 0; i < min_len; ++i) {
            char_comp_count++; // Фиксируем посимвольное сравнение
            if (a[i] != b[i]) {
                return a[i] < b[i];
            }
        }
        char_comp_count++; // Сравнение длин, если префиксы полностью совпали
        return a.length() < b.length();
    }


    // Стандартный IntroSort (std::sort)
    inline void stdQuickSort(std::vector<std::string>& arr) {
        std::sort(arr.begin(), arr.end(), trackingCompare);
    }

    // Стандартный MergeSort (std::stable_sort)
    inline void stdMergeSort(std::vector<std::string>& arr) {
        std::stable_sort(arr.begin(), arr.end(), trackingCompare);
    }

    // Собственная реализация стандартной быстрой сортировки
    inline void customQuickSortRec(std::vector<std::string>& arr, int low, int high) {
        if (low >= high) return;
        
        // Выбор опорного элемента из середины
        int mid = low + (high - low) / 2;
        std::swap(arr[low], arr[mid]);
        std::string pivot = arr[low];
        
        int lt = low, gt = high;
        int i = low + 1;
        
        // Трехпутевое разделение массива вокруг опорной строки
        while (i <= gt) {
            if (trackingCompare(arr[i], pivot)) {
                std::swap(arr[lt++], arr[i++]);
            } else if (trackingCompare(pivot, arr[i])) {
                std::swap(arr[i], arr[gt--]);
            } else {
                i++;
            }
        }
        
        // Рекурсивный запуск для левой и правой половин
        customQuickSortRec(arr, low, lt - 1);
        customQuickSortRec(arr, gt + 1, high);
    }

    inline void customQuickSort(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        customQuickSortRec(arr, 0, static_cast<int>(arr.size()) - 1);
    }

    // Собственная реализация стандартного слияния (MergeSort)
    inline void customMergeSortRec(std::vector<std::string>& arr, int low, int high, std::vector<std::string>& aux) {
        if (low >= high) return;
        
        int mid = low + (high - low) / 2;
        customMergeSortRec(arr, low, mid, aux);
        customMergeSortRec(arr, mid + 1, high, aux);
        
        // Копируем во вспомогательный буфер
        for (int i = low; i <= high; ++i) {
            aux[i] = std::move(arr[i]);
        }
        
        // Слияние двух отсортированных частей
        int i1 = low, i2 = mid + 1, j = low;
        while (i1 <= mid && i2 <= high) {
            if (trackingCompare(aux[i1], aux[i2])) {
                arr[j++] = std::move(aux[i1++]);
            } else {
                arr[j++] = std::move(aux[i2++]);
            }
        }
        while (i1 <= mid) {
            arr[j++] = std::move(aux[i1++]);
        }
        while (i2 <= high) {
            arr[j++] = std::move(aux[i2++]);
        }
    }

    inline void customMergeSort(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        std::vector<std::string> aux(arr.size());
        customMergeSortRec(arr, 0, static_cast<int>(arr.size()) - 1, aux);
    }


    // Тернарная быстрая сортировка строк
    inline void ternaryStringQuickSortRec(std::vector<std::string>& arr, int low, int high, int d) {
        if (high <= low) return;
        
        // Выбор опорного элемента из середины
        int mid = low + (high - low) / 2;
        std::swap(arr[low], arr[mid]);
        
        // Символ опорной строки на глубине d (или -1, если строка закончилась)
        int pivot_char = (d < static_cast<int>(arr[low].length())) ? static_cast<unsigned char>(arr[low][d]) : -1;
        
        int lt = low, gt = high;
        int i = low + 1;
        
        // Группируем строки на три группы по d-му символу
        while (i <= gt) {
            int c = (d < static_cast<int>(arr[i].length())) ? static_cast<unsigned char>(arr[i][d]) : -1;
            char_comp_count++; // Фиксируем посимвольное сравнение
            if (c < pivot_char) {
                std::swap(arr[lt++], arr[i++]);
            } else if (c > pivot_char) {
                std::swap(arr[i], arr[gt--]);
            } else {
                i++;
            }
        }
        
        // Рекурсивно сортируем группы
        ternaryStringQuickSortRec(arr, low, lt - 1, d);
        if (pivot_char >= 0) {
            ternaryStringQuickSortRec(arr, lt, gt, d + 1); // Увеличиваем глубину для равных символов
        }
        ternaryStringQuickSortRec(arr, gt + 1, high, d);
    }

    inline void ternaryStringQuickSort(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        ternaryStringQuickSortRec(arr, 0, static_cast<int>(arr.size()) - 1, 0);
    }

    // Сортировка слиянием строк с кэшированием длин наибольших общих префиксов (LCP)
    inline void stringMergeSortRec(std::vector<std::string>& arr, std::vector<int>& lcp, int low, int high, 
                                  std::vector<std::string>& aux_arr, std::vector<int>& aux_lcp) {
        if (high <= low) {
            if (high == low) {
                lcp[low] = 0; // LCP для единственного элемента равен 0
            }
            return;
        }
        
        int mid = low + (high - low) / 2;
        stringMergeSortRec(arr, lcp, low, mid, aux_arr, aux_lcp);
        stringMergeSortRec(arr, lcp, mid + 1, high, aux_arr, aux_lcp);
        
        // Копируем обе половины во вспомогательный буфер
        for (int i = low; i <= high; ++i) {
            aux_arr[i] = std::move(arr[i]);
            aux_lcp[i] = lcp[i];
        }
        
        int i1 = low, i2 = mid + 1, j = low;
        int h1 = 0, h2 = 0; // Накопленные префиксы с предыдущим вставленным элементом
        
        while (i1 <= mid && i2 <= high) {
            if (h1 > h2) {
                // Левый элемент гарантированно меньше, используем известное h1 без сравнений
                arr[j] = std::move(aux_arr[i1]);
                lcp[j] = h1;
                j++; i1++;
                h1 = (i1 <= mid) ? aux_lcp[i1] : 0;
            } else if (h2 > h1) {
                // Правый элемент гарантированно меньше, используем известное h2 без сравнений
                arr[j] = std::move(aux_arr[i2]);
                lcp[j] = h2;
                j++; i2++;
                h2 = (i2 <= high) ? aux_lcp[i2] : 0;
            } else {
                // Сравниваем посимвольно, начиная с накопленной общей глубины h1
                int k = h1;
                bool s1_less = false;
                while (k < static_cast<int>(aux_arr[i1].length()) && k < static_cast<int>(aux_arr[i2].length())) {
                    char_comp_count++;
                    if (aux_arr[i1][k] != aux_arr[i2][k]) {
                        s1_less = aux_arr[i1][k] < aux_arr[i2][k];
                        break;
                    }
                    k++;
                }
                if (k == static_cast<int>(aux_arr[i1].length()) || k == static_cast<int>(aux_arr[i2].length())) {
                    if (aux_arr[i1].length() == aux_arr[i2].length()) {
                        s1_less = true;
                    } else {
                        char_comp_count++;
                        s1_less = aux_arr[i1].length() < aux_arr[i2].length();
                    }
                }
                
                int lcp_val = k; // Новое вычисленное LCP
                
                if (s1_less) {
                    arr[j] = std::move(aux_arr[i1]);
                    lcp[j] = h1;
                    j++; i1++;
                    h1 = (i1 <= mid) ? aux_lcp[i1] : 0;
                    h2 = lcp_val;
                } else {
                    arr[j] = std::move(aux_arr[i2]);
                    lcp[j] = h2;
                    j++; i2++;
                    h2 = (i2 <= high) ? aux_lcp[i2] : 0;
                    h1 = lcp_val;
                }
            }
        }
        
        // Копируем остатки
        while (i1 <= mid) {
            arr[j] = std::move(aux_arr[i1]);
            lcp[j] = h1;
            j++; i1++;
            h1 = (i1 <= mid) ? aux_lcp[i1] : 0;
        }
        while (i2 <= high) {
            arr[j] = std::move(aux_arr[i2]);
            lcp[j] = h2;
            j++; i2++;
            h2 = (i2 <= high) ? aux_lcp[i2] : 0;
        }
    }

    inline void stringMergeSort(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        std::vector<int> lcp(arr.size(), 0);
        std::vector<std::string> aux_arr(arr.size());
        std::vector<int> aux_lcp(arr.size());
        stringMergeSortRec(arr, lcp, 0, static_cast<int>(arr.size()) - 1, aux_arr, aux_lcp);
    }

    // Поразрядная MSD сортировка (MSD Radix Sort без переключения)
    inline void msdRadixSortRec(std::vector<std::string>& arr, int low, int high, int d, std::vector<std::string>& aux) {
        if (high <= low) return;
        
        const int R = 257; // 256 символов ASCII + 1 терминальный символ
        std::vector<int> count(R + 2, 0);
        
        // Считаем частоты символов
        for (int i = low; i <= high; ++i) {
            int c = (d < static_cast<int>(arr[i].length())) ? (static_cast<unsigned char>(arr[i][d]) + 1) : 0;
            count[c + 2]++;
        }
        
        // Считаем префиксные суммы
        for (int r = 0; r < R + 1; ++r) {
            count[r + 1] += count[r];
        }
        
        // Распределяем элементы по корзинам
        for (int i = low; i <= high; ++i) {
            int c = (d < static_cast<int>(arr[i].length())) ? (static_cast<unsigned char>(arr[i][d]) + 1) : 0;
            aux[count[c + 1]++] = std::move(arr[i]);
        }
        
        // Копируем обратно в массив arr
        for (int i = low; i <= high; ++i) {
            arr[i] = std::move(aux[i - low]);
        }
        
        // Рекурсивно сортируем каждую непустую корзину (кроме корзины 0)
        for (int r = 1; r < R; ++r) {
            int bucket_low = low + count[r];
            int bucket_high = low + count[r + 1] - 1;
            msdRadixSortRec(arr, bucket_low, bucket_high, d + 1, aux);
        }
    }

    inline void msdRadixSort(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        std::vector<std::string> aux(arr.size());
        msdRadixSortRec(arr, 0, static_cast<int>(arr.size()) - 1, 0, aux);
    }

    // Гибридная MSD сортировка с переходом на Ternary QuickSort для небольших подмассивов (< 74)
    inline void msdRadixSortFallbackRec(std::vector<std::string>& arr, int low, int high, int d, std::vector<std::string>& aux) {
        if (high <= low) return;
        
        // Если размер поддиапазона меньше мощности исходного алфавита (74) - переключаемся на тернарный поиск
        if (high - low + 1 < 74) {
            ternaryStringQuickSortRec(arr, low, high, d);
            return;
        }
        
        const int R = 257;
        std::vector<int> count(R + 2, 0);
        
        // Подсчет частот
        for (int i = low; i <= high; ++i) {
            int c = (d < static_cast<int>(arr[i].length())) ? (static_cast<unsigned char>(arr[i][d]) + 1) : 0;
            count[c + 2]++;
        }
        
        // Префиксные суммы
        for (int r = 0; r < R + 1; ++r) {
            count[r + 1] += count[r];
        }
        
        // Распределение по корзинам
        for (int i = low; i <= high; ++i) {
            int c = (d < static_cast<int>(arr[i].length())) ? (static_cast<unsigned char>(arr[i][d]) + 1) : 0;
            aux[count[c + 1]++] = std::move(arr[i]);
        }
        
        // Копируем обратно
        for (int i = low; i <= high; ++i) {
            arr[i] = std::move(aux[i - low]);
        }
        
        // Рекурсивный запуск для корзин
        for (int r = 1; r < R; ++r) {
            int bucket_low = low + count[r];
            int bucket_high = low + count[r + 1] - 1;
            msdRadixSortFallbackRec(arr, bucket_low, bucket_high, d + 1, aux);
        }
    }

    inline void msdRadixSortFallback(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        std::vector<std::string> aux(arr.size());
        msdRadixSortFallbackRec(arr, 0, static_cast<int>(arr.size()) - 1, 0, aux);
    }
}
