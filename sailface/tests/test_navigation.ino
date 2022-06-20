#line 2 "test_navigation.ino"

#include <AUnit.h>

// test-once test named "once"
test(once) {
  int x = 1;
  assertLessOrEqual(x, 1);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while(!Serial); // for the Arduino Leonardo/Micro only

  Serial.println(F("This test should produce the following:"));
  Serial.println(
    F("1 passed, 0 failed, 0 skipped, 0 timed out, out of 1 test(s).")
  );
  Serial.println(F("----"));
}

void loop() {
  aunit::TestRunner::run();
}
