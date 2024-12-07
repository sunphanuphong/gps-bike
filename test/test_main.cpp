#include <Arduino.h>
#include <unity.h>

// ฟังก์ชัน setup สำหรับ Unity
void setUp(void) {
  // การเตรียมการทดสอบ
}

// ฟังก์ชัน tearDown สำหรับ Unity
void tearDown(void) {
  // การทำความสะอาดหลังการทดสอบ
}

// ฟังก์ชันทดสอบพื้นฐาน
void test_example(void) {
  TEST_ASSERT_EQUAL(1, 1); // ทดสอบความเท่ากันของ 1 และ 1
}

// ฟังก์ชัน setup สำหรับ Arduino
void setup() {
  UNITY_BEGIN(); // เริ่มต้นการทดสอบ
  RUN_TEST(test_example); // เรียกใช้ฟังก์ชันทดสอบ
  UNITY_END(); // สิ้นสุดการทดสอบ
}

// ฟังก์ชัน loop สำหรับ Arduino (ไม่ใช้ในกรณีนี้)
void loop() {
  // ไม่มีการทำงานใน loop สำหรับการทดสอบ
}
