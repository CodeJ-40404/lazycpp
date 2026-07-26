#define LAZY_EXPORT_ALL
#define LAZY_GLOBAL_EXPORT  // 将 lazy 命名空间也导入全局
#include "lazy.h"

// 所有符号都在全局命名空间
// 只using namespace lazy; 是最好的折中方案

int main() {
    fast_io();          // 以下都是全局可用
    log_info("Hello");
    log_warn("WELCOME TO LAZYCPP!!!!!");
	log_error("ARE U READY?");

    cr ans = input<cr>("y/n?\n");
    if (ans == 'y')
    {
		outl("HOLD ON...");
		wait(1000);
		log_info("Let's go!\n");
    }
    else {
		log_error("Exiting...");
        return 0;
    }
    vi v = { 1, 2, 3 };
    print(v);
    // 如果多个子模块有同名函数，会CE

	//进度条
    ProgressBar pb(100, 50);

    rep (i, 101) {
        pb.update(i);
		wait(50);
    }

    return 0;
}