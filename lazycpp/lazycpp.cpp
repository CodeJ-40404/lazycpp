#include "lazy.h"
using namespace lazy;
#define LAZY_EXPORT_ALL

int main() {
    
    char choice;
    outl("选择语言/Choose Language, c/e");
    std::cin >> choice;
    if (choice == 'c' || choice == 'C') {
        set_language_chinese();
    } else {
        set_language_english();
    }

    vec_int arr = rand_int_array(10,1,100);
    SortVisualizer<int> sorter(arr);
    sorter.enable_animation(true, 1000);
	sorter.insertion_sort();
	sorter.print_steps();
	sorter.total_steps();
    outl("\n");
    return 0;
}