// teaching_demo.cpp
// lazy_teaching 功能演示 - 带循环菜单
// 编译: g++ -std=c++17 -O2 teaching_demo.cpp -o teaching_demo

#define LAZY_EXPORT_ALL
#include "lazy.h"

using namespace lazy;

// ============================================================
// 工具函数
// ============================================================
std::vector<int> make_random_array(int size, int min = 0, int max = 100) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = random::rand_int(min, max);
    }
    return arr;
}

void wait_for_enter() {
    std::cout << "\n按 Enter 键返回主菜单...";
    std::cin.ignore(1000, '\n');
    std::cin.get();
}

// ============================================================
// 演示 1: 排序可视化
// ============================================================
void demo_sorting() {
    teaching::clear_screen();
    teaching::print_title("=== 排序算法可视化演示 ===", '=');

    auto arr = make_random_array(8, 1, 50);

    std::cout << "\n原始数组: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    // 冒泡排序
    std::cout << "--- 冒泡排序 ---\n";
    {
        auto data = arr;
        teaching::SortVisualizer<int> visualizer(data);
        visualizer.enable_animation(true, 300);
        visualizer.bubble_sort();
        std::cout << "步骤总数: " << visualizer.total_steps() << "\n\n";
    }

    // 选择排序
    std::cout << "--- 选择排序 ---\n";
    {
        auto data = arr;
        teaching::SortVisualizer<int> visualizer(data);
        visualizer.enable_animation(true, 300);
        visualizer.selection_sort();
        std::cout << "步骤总数: " << visualizer.total_steps() << "\n\n";
    }

    // 插入排序
    std::cout << "--- 插入排序 ---\n";
    {
        auto data = arr;
        teaching::SortVisualizer<int> visualizer(data);
        visualizer.enable_animation(true, 300);
        visualizer.insertion_sort();
        std::cout << "步骤总数: " << visualizer.total_steps() << "\n\n";
    }

    wait_for_enter();
}

// ============================================================
// 演示 2: 二分查找可视化
// ============================================================
void demo_binary_search() {
    teaching::clear_screen();
    teaching::print_title("=== 二分查找可视化演示 ===", '=');

    auto arr = make_random_array(10, 1, 50);
    std::sort(arr.begin(), arr.end());

    int target = arr[random::rand_int(0, (int)arr.size() - 1)];

    std::cout << "\n查找目标: " << target << "\n";
    std::cout << "有序数组: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    teaching::BinarySearchVisualizer<int> searcher(arr);
    searcher.enable_animation(true, 500);
    int result = searcher.search(target);

    if (result != -1) {
        std::cout << "✅ 找到! 位置: " << result << "\n";
    }
    else {
        std::cout << "❌ 未找到\n";
    }

    wait_for_enter();
}

// ============================================================
// 演示 3: 栈操作可视化
// ============================================================
void demo_stack() {
    teaching::clear_screen();
    teaching::print_title("=== 栈操作可视化演示 ===", '=');

    teaching::StackDemo<int> stack;
    stack.enable_animation(true, 400);

    std::cout << "\n执行 push 操作:\n";
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.push(40);

    std::cout << "\n执行 pop 操作:\n";
    stack.pop();
    stack.pop();

    std::cout << "\n查看栈顶:\n";
    stack.top();

    wait_for_enter();
}

// ============================================================
// 演示 4: 队列操作可视化
// ============================================================
void demo_queue() {
    teaching::clear_screen();
    teaching::print_title("=== 队列操作可视化演示 ===", '=');

    teaching::QueueDemo<int> queue;
    queue.enable_animation(true, 400);

    std::cout << "\n执行 enqueue 操作:\n";
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    queue.enqueue(40);

    std::cout << "\n执行 dequeue 操作:\n";
    queue.dequeue();
    queue.dequeue();

    std::cout << "\n查看队首和队尾:\n";
    queue.front();

    wait_for_enter();
}

// ============================================================
// 演示 5: 递归可视化 - 斐波那契
// ============================================================
void demo_recursion() {
    teaching::clear_screen();
    teaching::print_title("=== 递归可视化演示 - 斐波那契 ===", '=');

    teaching::RecursionVisualizer tracer;
    tracer.enable(true);
    tracer.enable_animation(true, 350);

    std::cout << "\n计算 fib(6) 的递归过程:\n\n";

    std::function<int(int)> fib = [&](int n) -> int {
        if (n <= 1) return n;
        return fib(n - 1) + fib(n - 2);
        };

    int result = tracer.trace(fib, "fib", 6);
    std::cout << "\n结果: fib(6) = " << result << "\n\n";

    wait_for_enter();
}

// ============================================================
// 演示 6: 斐波那契可视化（带缓存对比）
// ============================================================
void demo_fibonacci() {
    teaching::clear_screen();
    teaching::print_title("=== 斐波那契对比演示 ===", '=');

    teaching::FibonacciVisualizer fib_vis;
    fib_vis.enable_animation(true, 350);

    std::cout << "\n--- 递归版本 (fib 6) ---\n";
    std::cout << "结果: " << fib_vis.fib_recursive(6) << "\n\n";

    std::cout << "--- 带记忆化版本 (fib 6) ---\n";
    std::cout << "结果: " << fib_vis.fib_memo(6) << "\n\n";

    std::cout << "--- 递归调用树 (fib 5) ---\n";
    fib_vis.print_tree(5);
    std::cout << "\n";

    wait_for_enter();
}

// ============================================================
// 演示 7: 链表可视化
// ============================================================
void demo_linked_list() {
    teaching::clear_screen();
    teaching::print_title("=== 链表可视化演示 ===", '=');

    teaching::LinkedListVisualizer<int> list;
    list.enable_animation(true, 400);

    std::cout << "\n构建链表:\n";
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);

    std::cout << "\n在头部插入:\n";
    list.push_front(5);

    std::cout << "\n反转链表:\n";
    list.reverse_with_visualization();

    wait_for_enter();
}

// ============================================================
// 演示 8: 步骤追踪器
// ============================================================
void demo_step_tracker() {
    teaching::clear_screen();
    teaching::print_title("=== 步骤追踪器演示 ===", '=');

    teaching::StepTracer tracer;
    tracer.enable_verbose(true);
    tracer.enable_animation(true, 250);

    std::cout << "\n模拟算法步骤:\n\n";

    tracer.step("初始化数组 [5, 3, 8, 1, 9]");
    tracer.step("第1轮: 比较 5 和 3, 交换");
    tracer.step("第1轮: 比较 5 和 8, 不变");
    tracer.step("第1轮: 比较 8 和 1, 交换");
    tracer.step("第1轮: 比较 8 和 9, 不变");
    tracer.step("第2轮: 比较 3 和 5, 不变");
    tracer.step("排序完成!");

    tracer.print_report();

    wait_for_enter();
}

// ============================================================
// 演示 9: 交互式教学工具
// ============================================================
void demo_interactive_tutor() {
    teaching::clear_screen();
    teaching::print_title("=== 交互式教学工具演示 ===", '=');

    teaching::InteractiveTutor tutor;
    tutor.enable_animation(true);

    tutor.record_state("开始: 数组 [5, 2, 8, 1, 9]");
    tutor.record_state("步骤1: 选择排序 - 找到最小值 1");
    tutor.record_state("步骤2: 交换 5 和 1 -> [1, 2, 8, 5, 9]");
    tutor.record_state("步骤3: 找到最小值 2 (已就位)");
    tutor.record_state("步骤4: 找到最小值 5 -> [1, 2, 5, 8, 9]");
    tutor.record_state("步骤5: 找到最小值 8 -> [1, 2, 5, 8, 9]");
    tutor.record_state("完成! 数组已排序");

    std::cout << "\n输入 'help' 查看命令, 'quit' 退出\n";
    tutor.run();

    wait_for_enter();
}

// ============================================================
// 演示 10: 算法性能比较
// ============================================================
void demo_algorithm_comparison() {
    teaching::clear_screen();
    teaching::print_title("=== 算法性能比较演示 ===", '=');

    teaching::AlgorithmComparator comparator;
    comparator.enable_verbose(true);
    comparator.test_sizes({ 100, 500, 1000 });

    comparator.add_algorithm("Bubble Sort", []() {
        auto arr = make_random_array(100);
        for (size_t i = 0; i < arr.size(); i++) {
            for (size_t j = 0; j < arr.size() - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
        });

    comparator.add_algorithm("std::sort", []() {
        auto arr = make_random_array(100);
        std::sort(arr.begin(), arr.end());
        });

    comparator.compare(3);

    wait_for_enter();
}

// ============================================================
// 演示 11: 中文语言支持
// ============================================================
void demo_chinese() {
    teaching::clear_screen();
    teaching::print_title("=== 中文语言支持演示 ===", '=');

    std::cout << "\n当前语言: "
        << (teaching::LanguageManager::get_language() == teaching::Language::ENGLISH ? "English" : "中文")
        << "\n\n";

    teaching::set_language_chinese();
    std::cout << "切换到: 中文\n\n";

    std::cout << "排序相关:\n";
    std::cout << "  " << teaching::LanguageManager::get("sort_start") << "\n";
    std::cout << "  " << teaching::LanguageManager::get("sort_sorted") << "\n";
    std::cout << "  " << teaching::LanguageManager::get("sort_comparing") << "\n";

    std::cout << "\n搜索相关:\n";
    std::cout << "  " << teaching::LanguageManager::get("search_start") << "\n";
    std::cout << "  " << teaching::LanguageManager::get("search_found") << "\n";

    std::cout << "\n数据结构:\n";
    std::cout << "  " << teaching::LanguageManager::get("stack_top") << "\n";
    std::cout << "  " << teaching::LanguageManager::get("queue_front") << "\n";

    wait_for_enter();
}

// ============================================================
// 演示 12: 数组可视化工具
// ============================================================
void demo_array_visualizer() {
    teaching::clear_screen();
    teaching::print_title("=== 数组可视化工具演示 ===", '=');

    std::vector<int> data = { 12, 7, 3, 18, 5, 9, 14, 1 };

    teaching::VisualConfig config;
    config.bar_width = 30;
    config.fill_char = '#';
    config.empty_char = '.';
    config.show_index = true;
    config.show_value = true;

    teaching::ArrayVisualizer<int> visualizer(data, config);

    std::cout << "\n原始数组:\n";
    visualizer.print();

    std::cout << "条形图视图:\n";
    visualizer.print_bars();

    std::cout << "高亮显示索引 2, 5:\n";
    visualizer.highlight({ 2, 5 });
    visualizer.print();

    std::cout << "比较索引 3 和 6:\n";
    visualizer.compare(3, 6);
    visualizer.print();

    wait_for_enter();
}

// ============================================================
// 演示 13: 快速排序可视化 (新增)
// ============================================================
void demo_quick_sort() {
    teaching::clear_screen();
    teaching::print_title("=== 快速排序可视化演示 ===", '=');

    auto arr = make_random_array(8, 1, 50);

    std::cout << "\n原始数组: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n\n";

    teaching::SortVisualizer<int> visualizer(arr);
    visualizer.enable_animation(true, 400);
    visualizer.quick_sort();
    std::cout << "步骤总数: " << visualizer.total_steps() << "\n\n";

    wait_for_enter();
}

// ============================================================
// 演示 14: 查找所有演示 (新增)
// ============================================================
void demo_find_all() {
    teaching::clear_screen();
    teaching::print_title("=== 查找算法演示 ===", '=');

    auto arr = make_random_array(12, 1, 30);
    std::sort(arr.begin(), arr.end());

    int target = arr[random::rand_int(0, (int)arr.size() - 1)];
    // 确保目标出现多次
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == target) {
            arr[i + random::rand_int(0, 2)] = target;
        }
    }
    std::sort(arr.begin(), arr.end());

    std::cout << "\n数组: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n查找目标: " << target << "\n\n";

    // 使用 algorithm 模块的查找功能
    auto positions = algorithm::find_all(arr, target);
    std::cout << "找到 " << positions.size() << " 个位置: ";
    for (int pos : positions) std::cout << pos << " ";
    std::cout << "\n";

    auto first = algorithm::first_equal(arr, target);
    auto last = algorithm::last_equal(arr, target);
    std::cout << "第一次出现: " << first << ", 最后一次出现: " << last << "\n";

    wait_for_enter();
}

// ============================================================
// 主菜单
// ============================================================
void show_menu() {
    teaching::clear_screen();

    std::cout << "\n";
    teaching::print_title("╔══════════════════════════════════════════╗", '═');
    std::cout << "║     LAZY_TEACHING 交互式演示程序            ║\n";
    std::cout << "║     版权所有 2026 lazycpp MIT License       ║\n";
    teaching::print_title("╚══════════════════════════════════════════╝", '═');

    std::cout << "\n";
    std::cout << "  ┌────────────────────────────────────────────────┐\n";
    std::cout << "  │     排序算法                                │\n";
    std::cout << "  │   1. 冒泡/选择/插入排序可视化               │\n";
    std::cout << "  │  13. 快速排序可视化                         │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     查找算法                                │\n";
    std::cout << "  │   2. 二分查找可视化                         │\n";
    std::cout << "  │  14. 查找所有出现位置                       │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     数据结构                                │\n";
    std::cout << "  │   3. 栈 (Stack) 操作可视化                  │\n";
    std::cout << "  │   4. 队列 (Queue) 操作可视化                │\n";
    std::cout << "  │   7. 链表 (LinkedList) 可视化               │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     递归与算法分析                         │\n";
    std::cout << "  │   5. 递归调用可视化                        │\n";
    std::cout << "  │   6. 斐波那契对比 (递归 vs 记忆化)         │\n";
    std::cout << "  │   8. 步骤追踪器                            │\n";
    std::cout << "  │  10. 算法性能比较                          │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     交互式工具                             │\n";
    std::cout << "  │   9. 交互式教学工具                        │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     其他功能                               │\n";
    std::cout << "  │  11. 中文语言支持                          │\n";
    std::cout << "  │  12. 数组可视化工具                        │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     全部运行                              │\n";
    std::cout << "  │   0. 运行所有演示                         │\n";
    std::cout << "  ├────────────────────────────────────────────────┤\n";
    std::cout << "  │     退出                                  │\n";
    std::cout << "  │   66. 退出程序                            │\n";
    std::cout << "  └────────────────────────────────────────────────┘\n";

    std::cout << "\n请输入选择 [1-14, 0, 66]: ";
}

// ============================================================
// 运行所有演示
// ============================================================
void run_all_demos() {
    std::vector<std::pair<std::string, std::function<void()>>> demos = {
        {"排序可视化", demo_sorting},
        {"二分查找可视化", demo_binary_search},
        {"栈操作可视化", demo_stack},
        {"队列操作可视化", demo_queue},
        {"递归可视化", demo_recursion},
        {"斐波那契对比", demo_fibonacci},
        {"链表可视化", demo_linked_list},
        {"步骤追踪器", demo_step_tracker},
        {"交互式教学工具", demo_interactive_tutor},
        {"算法性能比较", demo_algorithm_comparison},
        {"中文语言支持", demo_chinese},
        {"数组可视化工具", demo_array_visualizer},
        {"快速排序可视化", demo_quick_sort},
        {"查找所有出现位置", demo_find_all}
    };

    for (size_t i = 0; i < demos.size(); i++) {
        std::cout << "\n▶ 运行 [" << (i + 1) << "/" << demos.size() << "] "
            << demos[i].first << " ...\n";
        teaching::sleep_ms(1000);
        demos[i].second();
    }
}

// ============================================================
// 主函数 - 循环菜单
// ============================================================
int main() {
    // 初始化
    lazy::debug::init("teaching_demo.log");
    lazy::io::fast_io();

    // 设置语言
    teaching::set_language_english();

    // 注册信号处理，确保程序退出时清理
    std::atexit([]() {
        lazy::debug::log_info("Teaching demo exited");
        });

    int choice;

    while (true) {
        show_menu();
        std::cin >> choice;
        std::cin.ignore(1000, '\n');


        // 运行选中的演示
        switch (choice) {
        case 1: demo_sorting(); break;
        case 2: demo_binary_search(); break;
        case 3: demo_stack(); break;
        case 4: demo_queue(); break;
        case 5: demo_recursion(); break;
        case 6: demo_fibonacci(); break;
        case 7: demo_linked_list(); break;
        case 8: demo_step_tracker(); break;
        case 9: demo_interactive_tutor(); break;
        case 10: demo_algorithm_comparison(); break;
        case 11: demo_chinese(); break;
        case 12: demo_array_visualizer(); break;
        case 13: demo_quick_sort(); break;
        case 14: demo_find_all(); break;
        case 0:
            run_all_demos();
            break;
        case 66:
			std::cout << "\n感谢使用 lazy_teaching 演示程序!\n";
			return 0;
        default:
            std::cout << "\n⚠️  无效选择: " << choice << "\n";
            std::cout << "按 Enter 继续...";
            std::cin.get();
            break;
        }
    }

    return 0;
}