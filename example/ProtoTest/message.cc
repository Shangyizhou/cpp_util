#include "message.pb.h"
#include "logger.h"
#include <fstream>
#include <iostream>

int main() {
    // 验证 Protobuf 库版本
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // 创建并填充对象
    example::Person person;
    person.set_name("Alice");
    person.set_id(123);
    person.add_emails("alice@example.com");
    person.mutable_attributes()->insert({"language", "C++"});

    // 序列化为二进制文件
    std::ofstream out("person.bin", std::ios::binary);
    person.SerializeToOstream(&out);
    out.close();

    // 从文件反序列化
    example::Person restored_person;
    std::ifstream in("person.bin", std::ios::binary);
    restored_person.ParseFromIstream(&in);
    in.close();

    // 打印数据
    std::cout << "Restored Name: " << restored_person.name() << std::endl;
    std::cout << "Restored ID: " << restored_person.id() << std::endl;
    for (const auto& email : restored_person.emails()) {
        LOG_INFO("Restored Email: {}", email);
    }
    
    for (const auto& [key, value] : restored_person.attributes()) {
        LOG_INFO("Restored Attribute {} {}", key, value);
    }

    // 清理 Protobuf 资源（程序退出前调用）
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}