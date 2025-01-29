#include <Arduino.h>
#include <unity.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

void setUp(void) {
}

void tearDown(void) {
    // Очистка после каждого теста
    // LittleFS.format();
}

//void test_littlefs_format(void) {
    // Действие
//    LittleFS.format();
//}

void test_littlefs_write(void) {
    // Действие
    File file = LittleFS.open("test.txt", "w");
    file.print("Hello, World!");
    file.close();
}

void test_littlefs_read(void) {
    // Действие
    File file = LittleFS.open("test.txt", "r");
    String fileContent = file.readString();
    file.close();
    TEST_ASSERT_EQUAL_STRING("Hello, World!", fileContent.c_str());
}

void test_littlefs_exists(void) {
    // Действие
    TEST_ASSERT_TRUE(LittleFS.exists("test.txt"));
}

void test_littlefs_remove(void) {
    // Действие
    LittleFS.remove("test.txt");
}

void test_littlefs_list(void) {
    // Действие
    listDir(LittleFS, "/", 10);
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    LittleFS.begin();

    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_littlefs_write);
    RUN_TEST(test_littlefs_read);
    RUN_TEST(test_littlefs_exists);
    RUN_TEST(test_littlefs_remove);
    RUN_TEST(test_littlefs_list);

    UNITY_END();
}

void loop()
{
    // UNITY_END(); // stop unit testing
}