// example.cpp
#include "lazy.h"

using namespace lazy::json;

int main() {
    std::cout << "========== JSON 库使用示例 ==========\n\n";

    // ============================================================
    // 1. 创建 JSON 对象
    // ============================================================
    std::cout << "1. 创建 JSON 对象:\n";

    Json user = {
        {"name", "张三"},
        {"age", 25},
        {"active", true},
        {"scores", Json::array()}  // 空数组
    };

    // 添加数组元素
    user["scores"].push_back(95);
    user["scores"].push_back(87);
    user["scores"].push_back(92);

    // 嵌套对象
    user["address"] = Json{
        {"city", "北京"},
        {"district", "海淀"}
    };

    std::cout << user.dump(true) << "\n\n";

    // ============================================================
    // 2. 访问 JSON 数据
    // ============================================================
    std::cout << "2. 访问数据:\n";

    std::string name = user["name"].as_string();
    int age = user["age"].as_int();
    bool active = user["active"].as_bool();
    double first_score = user["scores"][0].as_number();
    std::string city = user["address"]["city"].as_string();

    std::cout << "姓名: " << name << "\n";
    std::cout << "年龄: " << age << "\n";
    std::cout << "活跃: " << (active ? "是" : "否") << "\n";
    std::cout << "第一科成绩: " << first_score << "\n";
    std::cout << "城市: " << city << "\n\n";

    // ============================================================
    // 3. 安全访问
    // ============================================================
    std::cout << "3. 安全访问:\n";

    // 如果键不存在，返回默认值
    std::string email = user["email"].get_string("未设置");
    bool admin = user["admin"].get_bool(false);

    std::cout << "邮箱: " << email << "\n";
    std::cout << "管理员: " << (admin ? "是" : "否") << "\n\n";

    // 使用 find() 检查是否存在
    auto found = user.find("address");
    if (found) {
        std::cout << "address 字段存在\n";
        std::cout << "城市: " << found->operator[]("city").as_string() << "\n";
    }
    std::cout << "\n";

    // ============================================================
    // 4. 遍历数组和对象
    // ============================================================
    std::cout << "4. 遍历数据:\n";

    // 遍历成绩数组
    std::cout << "成绩列表: ";
    for (const auto& score : user["scores"]) {
        std::cout << score.as_int() << " ";
    }
    std::cout << "\n";

    // 遍历对象的所有键值对
    std::cout << "用户信息:\n";
    for (const auto& [key, value] : user.as_object()) {
        std::cout << "  " << key << ": ";
        if (value.is_string()) {
            std::cout << value.as_string();
        }
        else if (value.is_number()) {
            std::cout << value.as_number();
        }
        else if (value.is_bool()) {
            std::cout << (value.as_bool() ? "true" : "false");
        }
        else if (value.is_array()) {
            std::cout << "[数组, 大小=" << value.size() << "]";
        }
        else if (value.is_object()) {
            std::cout << "{对象, 大小=" << value.size() << "}";
        }
        else {
            std::cout << "null";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    // ============================================================
    // 5. 修改 JSON 数据
    // ============================================================
    std::cout << "5. 修改数据:\n";

    user["age"] = 26;  // 修改
    user["scores"].push_back(88);  // 添加
    user["email"] = "zhangsan@example.com";  // 新增

    std::cout << user.dump(true) << "\n\n";

    // ============================================================
    // 6. 解析 JSON 字符串
    // ============================================================
    std::cout << "6. 解析 JSON 字符串:\n";

    std::string json_str = R"({
        "product": "手机",
        "price": 5999.99,
        "specs": ["6.1英寸", "128GB", "双卡"]
    })";

    Json product = Json::parse(json_str);
    std::cout << "产品: " << product["product"].as_string() << "\n";
    std::cout << "价格: " << product["price"].as_number() << "\n";
    std::cout << "规格: " << product["specs"][0].as_string() << "\n\n";

    // ============================================================
    // 7. 文件操作
    // ============================================================
    std::cout << "7. 文件操作:\n";

    // 保存到文件
    user.save("user_data.json", true);  // pretty = true, indent = 2
    std::cout << "√ 已保存到 user_data.json\n";

    // 读取文件
    if (JsonFile::file_exists("user_data.json")) {
        Json loaded = Json::load("user_data.json");
        std::cout << "√ 从文件读取成功\n";
        std::cout << "姓名: " << loaded["name"].as_string() << "\n";
    }
    std::cout << "\n";

    // ============================================================
    // 8. 容器转换
    // ============================================================
    std::cout << "8. 容器转换:\n";

    // vector -> JSON
    std::vector<int> numbers = { 10, 20, 30, 40, 50 };
    Json num_array = vector_to_json(numbers);
    std::cout << "数组转JSON: " << num_array.dump() << "\n";

    // JSON -> vector
    std::vector<int> back = json_to_vector<int>(num_array);
    std::cout << "JSON转数组: ";
    for (int n : back) {
        std::cout << n << " ";
    }
    std::cout << "\n\n";

    // ============================================================
    // 9. 紧凑输出 vs 美化输出
    // ============================================================
    std::cout << "9. 输出格式对比:\n";
    std::cout << "美化输出:\n" << user.dump(true) << "\n";
    std::cout << "紧凑输出:\n" << user.dump(false) << "\n\n";

    // ============================================================
    // 10. 使用 JsonFile 进行更复杂的文件操作
    // ============================================================
    std::cout << "10. 文件追加操作:\n";

    // 追加到 JSON 对象文件
    JsonFile::append("config.json", "theme", Json("dark"));
    JsonFile::append("config.json", "language", Json("zh-CN"));

    // 批量追加
    std::map<std::string, Json> updates = {
        {"notifications", Json(true)},
        {"auto_save", Json(true)},
        {"font_size", Json(14)}
    };
    JsonFile::append("config.json", updates);

    // 读取查看
    Json config = JsonFile::read("config.json");
    std::cout << "配置文件内容:\n" << config.dump(true) << "\n\n";

    // ============================================================
    // 11. 异常处理
    // ============================================================
    std::cout << "11. 异常处理:\n";

    try {
        // 尝试解析无效 JSON
        Json::parse("{invalid json}");
    }
    catch (const JsonException& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }

    try {
        // 尝试读取不存在的文件
        Json::load("non_existent_example.json");
    }
    catch (const JsonFileException& e) {
        std::cout << "文件异常: " << e.what() << "\n";
    }

    std::cout << "\n========== 示例结束 ==========\n";
    return 0;
}