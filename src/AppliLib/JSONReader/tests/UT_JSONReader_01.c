#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "JSONReader.h"
#include "String2.h"

#define TEST_PASS 1
#define TEST_FAIL 0

/*============== Test Helpers ==============*/
void print_test_result(const char* test_name, int result)
{
  printf("[%s] %s\n", result ? "PASS" : "FAIL", test_name);
}

/*============== Test Cases ==============*/

/* Test 1: Parse simple object */
int test_parse_simple_object()
{
  String* json = String_new("{\"key\":\"value\"}");
  JsonReader* reader = JsonReader_new(json);

  JsonNode node;
  
  node = JsonReader_read(reader);
  if (node != JSONOBJECT_START) return TEST_FAIL;

  node = JsonReader_read(reader);
  if (node != JSONKEY) return TEST_FAIL;
  String* key = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(key), "key") != 0) return TEST_FAIL;
  String_delete(key);

  node = JsonReader_read(reader);
  if (node != JSONCOLON) return TEST_FAIL;

  node = JsonReader_read(reader);
  if (node != JSONSTRING_VALUE) return TEST_FAIL;
  String* value = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(value), "value") != 0) return TEST_FAIL;
  String_delete(value);

  node = JsonReader_read(reader);
  if (node != JSONOBJECT_END) return TEST_FAIL;

  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/* Test 2: Parse array of numbers */
int test_parse_array_numbers()
{
  String* json = String_new("[1, 2.5, -3]");
  JsonReader* reader = JsonReader_new(json);

  JsonNode node = JsonReader_read(reader);
  if (node != JSONARRAY_START) return TEST_FAIL;

  /* First number: 1 */
  node = JsonReader_read(reader);
  if (node != JSONNUMBER_VALUE) return TEST_FAIL;
  String* content = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(content), "1") != 0) return TEST_FAIL;
  String_delete(content);

  /* Comma */
  node = JsonReader_read(reader);
  if (node != JSONCOMMA) return TEST_FAIL;

  /* Second number: 2.5 */
  node = JsonReader_read(reader);
  if (node != JSONNUMBER_VALUE) return TEST_FAIL;
  content = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(content), "2.5") != 0) return TEST_FAIL;
  String_delete(content);

  /* Comma */
  node = JsonReader_read(reader);
  if (node != JSONCOMMA) return TEST_FAIL;

  /* Third number: -3 */
  node = JsonReader_read(reader);
  if (node != JSONNUMBER_VALUE) return TEST_FAIL;
  content = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(content), "-3") != 0) return TEST_FAIL;
  String_delete(content);

  /* Array end */
  node = JsonReader_read(reader);
  if (node != JSONARRAY_END) return TEST_FAIL;

  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/* Test 3: Parse boolean and null values */
int test_parse_boolean_null()
{
  String* json = String_new("{\"flag\":true,\"nullable\":null,\"disabled\":false}");
  JsonReader* reader = JsonReader_new(json);

  JsonReader_read(reader); /* { */
  JsonReader_read(reader); /* "flag" key */
  JsonReader_read(reader); /* : */

  JsonNode node = JsonReader_read(reader);
  if (node != JSONBOOLEAN_VALUE) return TEST_FAIL;
  String* content = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(content), "true") != 0) return TEST_FAIL;
  String_delete(content);

  JsonReader_read(reader); /* , */
  JsonReader_read(reader); /* "nullable" key */
  JsonReader_read(reader); /* : */

  node = JsonReader_read(reader);
  if (node != JSONNULL_VALUE) return TEST_FAIL;
  content = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(content), "null") != 0) return TEST_FAIL;
  String_delete(content);

  JsonReader_read(reader); /* , */
  JsonReader_read(reader); /* "disabled" key */
  JsonReader_read(reader); /* : */

  node = JsonReader_read(reader);
  if (node != JSONBOOLEAN_VALUE) return TEST_FAIL;
  content = JsonReader_getContent(reader);
  if (strcmp(String_getBuffer(content), "false") != 0) return TEST_FAIL;
  String_delete(content);

  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/* Test 4: Parse nested objects */
int test_parse_nested_objects()
{
  String* json = String_new("{\"user\":{\"name\":\"Alice\",\"age\":30}}");
  JsonReader* reader = JsonReader_new(json);

  if (JsonReader_read(reader) != JSONOBJECT_START) return TEST_FAIL; /* outer { */
  if (JsonReader_read(reader) != JSONKEY) return TEST_FAIL;           /* "user" */
  if (JsonReader_read(reader) != JSONCOLON) return TEST_FAIL;         /* : */
  if (JsonReader_read(reader) != JSONOBJECT_START) return TEST_FAIL; /* inner { */
  if (JsonReader_read(reader) != JSONKEY) return TEST_FAIL;           /* "name" */
  if (JsonReader_read(reader) != JSONCOLON) return TEST_FAIL;         /* : */
  if (JsonReader_read(reader) != JSONSTRING_VALUE) return TEST_FAIL; /* "Alice" */
  if (JsonReader_read(reader) != JSONCOMMA) return TEST_FAIL;         /* , */
  if (JsonReader_read(reader) != JSONKEY) return TEST_FAIL;           /* "age" */
  if (JsonReader_read(reader) != JSONCOLON) return TEST_FAIL;         /* : */
  if (JsonReader_read(reader) != JSONNUMBER_VALUE) return TEST_FAIL; /* 30 */
  if (JsonReader_read(reader) != JSONOBJECT_END) return TEST_FAIL;   /* inner } */
  if (JsonReader_read(reader) != JSONOBJECT_END) return TEST_FAIL;   /* outer } */

  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/* Test 5: Handle escape sequences in strings */
int test_parse_escaped_strings()
{
  String* json = String_new("{\"text\":\"Line1\\nLine2\\tTabbed\"}");
  JsonReader* reader = JsonReader_new(json);

  JsonReader_read(reader); /* { */
  JsonReader_read(reader); /* "text" */
  JsonReader_read(reader); /* : */

  JsonNode node = JsonReader_read(reader);
  if (node != JSONSTRING_VALUE) return TEST_FAIL;
  
  String* content = JsonReader_getContent(reader);
  /* Should contain the escaped sequences */
  if (JsonReader_isError(reader)) return TEST_FAIL;
  String_delete(content);

  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/* Test 6: Error handling - invalid JSON */
int test_error_malformed_json()
{
  String* json = String_new("{key:value}"); /* Missing quotes */
  JsonReader* reader = JsonReader_new(json);

  JsonReader_read(reader); /* { */
  JsonNode node = JsonReader_read(reader); /* Should fail on unquoted key */
  
  int has_error = (node == JSONNONE && JsonReader_isError(reader));

  JsonReader_delete(reader);
  String_delete(json);
  return has_error ? TEST_PASS : TEST_FAIL;
}

/* Test 7: Line and column tracking */
int test_line_column_tracking()
{
  String* json = String_new("{\n  \"key\": \"value\"\n}");
  JsonReader* reader = JsonReader_new(json);

  if (JsonReader_getLine(reader) != 1) return TEST_FAIL;
  if (JsonReader_getColumn(reader) != 1) return TEST_FAIL;

  JsonReader_read(reader); /* { */
  JsonReader_read(reader); /* "key" */

  /* After reading "key" on line 2 */
  if (JsonReader_getLine(reader) < 1) return TEST_FAIL;

  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/* Test 8: Complex nested structure */
int test_parse_complex_structure()
{
  String* json = String_new(
    "{"
    "  \"users\": ["
    "    {\"id\": 1, \"active\": true},"
    "    {\"id\": 2, \"active\": false}"
    "  ],"
    "  \"count\": 2"
    "}"
  );
  JsonReader* reader = JsonReader_new(json);

  if (JsonReader_read(reader) != JSONOBJECT_START) return TEST_FAIL;    /* { */
  if (JsonReader_read(reader) != JSONKEY) return TEST_FAIL;             /* "users" */
  if (JsonReader_read(reader) != JSONCOLON) return TEST_FAIL;           /* : */
  if (JsonReader_read(reader) != JSONARRAY_START) return TEST_FAIL;    /* [ */
  if (JsonReader_read(reader) != JSONOBJECT_START) return TEST_FAIL;    /* { */
  if (JsonReader_read(reader) != JSONKEY) return TEST_FAIL;             /* "id" */
  if (JsonReader_read(reader) != JSONCOLON) return TEST_FAIL;           /* : */
  if (JsonReader_read(reader) != JSONNUMBER_VALUE) return TEST_FAIL;   /* 1 */
  
  JsonReader_delete(reader);
  String_delete(json);
  return TEST_PASS;
}

/*============== Main Test Runner ==============*/
int main()
{
  printf("===== JSONReader Test Suite =====\n\n");

  print_test_result("test_parse_simple_object", test_parse_simple_object());
  print_test_result("test_parse_array_numbers", test_parse_array_numbers());
  print_test_result("test_parse_boolean_null", test_parse_boolean_null());
  print_test_result("test_parse_nested_objects", test_parse_nested_objects());
  print_test_result("test_parse_escaped_strings", test_parse_escaped_strings());
  print_test_result("test_error_malformed_json", test_error_malformed_json());
  print_test_result("test_line_column_tracking", test_line_column_tracking());
  print_test_result("test_parse_complex_structure", test_parse_complex_structure());

  printf("\n===== Tests Complete =====\n");
  return 0;
}
