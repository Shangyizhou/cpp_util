#include "message.pb.h"
#include "logger.h"
#include <gtest/gtest.h>
#include <fstream>

class ProtobufTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 初始化测试数据
        original_person_.set_name("Alice");
        original_person_.set_id(123);
        original_person_.add_emails("alice@example.com");
        original_person_.mutable_attributes()->insert({"language", "C++"});
        
        // 序列化到临时文件
        std::ofstream out("test_person.bin", std::ios::binary);
        original_person_.SerializeToOstream(&out);
        out.close();
    }

    void TearDown() override {
        // 清理临时文件
        std::remove("test_person.bin");
    }

    example::Person original_person_;
};

// 测试序列化和反序列化的完整性
TEST_F(ProtobufTest, SerializationRoundTrip) {
    // 反序列化
    example::Person restored_person;
    std::ifstream in("test_person.bin", std::ios::binary);
    ASSERT_TRUE(restored_person.ParseFromIstream(&in));
    in.close();

    // 验证字段值
    EXPECT_EQ(original_person_.name(), restored_person.name());
    EXPECT_EQ(original_person_.id(), restored_person.id());
    ASSERT_EQ(original_person_.emails_size(), restored_person.emails_size());
    EXPECT_EQ(original_person_.emails(0), restored_person.emails(0));
    EXPECT_EQ(original_person_.attributes().at("language"), 
              restored_person.attributes().at("language"));
}

// 测试重复字段操作
TEST_F(ProtobufTest, RepeatedFieldOperations) {
    example::Person person;
    person.add_emails("test@example.com");
    person.add_emails("dev@example.com");

    EXPECT_EQ(person.emails_size(), 2);
    EXPECT_EQ(person.emails(0), "test@example.com");
    EXPECT_EQ(person.emails(1), "dev@example.com");
}

// 测试日志输出（可选）
TEST_F(ProtobufTest, LogOutput) {
    testing::internal::CaptureStdout();  // 捕获 stdout
    LOG_INFO("Test log message");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Test log message") != std::string::npos);
}

int main(int argc, char** argv) {
    // 初始化 Protobuf 和 GTest
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    ::testing::InitGoogleTest(&argc, argv);
    
    // 运行测试
    int ret = RUN_ALL_TESTS();
    
    // 清理 Protobuf
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}