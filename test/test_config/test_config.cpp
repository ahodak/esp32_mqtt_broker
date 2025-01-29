#include <Arduino.h>
#include <unity.h>
#include <config.hpp>

#define TEST_CONFIG_FILE "/tests/config.json"
void setUp(void) {
}

void tearDown(void) {
}

void test_config_save(void) {
    Config config = Config(TEST_CONFIG_FILE);
    config.setString("test1", "string", "test11");
    config.setInt("test1", "int", 123);
    config.setFloat("test1", "float", static_cast<float>(123.456));
    config.setBool("test1", "bool", true);
    config.setString("test2", "string", "test21");
    config.setInt("test2", "int", 1234567890);
    config.setFloat("test2", "float", static_cast<double>(1234567890.789));
    config.setBool("test2", "bool", false);
    config.save();
}

void test_config_load(void) {
    Config config = Config(TEST_CONFIG_FILE);
    config.load();

    String string_value = config.getString("test1", "string", "default_value");
    TEST_ASSERT_EQUAL_STRING("test11", string_value.c_str());
}

void test_get_string(void) {
    Config config = Config(TEST_CONFIG_FILE);
    String value = config.getString("test1", "string", "default_value");
    TEST_ASSERT_EQUAL_STRING("test11", value.c_str());
}

void test_get_int(void) {
    Config config = Config(TEST_CONFIG_FILE);
    int value = config.getInt("test1", "int", 0);
    TEST_ASSERT_EQUAL_INT(123, value);
}

void test_get_float(void) {
    Config config = Config(TEST_CONFIG_FILE);
    float value = config.getFloat("test1", "float", 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(123.456, value);
}

void test_get_bool(void) {
    Config config = Config(TEST_CONFIG_FILE);
    bool value = config.getBool("test1", "bool", false);
    TEST_ASSERT_TRUE(value);
}

void test_config_remove(void) {
    Config config = Config(TEST_CONFIG_FILE);
    config.remove();
}

void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_config_save);
    RUN_TEST(test_config_load);
    RUN_TEST(test_get_string);
    RUN_TEST(test_get_int);
    RUN_TEST(test_get_float);
    RUN_TEST(test_get_bool);
    RUN_TEST(test_config_remove);

    UNITY_END();
}