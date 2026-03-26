#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "JSONWriter.h"
#include "String2.h"

#define TEST_PASS 1
#define TEST_FAIL 0

void print_test_result(const char* test_name, int result)
{
  printf("[%s] %s\n", result ? "PASS" : "FAIL", test_name);
}

/* Test 1: Write simple object */
int test_write_simple_object()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "name");
  JsonWriter_writeStringValue(writer, "Alice");
  JsonWriter_writeKey(writer, "age");
  JsonWriter_writeIntValue(writer, 30);
  JsonWriter_endObject(writer);

  String* result = JsonWriter_getResult(writer);
  const char* expected = "{\"name\":\"Alice\",\"age\":30}";
  
  int success = (strcmp(String_getBuffer(result), expected) == 0);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 2: Write array of numbers */
int test_write_array_numbers()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startArray(writer);
  JsonWriter_writeArrayInt(writer, 1);
  JsonWriter_writeArrayNumber(writer, 2.5);
  JsonWriter_writeArrayInt(writer, -3);
  JsonWriter_endArray(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (strstr(json_str, "[") != NULL && 
                 strstr(json_str, "]") != NULL &&
                 strstr(json_str, "1") != NULL &&
                 strstr(json_str, "2.5") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 3: Write boolean and null values */
int test_write_boolean_null()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "active");
  JsonWriter_writeBooleanValue(writer, 1);
  JsonWriter_writeKey(writer, "inactive");
  JsonWriter_writeBooleanValue(writer, 0);
  JsonWriter_writeKey(writer, "data");
  JsonWriter_writeNullValue(writer);
  JsonWriter_endObject(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (strstr(json_str, "true") != NULL && 
                 strstr(json_str, "false") != NULL &&
                 strstr(json_str, "null") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 4: Write nested objects */
int test_write_nested_objects()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "user");
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "name");
  JsonWriter_writeStringValue(writer, "Bob");
  JsonWriter_writeKey(writer, "email");
  JsonWriter_writeStringValue(writer, "bob@example.com");
  JsonWriter_endObject(writer);
  JsonWriter_endObject(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (strstr(json_str, "\"user\"") != NULL && 
                 strstr(json_str, "\"name\"") != NULL &&
                 strstr(json_str, "\"Bob\"") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 5: Write escaped strings */
int test_write_escaped_strings()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "text");
  JsonWriter_writeStringValue(writer, "Line1\nLine2\tTabbed\"quoted\"");
  JsonWriter_endObject(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (strstr(json_str, "\\n") != NULL && 
                 strstr(json_str, "\\t") != NULL &&
                 strstr(json_str, "\\\"") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 6: Complex nested structure */
int test_write_complex_structure()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "users");
  JsonWriter_startArray(writer);
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "id");
  JsonWriter_writeIntValue(writer, 1);
  JsonWriter_writeKey(writer, "active");
  JsonWriter_writeBooleanValue(writer, 1);
  JsonWriter_endObject(writer);
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "id");
  JsonWriter_writeIntValue(writer, 2);
  JsonWriter_writeKey(writer, "active");
  JsonWriter_writeBooleanValue(writer, 0);
  JsonWriter_endObject(writer);
  
  JsonWriter_endArray(writer);
  JsonWriter_writeKey(writer, "count");
  JsonWriter_writeIntValue(writer, 2);
  JsonWriter_endObject(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (JsonWriter_getError(writer) == 0 &&
                 strstr(json_str, "\"users\"") != NULL &&
                 strstr(json_str, "\"count\"") != NULL &&
                 strstr(json_str, "[") != NULL &&
                 strstr(json_str, "]") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 7: Error handling - invalid nesting */
int test_error_invalid_nesting()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "test");
  int result = JsonWriter_writeArrayString(writer, "invalid"); /* Array string in object */
  
  int has_error = (result != 0 || JsonWriter_getError(writer) != 0);
  
  JsonWriter_delete(writer);
  
  return has_error ? TEST_PASS : TEST_FAIL;
}

/* Test 8: Reset functionality */
int test_reset_functionality()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "first");
  JsonWriter_writeStringValue(writer, "value");
  JsonWriter_endObject(writer);
  
  JsonWriter_reset(writer);
  
  JsonWriter_startArray(writer);
  JsonWriter_writeArrayInt(writer, 42);
  JsonWriter_endArray(writer);
  
  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (strstr(json_str, "[42]") != NULL && 
                 strstr(json_str, "first") == NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 9: Floating point numbers */
int test_float_numbers()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startObject(writer);
  JsonWriter_writeKey(writer, "pi");
  JsonWriter_writeNumberValue(writer, 3.14159);
  JsonWriter_writeKey(writer, "small");
  JsonWriter_writeNumberValue(writer, 0.001);
  JsonWriter_writeKey(writer, "large");
  JsonWriter_writeNumberValue(writer, 1e10);
  JsonWriter_endObject(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (JsonWriter_getError(writer) == 0 &&
                 strstr(json_str, "3.14") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/* Test 10: Array of mixed types */
int test_mixed_array()
{
  JsonWriter* writer = JsonWriter_new();
  
  JsonWriter_startArray(writer);
  JsonWriter_writeArrayString(writer, "text");
  JsonWriter_writeArrayInt(writer, 42);
  JsonWriter_writeArrayBoolean(writer, 1);
  JsonWriter_writeArrayNull(writer);
  JsonWriter_endArray(writer);

  String* result = JsonWriter_getResult(writer);
  const char* json_str = String_getBuffer(result);
  
  int success = (strstr(json_str, "\"text\"") != NULL && 
                 strstr(json_str, "42") != NULL &&
                 strstr(json_str, "true") != NULL &&
                 strstr(json_str, "null") != NULL);
  
  String_delete(result);
  JsonWriter_delete(writer);
  
  return success ? TEST_PASS : TEST_FAIL;
}

/*============== Main Test Runner ==============*/
int main()
{
  printf("===== JSONWriter Test Suite =====\n\n");

  print_test_result("test_write_simple_object", test_write_simple_object());
  print_test_result("test_write_array_numbers", test_write_array_numbers());
  print_test_result("test_write_boolean_null", test_write_boolean_null());
  print_test_result("test_write_nested_objects", test_write_nested_objects());
  print_test_result("test_write_escaped_strings", test_write_escaped_strings());
  print_test_result("test_write_complex_structure", test_write_complex_structure());
  print_test_result("test_error_invalid_nesting", test_error_invalid_nesting());
  print_test_result("test_reset_functionality", test_reset_functionality());
  print_test_result("test_float_numbers", test_float_numbers());
  print_test_result("test_mixed_array", test_mixed_array());

  printf("\n===== Tests Complete =====\n");
  return 0;
}
